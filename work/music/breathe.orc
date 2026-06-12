# Orquestación de "Breathe (In the Air)" (Pink Floyd, The Dark Side of the Moon, 1973).
# Canal 1 = guitarra eléctrica lead  → FM, 2.º armónico, sostenido (Gilmour-esque)
# Canal 2 = guitarra eléctrica rítm. → FM, 3.er armónico, más brillante
# Canal 3 = guitarra eléctrica harm. → FM, 2.º armónico, armónicos altos
# Canal 4 = guitarra eléctrica esp.  → FM, igual que ch1 (notas esporádicas)
# Canal 5 = oboe                     → FM, 3.er armónico, ataque suave
# Canal 7 = bajo/piano (grave)       → FM profundo N2=1
# Canal 8 = bajo/piano (medio-grave) → FM N2=2
# Canal 9 = bajo/piano (percusivo)   → FM muy corto, N2=1
1	InstrumentFM	ADSR_A=0.03; ADSR_D=0.15; ADSR_S=0.70; ADSR_R=0.25; N1=1; N2=2; I=12;
2	InstrumentFM	ADSR_A=0.02; ADSR_D=0.12; ADSR_S=0.65; ADSR_R=0.20; N1=1; N2=3; I=10;
3	InstrumentFM	ADSR_A=0.03; ADSR_D=0.12; ADSR_S=0.60; ADSR_R=0.20; N1=1; N2=2; I=14;
4	InstrumentFM	ADSR_A=0.02; ADSR_D=0.15; ADSR_S=0.65; ADSR_R=0.25; N1=1; N2=2; I=10;
5	InstrumentFM	ADSR_A=0.05; ADSR_D=0.08; ADSR_S=0.75; ADSR_R=0.15; N1=1; N2=3; I=8;
7	InstrumentFM	ADSR_A=0.01; ADSR_D=0.30; ADSR_S=0.50; ADSR_R=0.15; N1=1; N2=1; I=10;
8	InstrumentFM	ADSR_A=0.01; ADSR_D=0.25; ADSR_S=0.45; ADSR_R=0.12; N1=1; N2=2; I=8;
9	InstrumentFM	ADSR_A=0.01; ADSR_D=0.10; ADSR_S=0.30; ADSR_R=0.08; N1=1; N2=1; I=6;
