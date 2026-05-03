#ifndef IO_H
#define IO_H

#include "waveform.h"

// Load CSV file into the samples array.
// Returns the number of samples read, or -1 on error.
int load_csv(const char *filename, WaveformSample *samples, int max_samples);

// Write the analysis report to a text file.
// Returns 0 on success, -1 if the file couldn't be opened.
int write_results(const char *filename,
                  double rms_A, double rms_B, double rms_C,
                  double pp_A, double pp_B, double pp_C,
                  double dc_A, double dc_B, double dc_C,
                  int clip_A, int clip_B, int clip_C,
                  int n);

#endif