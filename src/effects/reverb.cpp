#include <math.h>
#include "reverb.h"
#include "keyvalue.h"

using namespace upc;
using namespace std;

static const unsigned int COMB_LENGTHS[4]   = { 1557, 1617, 1491, 1422 };
static const unsigned int ALLPASS_LENGTHS[2] = { 556, 441 };

Reverb::Reverb(const std::string &param) {
  KeyValue kv(param);

  if (!kv.to_float("room_size", room_size))
    room_size = 0.5f;
  if (!kv.to_float("damp", damp))
    damp = 0.2f;

  for (unsigned int i = 0; i < N_COMB; i++) {
    comb_buf[i].assign(COMB_LENGTHS[i], 0.0f);
    comb_idx[i] = 0;
    comb_lp[i]  = 0.0f;
  }
  for (unsigned int i = 0; i < N_ALLPASS; i++) {
    ap_buf[i].assign(ALLPASS_LENGTHS[i], 0.0f);
    ap_idx[i] = 0;
  }
}

void Reverb::command(unsigned int comm) {
  if (comm == 1) {
    for (unsigned int i = 0; i < N_COMB; i++) {
      fill(comb_buf[i].begin(), comb_buf[i].end(), 0.0f);
      comb_idx[i] = 0;
      comb_lp[i]  = 0.0f;
    }
    for (unsigned int i = 0; i < N_ALLPASS; i++) {
      fill(ap_buf[i].begin(), ap_buf[i].end(), 0.0f);
      ap_idx[i] = 0;
    }
  }
}

void Reverb::operator()(std::vector<float> &x) {
  const float wet = 0.3f;
  const float dry = 0.7f;
  const float ap_fb = 0.5f;

  for (unsigned int n = 0; n < x.size(); n++) {
    float in = x[n];
    float comb_sum = 0.0f;

    for (unsigned int i = 0; i < N_COMB; i++) {
      float delayed = comb_buf[i][comb_idx[i]];
      comb_lp[i] = delayed * (1.0f - damp) + comb_lp[i] * damp;
      comb_buf[i][comb_idx[i]] = in + comb_lp[i] * room_size;
      comb_idx[i] = (comb_idx[i] + 1) % comb_buf[i].size();
      comb_sum += delayed;
    }

    float y = comb_sum * 0.25f;

    for (unsigned int i = 0; i < N_ALLPASS; i++) {
      float delayed = ap_buf[i][ap_idx[i]];
      float v = y + ap_fb * delayed;
      ap_buf[i][ap_idx[i]] = v;
      ap_idx[i] = (ap_idx[i] + 1) % ap_buf[i].size();
      y = delayed - ap_fb * v;
    }

    x[n] = dry * in + wet * y;
  }
}
