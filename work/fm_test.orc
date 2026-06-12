# Test orchestration for InstrumentVibrato and InstrumentFM
# Channel 1: Vibrato instrument (gentle, ~6 Hz, narrow depth)
# Channel 2: FM with N1=N2=1, I=2 — classic brass-like tone
# Channel 3: FM with N1=1, N2=3, I=4 — bell-like, inharmonic timbre
# Channel 4: FM with N1=2, N2=1, I=1.5 — woody/hollow

1	InstrumentVibrato	ADSR_A=0.02; ADSR_D=0.1; ADSR_S=0.6; ADSR_R=0.2; fm=6; I=1.0;
2	InstrumentFM		ADSR_A=0.02; ADSR_D=0.1; ADSR_S=0.5; ADSR_R=0.2; I=2.0; N1=1; N2=1;
3	InstrumentFM		ADSR_A=0.005; ADSR_D=0.5; ADSR_S=0.0; ADSR_R=1.5; I=4.0; N1=1; N2=3;
4	InstrumentFM		ADSR_A=0.02; ADSR_D=0.1; ADSR_S=0.5; ADSR_R=0.2; I=1.5; N1=2; N2=1;
