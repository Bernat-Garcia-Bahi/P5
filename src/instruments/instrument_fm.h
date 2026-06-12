#ifndef INSTRUMENT_FM
#define INSTRUMENT_FM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM: public upc::Instrument {
    EnvelopeADSR adsr;
    float f0;
    float A;
    float I_st;   // modulation index expressed in semitones (peak pitch deviation)
    float beta;   // dimensionless modulation index used by the Chowning formula
    float N1;
    float N2;
    double phase_c;
    double phase_m;
    double inc_phase_c;
    double inc_phase_m;
  public:
    InstrumentFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1);
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;}
  };
}

#endif
