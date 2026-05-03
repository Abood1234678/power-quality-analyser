#ifndef WAVEFORM_H
#define WAVEFORM_H

// The WaveformSample struct — one row of the CSV file.
typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;

// RMS voltage for each phase
double compute_rms_phase_A(WaveformSample *samples, int n);
double compute_rms_phase_B(WaveformSample *samples, int n);
double compute_rms_phase_C(WaveformSample *samples, int n);

// Peak-to-peak voltage for each phase
double compute_peak_to_peak_phase_A(WaveformSample *samples, int n);
double compute_peak_to_peak_phase_B(WaveformSample *samples, int n);
double compute_peak_to_peak_phase_C(WaveformSample *samples, int n);

// DC offset (mean) for each phase
double compute_dc_offset_phase_A(WaveformSample *samples, int n);
double compute_dc_offset_phase_B(WaveformSample *samples, int n);
double compute_dc_offset_phase_C(WaveformSample *samples, int n);

// Count clipped samples (|V| >= 324.9 V) for each phase
int count_clipped_phase_A(WaveformSample *samples, int n);
int count_clipped_phase_B(WaveformSample *samples, int n);
int count_clipped_phase_C(WaveformSample *samples, int n);

// Check whether an RMS voltage is within EN 50160 tolerance band.
// Returns 1 if compliant, 0 if not.
int check_compliance(double rms);

#endif