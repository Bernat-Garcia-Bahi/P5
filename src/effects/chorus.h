#ifndef CHORUS_H
#define CHORUS_H

#include <vector>
#include <string>
#include "effect.h"

namespace upc {
  class Chorus: public upc::Effect {
    private:
      float fm;
      float delay_ms, depth_ms;
      float mix;
      float center_samples, depth_samples;
      float phase, inc_phase;
      std::vector<float> buffer;
      unsigned int write_idx;
    public:
      Chorus(const std::string &param = "");
      void operator()(std::vector<float> &x);
      void command(unsigned int);
  };
}

#endif
