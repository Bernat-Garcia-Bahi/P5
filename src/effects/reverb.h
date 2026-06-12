#ifndef REVERB_H
#define REVERB_H

#include <vector>
#include <string>
#include "effect.h"

namespace upc {
  class Reverb: public upc::Effect {
    private:
      float room_size;
      float damp;

      static const unsigned int N_COMB = 4;
      static const unsigned int N_ALLPASS = 2;

      std::vector<float> comb_buf[N_COMB];
      unsigned int comb_idx[N_COMB];
      float comb_lp[N_COMB];

      std::vector<float> ap_buf[N_ALLPASS];
      unsigned int ap_idx[N_ALLPASS];

    public:
      Reverb(const std::string &param = "");
      void operator()(std::vector<float> &x);
      void command(unsigned int);
  };
}

#endif
