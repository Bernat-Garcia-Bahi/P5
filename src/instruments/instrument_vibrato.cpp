#include <math.h>
#include "instrument_vibrato.h"
#include "keyvalue.h"

using namespace upc;
using namespace std;

InstrumentVibrato::InstrumentVibrato(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);

  if (!kv.to_float("fm", fm))
    fm = 6.0f;
  if (!kv.to_float("I", I))
    I = 1.0f;

  inc_phase_m = 2.0 * M_PI * (double) fm / (double) SamplingRate;

  f0 = 0.0f;
  phase_c = 0.0;
  phase_m = 0.0;
  A = 0.0f;
}

void InstrumentVibrato::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.0f;
    f0 = 440.0f * powf(2.0f, (note - 69) / 12.0f);
    phase_c = 0.0;
    phase_m = 0.0;
  }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}

const vector<float> & InstrumentVibrato::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  const double two_pi = 2.0 * M_PI;
  const double inv_fs = 1.0 / (double) SamplingRate;

  for (unsigned int i = 0; i < x.size(); ++i) {
    double f_inst = (double) f0 + (double) I * (double) fm * sin(phase_m);
    phase_c += two_pi * f_inst * inv_fs;
    phase_m += inc_phase_m;

    while (phase_c >= two_pi) phase_c -= two_pi;
    while (phase_c < 0.0) phase_c += two_pi;
    while (phase_m >= two_pi) phase_m -= two_pi;

    x[i] = A * (float) sin(phase_c);
  }

  adsr(x);
  return x;
}
