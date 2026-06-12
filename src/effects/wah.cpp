#include <math.h>
#include "wah.h"
#include "keyvalue.h"
#include "instrument.h"

using namespace upc;
using namespace std;

Wah::Wah(const std::string &param) {
  KeyValue kv(param);

  if (!kv.to_float("fm", fm))
    fm = 1.5f;
  if (!kv.to_float("fmin", fmin))
    fmin = 400.0f;
  if (!kv.to_float("fmax", fmax))
    fmax = 2200.0f;
  if (!kv.to_float("Q", Q))
    Q = 5.0f;

  phase = 0.0f;
  inc_phase = 2.0f * (float) M_PI * fm / (float) SamplingRate;

  z1 = 0.0f;
  z2 = 0.0f;
}

void Wah::command(unsigned int comm) {
  if (comm == 1) {
    phase = 0.0f;
    z1 = 0.0f;
    z2 = 0.0f;
  }
}

void Wah::operator()(std::vector<float> &x) {
  const float fs = (float) SamplingRate;
  const float two_pi = 2.0f * (float) M_PI;
  const float mid = 0.5f * (fmax + fmin);
  const float half = 0.5f * (fmax - fmin);

  for (unsigned int i = 0; i < x.size(); i++) {
    float fc = mid + half * sinf(phase);
    if (fc < 20.0f) fc = 20.0f;
    if (fc > 0.45f * fs) fc = 0.45f * fs;

    float w0 = two_pi * fc / fs;
    float alpha = sinf(w0) / (2.0f * Q);
    float cs = cosf(w0);

    float b0 = alpha;
    float b1 = 0.0f;
    float b2 = -alpha;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cs;
    float a2 = 1.0f - alpha;

    float in = x[i];
    float y = (b0 * in + z1) / a0;
    z1 = b1 * in - a1 * y + z2;
    z2 = b2 * in - a2 * y;

    x[i] = 0.5f * in + y;

    phase += inc_phase;
    if (phase > two_pi) phase -= two_pi;
  }
}
