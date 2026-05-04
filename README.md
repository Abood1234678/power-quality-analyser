# Power Quality Waveform Analyser

UWE Programming for Engineers (UGMFGT-15-1) coursework.

A C99 program that reads 3-phase voltage data from a CSV log file, computes
key power quality metrics for each phase, and writes a formatted analysis
report to a text file.

## What the program does

For each of the three phases (A, B, C) it computes:

- **RMS voltage** - the effective AC voltage, equivalent DC delivering the same power
- **Peak-to-peak voltage** - largest sample minus smallest sample
- **DC offset** - arithmetic mean of all samples (should be ~0 V for clean AC)
- **Clipping detection** - count of samples where |V| >= 324.9 V (sensor saturation)
- **EN 50160 compliance check** - flags whether RMS is within the 207-253 V tolerance band

Results are printed to the terminal and written to results.txt.

## Project structure

- main.c - entry point and orchestration
- waveform.h / waveform.c - analysis functions and the WaveformSample struct
- io.h / io.c - file I/O (CSV reader and results writer)
- CMakeLists.txt - build configuration
- power_quality_log.csv - input dataset (1000 rows, 8 columns)
- results.txt - generated report
- logbook.docx - development logbook

## Build and run

The project was developed in CLion 2021.3.3 with the bundled MinGW GCC toolchain.
Open the Portfolio folder in CLion, allow it to auto-detect CMakeLists.txt,
and click the green Run button. The math library is linked automatically via
target_link_libraries(Portfolio m) because the analysis uses sqrt() and fabs().

## Expected output

For the supplied dataset:

- RMS ~229.81 V on all three phases (inside EN 50160 207-253 V band)
- Peak-to-peak ~650 V
- DC offset ~0.000000 V
- Clipped samples: 20 per phase
- Compliance: COMPLIANT

## Author

Abdelrahman Hamed
