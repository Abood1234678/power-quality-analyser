// io.c — file input/output for the Power Quality Waveform Analyser.
// Implements load_csv() to read the 1000-row CSV into a WaveformSample
// array, and write_results() to write the analysis report to a text file.
// Function declarations are in io.h.

#include <stdio.h>
#include "io.h"
#include "waveform.h"

// Load samples from a CSV file into the array.
// Returns the number of samples read, or -1 on error.
int load_csv(const char *filename, WaveformSample *samples, int max_samples) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    // Skip the header line
    char header_line[256];
    fgets(header_line, sizeof(header_line), file);

    // Read each data row into the samples array
    int count = 0;
    while (count < max_samples) {
        int items_read = fscanf(file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                                &samples[count].timestamp,
                                &samples[count].phase_A_voltage,
                                &samples[count].phase_B_voltage,
                                &samples[count].phase_C_voltage,
                                &samples[count].line_current,
                                &samples[count].frequency,
                                &samples[count].power_factor,
                                &samples[count].thd_percent);
        if (items_read != 8) {
            break;
        }
        count++;
    }

    fclose(file);
    return count;
}

// Write the full analysis report to a text file.
int write_results(const char *filename,
                  double rms_A, double rms_B, double rms_C,
                  double pp_A, double pp_B, double pp_C,
                  double dc_A, double dc_B, double dc_C,
                  int clip_A, int clip_B, int clip_C,
                  int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: could not open '%s' for writing.\n", filename);
        return -1;
    }

    fprintf(file, "Power Quality Waveform Analyser - Results\n");
    fprintf(file, "==========================================\n");
    fprintf(file, "Samples analysed: %d\n", n);
    fprintf(file, "\n");

    fprintf(file, "RMS voltages (V):\n");
    fprintf(file, "  Phase A: %.4f\n", rms_A);
    fprintf(file, "  Phase B: %.4f\n", rms_B);
    fprintf(file, "  Phase C: %.4f\n", rms_C);
    fprintf(file, "\n");

    fprintf(file, "Peak-to-peak voltages (V):\n");
    fprintf(file, "  Phase A: %.4f\n", pp_A);
    fprintf(file, "  Phase B: %.4f\n", pp_B);
    fprintf(file, "  Phase C: %.4f\n", pp_C);
    fprintf(file, "\n");

    fprintf(file, "DC offsets (V):\n");
    fprintf(file, "  Phase A: %.6f\n", dc_A);
    fprintf(file, "  Phase B: %.6f\n", dc_B);
    fprintf(file, "  Phase C: %.6f\n", dc_C);
    fprintf(file, "\n");

    fprintf(file, "Clipped samples (|V| >= 324.9 V):\n");
    fprintf(file, "  Phase A: %d\n", clip_A);
    fprintf(file, "  Phase B: %d\n", clip_B);
    fprintf(file, "  Phase C: %d\n", clip_C);
    fprintf(file, "  Total:   %d\n", clip_A + clip_B + clip_C);
    fprintf(file, "\n");

    fprintf(file, "EN 50160 compliance (207 V <= V_RMS <= 253 V):\n");
    fprintf(file, "  Phase A: %s\n", check_compliance(rms_A) ? "COMPLIANT" : "NON-COMPLIANT");
    fprintf(file, "  Phase B: %s\n", check_compliance(rms_B) ? "COMPLIANT" : "NON-COMPLIANT");
    fprintf(file, "  Phase C: %s\n", check_compliance(rms_C) ? "COMPLIANT" : "NON-COMPLIANT");

    fclose(file);
    return 0;
}