#ifndef INSTRUMENT_VIBRATO
#define INSTRUMENT_VIBRATO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentVibrato: public upc::Instrument {
    EnvelopeADSR adsr;
    float f0;
    float A;
    float fm;
    float I;
    double phase_c;
    double phase_m;
    double inc_phase_m;
  public:
    InstrumentVibrato(const std::string &param = "");
    void command(long cmd, long note, long velocity=1);
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;}
  };
}

#endif
