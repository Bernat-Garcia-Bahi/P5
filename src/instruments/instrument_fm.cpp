#include <math.h>
#include "instrument_fm.h"
#include "keyvalue.h"

using namespace upc;
using namespace std;

InstrumentFM::InstrumentFM(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);

  if (!kv.to_float("I", I_st))
    I_st = 2.0f;
  if (!kv.to_float("N1", N1))
    N1 = 1.0f;
  if (!kv.to_float("N2", N2))
    N2 = 1.0f;

  // Peak frequency deviation: Δf = f0·(2^(I_st/12) − 1)
  // Dimensionless Chowning modulation index: β = Δf / fm = (2^(I_st/12) − 1) / N2
  if (N2 > 0.0f)
    beta = (powf(2.0f, I_st / 12.0f) - 1.0f) / N2;
  else
    beta = 0.0f;

  f0 = 0.0f;
  A = 0.0f;
  phase_c = 0.0;
  phase_m = 0.0;
  inc_phase_c = 0.0;
  inc_phase_m = 0.0;
}

void InstrumentFM::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.0f;
    f0 = 440.0f * powf(2.0f, (note - 69) / 12.0f);
    float fc = N1 * f0;
    float fmod = N2 * f0;
    inc_phase_c = 2.0 * M_PI * (double) fc / (double) SamplingRate;
    inc_phase_m = 2.0 * M_PI * (double) fmod / (double) SamplingRate;
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

const vector<float> & InstrumentFM::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  const double two_pi = 2.0 * M_PI;

  for (unsigned int i = 0; i < x.size(); ++i) {
    double arg = phase_c + (double) beta * sin(phase_m);
    x[i] = A * (float) sin(arg);

    phase_c += inc_phase_c;
    phase_m += inc_phase_m;

    while (phase_c >= two_pi) phase_c -= two_pi;
    while (phase_m >= two_pi) phase_m -= two_pi;
  }

  adsr(x);
  return x;
}
