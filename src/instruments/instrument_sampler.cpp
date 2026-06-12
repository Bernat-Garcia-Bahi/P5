#include <cmath>
#include <sndfile.h>
#include "instrument_sampler.h"
#include "keyvalue.h"

using namespace std;
using namespace upc;

map<string, weak_ptr<vector<float>>> InstrumentSampler::cache;

shared_ptr<vector<float>> InstrumentSampler::load(const string &filename) {
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

InstrumentSampler::InstrumentSampler(const string &param)
    : adsr(SamplingRate, param), phase(0), step(1), A(1), ft(SamplingRate) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  float f;
  if (kv.to_float("ft", f)) ft = f; // frecuencia de grabación original; por defecto: igual que la de salida

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

void InstrumentSampler::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.0f;
    phase = 0;
    double f0 = 440.0 * pow(2.0, (note - 69) / 12.0);
    // step = f0/ft: si f0==ft se lee cada muestra (tono original);
    // f0 > ft implica lectura más rápida (tono más alto), f0 < ft más lenta (tono más bajo)
    step = f0 / ft;
  } else if (cmd == 8) {
    adsr.stop();
  } else if (cmd == 0) {
    adsr.end();
  }
}

const vector<float> &InstrumentSampler::synthesize() {
  if (!adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  if (!tbl || tbl->empty() || !bActive) {
    x.assign(x.size(), 0);
    return x;
  }

  const size_t L = tbl->size();
  for (size_t i = 0; i < x.size(); ++i) {
    // se para una muestra antes del final para que idx+1 sea siempre un índice válido
    if (phase >= (double)(L - 1)) {
      x[i] = 0;
      for (++i; i < x.size(); ++i) x[i] = 0;
      adsr.end(); // fuerza el ADSR a inactivo; adsr(x) a continuación pondrá a cero el resto
      break;
    }
    size_t idx = (size_t)phase;
    double frac = phase - idx;
    // interpolación lineal: x = (1-frac)*tbl[idx] + frac*tbl[idx+1]
    x[i] = A * ((float)(1.0 - frac) * (*tbl)[idx] + (float)frac * (*tbl)[idx + 1]);
    phase += step;
  }
  adsr(x); // aplica la envolvente ADSR sobre el bloque y avanza su estado interno
  return x;
}
