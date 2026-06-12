#include <math.h>
#include "flanger.h"
#include "keyvalue.h"
#include "instrument.h"

using namespace upc;
using namespace std;

Flanger::Flanger(const std::string &param) {
  KeyValue kv(param);

  if (!kv.to_float("fm", fm))
    fm = 0.5f;
  if (!kv.to_float("depth", depth))
    depth = 3.0f;
  if (!kv.to_float("fb", fb))
    fb = 0.7f;

  max_delay_samples = (depth / 1000.0f) * (float) SamplingRate;

  unsigned int buf_size = (unsigned int) ceil(max_delay_samples) + 2;
  buffer.assign(buf_size, 0.0f);
  write_idx = 0;

  phase = 0.0f;
  inc_phase = 2.0f * M_PI * fm / (float) SamplingRate;
}

void Flanger::command(unsigned int comm) {
  if (comm == 1) {
    phase = 0.0f;
    fill(buffer.begin(), buffer.end(), 0.0f);
    write_idx = 0;
  }
}

void Flanger::operator()(std::vector<float> &x) {
  const unsigned int N = buffer.size();

  for (unsigned int i = 0; i < x.size(); i++) {
    float delay = 0.5f * max_delay_samples * (1.0f + sinf(phase));
    if (delay < 1.0f) delay = 1.0f;
    if (delay > (float)(N - 1)) delay = (float)(N - 1);

    float read_pos = (float) write_idx - delay;
    while (read_pos < 0.0f) read_pos += (float) N;

    unsigned int i0 = (unsigned int) read_pos;
    unsigned int i1 = (i0 + 1) % N;
    float frac = read_pos - (float) i0;

    float delayed = buffer[i0] + frac * (buffer[i1] - buffer[i0]);

    float in = x[i];
    buffer[write_idx] = in + fb * delayed;

    x[i] = in + fb * delayed;

    write_idx = (write_idx + 1) % N;
    phase += inc_phase;
    if (phase > 2.0f * M_PI) phase -= 2.0f * M_PI;
  }
}
