#ifndef INSTRUMENT_WAVETABLE_H
#define INSTRUMENT_WAVETABLE_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "envelope_adsr.h"
#include "instrument.h"

namespace upc {
  // Síntesis por tabla de ondas periódica: lee exactamente un ciclo de forma
  // de onda de un fichero WAV y lo repite a la frecuencia objetivo usando
  // interpolación lineal.
  // Parámetros: file=<ruta>  ADSR_A/D/S/R (ver EnvelopeADSR)
  class InstrumentWavetable : public Instrument {
    EnvelopeADSR adsr;
    double phase, step; // índice de lectura fraccionario e incremento por muestra
    float A;            // amplitud escalada desde la velocidad MIDI
    std::shared_ptr<std::vector<float>> tbl; // compartida con las clones polifónicas

    // weak_ptr para que el buffer se libere automáticamente cuando ningún instrumento lo referencie
    static std::map<std::string, std::weak_ptr<std::vector<float>>> cache;
    static std::shared_ptr<std::vector<float>> load(const std::string &filename);

  public:
    InstrumentWavetable(const std::string &param = "");
    void command(long cmd, long note, long velocity = 1);
    const std::vector<float> &synthesize();
  };
}

#endif
