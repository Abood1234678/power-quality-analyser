// waveform.c — analysis functions for the Power Quality Waveform Analyser.
// Implements RMS, peak-to-peak, DC offset, clipping detection, and
// EN 50160 compliance check for each of the three phases (A, B, C).
// Function declarations are in waveform.h.

#include <math.h>
#include "waveform.h"

// RMS voltage for each phase.
// RMS = sqrt(mean of squared samples).
double compute_rms_phase_A(WaveformSample *samples, int n) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq += samples[i].phase_A_voltage * samples[i].phase_A_voltage;
    }
    return sqrt(sum_sq / n);
}

double compute_rms_phase_B(WaveformSample *samples, int n) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq += samples[i].phase_B_voltage * samples[i].phase_B_voltage;
    }
    return sqrt(sum_sq / n);
}

double compute_rms_phase_C(WaveformSample *samples, int n) {
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq += samples[i].phase_C_voltage * samples[i].phase_C_voltage;
    }
    return sqrt(sum_sq / n);
}

// Peak-to-peak voltage for each phase.
double compute_peak_to_peak_phase_A(WaveformSample *samples, int n) {
    double v_max = samples[0].phase_A_voltage;
    double v_min = samples[0].phase_A_voltage;
    for (int i = 1; i < n; i++) {
        double v = samples[i].phase_A_voltage;
        if (v > v_max) v_max = v;
        if (v < v_min) v_min = v;
    }
    return v_max - v_min;
}

double compute_peak_to_peak_phase_B(WaveformSample *samples, int n) {
    double v_max = samples[0].phase_B_voltage;
    double v_min = samples[0].phase_B_voltage;
    for (int i = 1; i < n; i++) {
        double v = samples[i].phase_B_voltage;
        if (v > v_max) v_max = v;
        if (v < v_min) v_min = v;
    }
    return v_max - v_min;
}

double compute_peak_to_peak_phase_C(WaveformSample *samples, int n) {
    double v_max = samples[0].phase_C_voltage;
    double v_min = samples[0].phase_C_voltage;
    for (int i = 1; i < n; i++) {
        double v = samples[i].phase_C_voltage;
        if (v > v_max) v_max = v;
        if (v < v_min) v_min = v;
    }
    return v_max - v_min;
}

// DC offset for each phase (arithmetic mean of all samples).
double compute_dc_offset_phase_A(WaveformSample *samples, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += samples[i].phase_A_voltage;
    }
    return sum / n;
}

double compute_dc_offset_phase_B(WaveformSample *samples, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += samples[i].phase_B_voltage;
    }
    return sum / n;
}

double compute_dc_offset_phase_C(WaveformSample *samples, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += samples[i].phase_C_voltage;
    }
    return sum / n;
}

// Count the number of clipped samples per phase (|V| >= 324.9 V).
int count_clipped_phase_A(WaveformSample *samples, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(samples[i].phase_A_voltage) >= 324.9) {
            count++;
        }
    }
    return count;
}

int count_clipped_phase_B(WaveformSample *samples, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(samples[i].phase_B_voltage) >= 324.9) {
            count++;
        }
    }
    return count;
}

int count_clipped_phase_C(WaveformSample *samples, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(samples[i].phase_C_voltage) >= 324.9) {
            count++;
        }
    }
    return count;
}

// Check EN 50160 compliance: 207 V <= rms <= 253 V.
int check_compliance(double rms) {
    if (rms >= 207.0 && rms <= 253.0) {
        return 1;
    } else {
        return 0;
    }
}