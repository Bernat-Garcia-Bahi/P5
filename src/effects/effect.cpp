#include <iostream>
#include "tremolo.h"
#include "vibrato.h"
#include "distortion.h"
#include "flanger.h"
#include "reverb.h"
#include "wah.h"
#include "chorus.h"

/*
  For each new effect:
  - Add the header in this file
  - Add the call to the constructor in get_effect() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Effect *get_effect(const string &name, const string &parameters) {
    Effect *pEffect = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "Tremolo") {
      pEffect = (Effect *) new Tremolo(parameters);
    }
	else if (name == "Vibrato") {
      pEffect = (Effect *) new Vibrato(parameters);
    }
    else if (name == "Distortion") {
      pEffect = (Effect *) new Distortion(parameters);
    }
    else if (name == "Flanger") {
      pEffect = (Effect *) new Flanger(parameters);
    }
    else if (name == "Reverb") {
      pEffect = (Effect *) new Reverb(parameters);
    }
    else if (name == "Wah") {
      pEffect = (Effect *) new Wah(parameters);
    }
    else if (name == "Chorus") {
      pEffect = (Effect *) new Chorus(parameters);
    }
    return pEffect;
  }
}
