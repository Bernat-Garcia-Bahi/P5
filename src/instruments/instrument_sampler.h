#ifndef INSTRUMENT_SAMPLER_H
#define INSTRUMENT_SAMPLER_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "envelope_adsr.h"
#include "instrument.h"

namespace upc {
  // Sampler de disparo único: reproduce un fichero WAV externo a una altura arbitraria.
  // La reproducción termina cuando se agota la muestra o finaliza la liberación ADSR,
  // lo que ocurra primero.
  // Parámetros: file=<ruta>  ft=<frecuencia de muestreo original>  ADSR_A/D/S/R
  class InstrumentSampler : public Instrument {
    EnvelopeADSR adsr;
    double phase, step; // índice de lectura fraccionario e incremento por muestra
    float A;            // amplitud escalada desde la velocidad MIDI
    float ft;           // frecuencia de grabación original del fichero cargado
    std::shared_ptr<std::vector<float>> tbl; // compartida con las clones polifónicas

    // weak_ptr para que el buffer se libere automáticamente cuando ningún instrumento lo referencie
    static std::map<std::string, std::weak_ptr<std::vector<float>>> cache;
    static std::shared_ptr<std::vector<float>> load(const std::string &filename);

  public:
    InstrumentSampler(const std::string &param = "");
    void command(long cmd, long note, long velocity = 1);
    const std::vector<float> &synthesize();
  };
}

#endif
