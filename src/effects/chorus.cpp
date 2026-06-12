#include <math.h>
#include "chorus.h"
#include "keyvalue.h"
#include "instrument.h"

using namespace upc;
using namespace std;

Chorus::Chorus(const std::string &param) {
  KeyValue kv(param);

  if (!kv.to_float("fm", fm))
    fm = 0.8f;
  if (!kv.to_float("delay", delay_ms))
    delay_ms = 20.0f;
  if (!kv.to_float("depth", depth_ms))
    depth_ms = 5.0f;
  if (!kv.to_float("mix", mix))
    mix = 0.5f;

  center_samples = (delay_ms / 1000.0f) * (float) SamplingRate;
  depth_samples  = (depth_ms  / 1000.0f) * (float) SamplingRate;

  unsigned int buf_size = (unsigned int) ceil(center_samples + depth_samples) + 4;
  buffer.assign(buf_size, 0.0f);
  write_idx = 0;

  phase = 0.0f;
  inc_phase = 2.0f * (float) M_PI * fm / (float) SamplingRate;
}

void Chorus::command(unsigned int comm) {
  if (comm == 1) {
    phase = 0.0f;
    fill(buffer.begin(), buffer.end(), 0.0f);
    write_idx = 0;
  }
}

void Chorus::operator()(std::vector<float> &x) {
  const unsigned int N = buffer.size();
  const float two_pi = 2.0f * (float) M_PI;

  for (unsigned int i = 0; i < x.size(); i++) {
    float delay = center_samples + depth_samples * sinf(phase);
    if (delay < 1.0f) delay = 1.0f;
    if (delay > (float)(N - 2)) delay = (float)(N - 2);

    float read_pos = (float) write_idx - delay;
    while (read_pos < 0.0f) read_pos += (float) N;

    unsigned int i0 = (unsigned int) read_pos;
    unsigned int i1 = (i0 + 1) % N;
    float frac = read_pos - (float) i0;
    float delayed = buffer[i0] + frac * (buffer[i1] - buffer[i0]);

    float in = x[i];
    buffer[write_idx] = in;

    x[i] = (1.0f - mix) * in + mix * delayed;

    write_idx = (write_idx + 1) % N;
    phase += inc_phase;
    if (phase > two_pi) phase -= two_pi;
  }
}
