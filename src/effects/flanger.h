#ifndef FLANGER_H
#define FLANGER_H

#include <vector>
#include <string>
#include "effect.h"

namespace upc {
  class Flanger: public upc::Effect {
    private:
      float fm, depth, fb;
      float max_delay_samples;
      float phase, inc_phase;
      std::vector<float> buffer;
      unsigned int write_idx;
    public:
      Flanger(const std::string &param = "");
      void operator()(std::vector<float> &x);
      void command(unsigned int);
  };
}

#endif
