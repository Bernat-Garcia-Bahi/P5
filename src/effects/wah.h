#ifndef WAH_H
#define WAH_H

#include <vector>
#include <string>
#include "effect.h"

namespace upc {
  class Wah: public upc::Effect {
    private:
      float fm;
      float fmin, fmax;
      float Q;
      float phase, inc_phase;
      float z1, z2;
    public:
      Wah(const std::string &param = "");
      void operator()(std::vector<float> &x);
      void command(unsigned int);
  };
}

#endif
