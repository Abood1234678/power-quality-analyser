// main.c — entry point for the Power Quality Waveform Analyser.
// Allocates memory for 1000 WaveformSample structs, loads the CSV via
// load_csv(), calls each analysis function from waveform.c, prints the
// results to the terminal, writes a report to results.txt via write_results(),
// and frees memory before exit.

#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

#define NUM_SAMPLES 1000

int main() {
    // Allocate memory for 1000 samples on the heap
    WaveformSample *samples = malloc(NUM_SAMPLES * sizeof(WaveformSample));
    if (samples == NULL) {
        printf("Error: failed to allocate memory.\n");
        return 1;
    }

    // Load the CSV
    int n = load_csv("../power_quality_log.csv", samples, NUM_SAMPLES);
    if (n < 0) {
        printf("Failed to load CSV file.\n");
        free(samples);
        return 1;
    }

    printf("Successfully loaded %d samples from CSV.\n\n", n);

    // Print the first 3 rows to confirm the data loaded correctly
    printf("First 3 rows:\n");
    for (int i = 0; i < 3; i++) {
        printf("Sample %d: t=%.4f, A=%.2f, B=%.2f, C=%.2f\n",
               i,
               samples[i].timestamp,
               samples[i].phase_A_voltage,
               samples[i].phase_B_voltage,
               samples[i].phase_C_voltage);
    }

    // Compute RMS for each phase
    double rms_A = compute_rms_phase_A(samples, n);
    double rms_B = compute_rms_phase_B(samples, n);
    double rms_C = compute_rms_phase_C(samples, n);
    printf("\nRMS voltages:\n");
    printf("  Phase A: %.4f V\n", rms_A);
    printf("  Phase B: %.4f V\n", rms_B);
    printf("  Phase C: %.4f V\n", rms_C);

    // Compute peak-to-peak for each phase
    double pp_A = compute_peak_to_peak_phase_A(samples, n);
    double pp_B = compute_peak_to_peak_phase_B(samples, n);
    double pp_C = compute_peak_to_peak_phase_C(samples, n);
    printf("\nPeak-to-peak voltages:\n");
    printf("  Phase A: %.4f V\n", pp_A);
    printf("  Phase B: %.4f V\n", pp_B);
    printf("  Phase C: %.4f V\n", pp_C);

    // Compute DC offset for each phase
    double dc_A = compute_dc_offset_phase_A(samples, n);
    double dc_B = compute_dc_offset_phase_B(samples, n);
    double dc_C = compute_dc_offset_phase_C(samples, n);
    printf("\nDC offsets:\n");
    printf("  Phase A: %.6f V\n", dc_A);
    printf("  Phase B: %.6f V\n", dc_B);
    printf("  Phase C: %.6f V\n", dc_C);

    // Count clipped samples for each phase
    int clip_A = count_clipped_phase_A(samples, n);
    int clip_B = count_clipped_phase_B(samples, n);
    int clip_C = count_clipped_phase_C(samples, n);
    printf("\nClipped samples (|V| >= 324.9):\n");
    printf("  Phase A: %d\n", clip_A);
    printf("  Phase B: %d\n", clip_B);
    printf("  Phase C: %d\n", clip_C);
    printf("  Total:   %d\n", clip_A + clip_B + clip_C);

    // Check EN 50160 compliance
    printf("\nEN 50160 compliance (207 V <= V_RMS <= 253 V):\n");
    printf("  Phase A: %s\n", check_compliance(rms_A) ? "COMPLIANT" : "NON-COMPLIANT");
    printf("  Phase B: %s\n", check_compliance(rms_B) ? "COMPLIANT" : "NON-COMPLIANT");
    printf("  Phase C: %s\n", check_compliance(rms_C) ? "COMPLIANT" : "NON-COMPLIANT");

    // Write the full report to results.txt
    int write_status = write_results("../results.txt",
                                     rms_A, rms_B, rms_C,
                                     pp_A, pp_B, pp_C,
                                     dc_A, dc_B, dc_C,
                                     clip_A, clip_B, clip_C,
                                     n);
    if (write_status == 0) {
        printf("\nResults written to results.txt\n");
    }

    // Free memory before exit
    free(samples);
    return 0;
}