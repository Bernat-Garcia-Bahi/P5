#include <math.h>
#include "seno.h"
#include "keyvalue.h"

using namespace upc;
using namespace std;

Seno::Seno(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  int N;

  if (!kv.to_int("N", N))
    N = 1024;

  tbl.resize(N);
  float ph = 0.0f;
  float dph = 2.0f * (float) M_PI / (float) N;
  for (int i = 0; i < N; ++i) {
    tbl[i] = sinf(ph);
    ph += dph;
  }

  phase = 0.0f;
  step = 0.0f;
  A = 0.0f;
}

void Seno::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.0f;
    float f0 = 440.0f * powf(2.0f, (note - 69) / 12.0f);
    step = f0 * (float) tbl.size() / (float) SamplingRate;
    phase = 0.0f;
  }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}

const vector<float> & Seno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  const float N = (float) tbl.size();
  for (unsigned int i = 0; i < x.size(); ++i) {
    unsigned int i0 = (unsigned int) phase;
    unsigned int i1 = (i0 + 1) % tbl.size();
    float frac = phase - (float) i0;
    x[i] = A * (tbl[i0] + frac * (tbl[i1] - tbl[i0]));

    phase += step;
    while (phase >= N) phase -= N;
  }

  adsr(x);
  return x;
}
