#include <cmath>
#include <sndfile.h>
#include "instrument_wavetable.h"
#include "keyvalue.h"

using namespace std;
using namespace upc;

map<string, weak_ptr<vector<float>>> InstrumentWavetable::cache;

shared_ptr<vector<float>> InstrumentWavetable::load(const string &filename) {
  // Devuelve el buffer cacheado si todavía está vivo (otro instrumento lo referencia)
  auto it = cache.find(filename);
  if (it != cache.end()) {
    auto ptr = it->second.lock();
    if (ptr) return ptr;
  }
  SF_INFO info = {};
  SNDFILE *sf = sf_open(filename.c_str(), SFM_READ, &info);
  if (!sf) return nullptr;

  // sf_readf_float lee muestras entrelazadas: [ch0, ch1, ...] por trama
  vector<float> raw(info.frames * info.channels);
  sf_readf_float(sf, raw.data(), info.frames);
  sf_close(sf);

  // Promedio de todos los canales para convertir a mono
  auto buf = make_shared<vector<float>>(info.frames);
  for (sf_count_t i = 0; i < info.frames; ++i) {
    float s = 0;
    for (int c = 0; c < info.channels; ++c)
      s += raw[i * info.channels + c];
    (*buf)[i] = s / info.channels;
  }
  cache[filename] = buf; // se almacena como weak_ptr; se libera cuando todos los shared_ptr desaparecen
  return buf;
}

InstrumentWavetable::InstrumentWavetable(const string &param)
    : adsr(SamplingRate, param), phase(0), step(0), A(1) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  string filename = kv("file");
  // KeyValue puede dejar espacios o comillas alrededor de la ruta del fichero
  auto trim = [](string s) {
    size_t a = s.find_first_not_of(" \t\r\n\"'");
    size_t b = s.find_last_not_of(" \t\r\n\"'");
    return (a == string::npos) ? string() : s.substr(a, b - a + 1);
  };
  filename = trim(filename);
  if (!filename.empty())
    tbl = load(filename);
}

void InstrumentWavetable::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.0f;
    phase = 0;
    if (tbl && !tbl->empty()) {
      double f0 = 440.0 * pow(2.0, (note - 69) / 12.0);
      // N = fm/f0 muestras por periodo de salida; step = L/N = L*f0/fm
      // así se recorre la tabla completa exactamente una vez por periodo de salida
      step = (double)tbl->size() * f0 / SamplingRate;
    }
  } else if (cmd == 8) {
    adsr.stop();
  } else if (cmd == 0) {
    adsr.end();
  }
}

const vector<float> &InstrumentWavetable::synthesize() {
  if (!adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  if (!tbl || tbl->empty() || !bActive) {
    x.assign(x.size(), 0);
    return x;
  }

  const double L = (double)tbl->size();
  for (size_t i = 0; i < x.size(); ++i) {
    size_t idx = (size_t)phase;
    double frac = phase - idx;
    size_t idx1 = (idx + 1) % tbl->size(); // módulo para que la tabla sea realmente periódica
    // interpolación lineal: x = (1-frac)*tbl[idx] + frac*tbl[idx+1]
    x[i] = A * ((float)(1.0 - frac) * (*tbl)[idx] + (float)frac * (*tbl)[idx1]);
    phase += step;
    if (phase >= L) phase -= L; // la resta es más barata que fmod para pasos pequeños
  }
  adsr(x); // aplica la envolvente ADSR sobre el bloque y avanza su estado interno
  return x;
}
