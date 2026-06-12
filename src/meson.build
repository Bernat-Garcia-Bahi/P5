# Programas del proyecto

sources = [
	'instruments/instrument.cpp',
	'instruments/instrument_dumb.cpp',
	'instruments/instrument_wavetable.cpp', # síntesis por tabla de ondas periódica
	'instruments/instrument_sampler.cpp',   # sampler de disparo único con transposición de tono
	'instruments/instrument_vibrato.cpp',   # vibrato nativo en el instrumento
	'instruments/instrument_fm.cpp',        # síntesis FM de Chowning
	'instruments/seno.cpp',                 # sinusoide por tabla con pitch e interpolación lineal
	'synth/envelope_adsr.cpp',
	'synth/midi_score.cpp',
	'synth/multinote_instr.cpp',
	'synth/orchest.cpp',
	'synth/synthesizer.cpp',
	'effects/effect.cpp',
	'effects/tremolo.cpp',
	'effects/vibrato.cpp',
	'effects/distortion.cpp',
	'effects/flanger.cpp',
	'effects/reverb.cpp',
	'effects/wah.cpp',
	'effects/chorus.cpp',
]

executable(
    'synth',
    sources: sources,
    include_directories: inc,
    link_args: ['-lm', '-lsndfile'],
    link_with: lib_pav,
    install: true,
)
