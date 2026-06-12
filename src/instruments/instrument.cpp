#include <iostream>
#include "instrument_dumb.h"
#include "instrument_wavetable.h" // síntesis por tabla de ondas periódica desde fichero WAV
#include "instrument_sampler.h"   // sampler de disparo único con transposición de tono
#include "instrument_vibrato.h"   // vibrato nativo via modulación FM de baja profundidad
#include "instrument_fm.h"        // síntesis FM de Chowning
#include "seno.h"                 // sinusoide por tabla con pitch correcto e interpolación lineal

/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
			      const string &parameters) {
    Instrument * pInst = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "InstrumentDumb") {
      pInst = (Instrument *) new InstrumentDumb(parameters);
    } else if (name == "InstrumentWavetable") {
      pInst = (Instrument *) new InstrumentWavetable(parameters);
    } else if (name == "InstrumentSampler") {
      pInst = (Instrument *) new InstrumentSampler(parameters);
    } else if (name == "vibrato" || name == "InstrumentVibrato") {
      pInst = (Instrument *) new InstrumentVibrato(parameters);
    } else if (name == "fm" || name == "InstrumentFM") {
      pInst = (Instrument *) new InstrumentFM(parameters);
    } else if (name == "Seno") {
      pInst = (Instrument *) new Seno(parameters);
    }
    return pInst;
  }
}
