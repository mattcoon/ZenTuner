/*
 * PTrack
 *
 * This code has been extracted from the Csound opcode "ptrack".
 * It has been modified to work as a Soundpipe module and modified again for use in ZenTuner.
 *
 * Original Author(s): Victor Lazzarini, Miller Puckette (Original Algorithm), Aurelius Prochazka
 * Year: 2007
 * Location: Opcodes/pitchtrack.c
 *
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "CMicrophonePitchDetector.h"

#define MINBIN 3
#define THRSH 10

void ptrack_pt2(int *npeak, int numpks, PEAK *peaklist, float totalpower, float *spec, int n)
{
    for (int i = 4*MINBIN; i < (4*(n-2)) && *npeak < numpks; i += 4) {
        float height = spec[i+2];
        float h1 = spec[i-2];
        float h2 = spec[i+6];
        float totalfreq, peakfr, tmpfr1, tmpfr2, m, v, stdev;

        if (height < h1 || height < h2 ||
            h1 < 0.00001*totalpower ||
            h2 < 0.00001*totalpower)
        {
            continue;
        }

        peakfr = ((spec[i-8] - spec[i+8]) * (2.0 * spec[i] - spec[i+8] - spec[i-8]) +
                  (spec[i-7] - spec[i+9]) * (2.0 * spec[i+1] - spec[i+9] - spec[i-7])) / (height + height);
        tmpfr1 =  ((spec[i-12] - spec[i+4]) * (2.0 * spec[i-4] - spec[i+4] - spec[i-12]) +
                   (spec[i-11] - spec[i+5]) * (2.0 * spec[i-3] - spec[i+5] - spec[i-11])) / (2.0 * h1) - 1;
        tmpfr2 = ((spec[i-4] - spec[i+12]) * (2.0 * spec[i+4] - spec[i+12] - spec[i-4]) +
                  (spec[i-3] - spec[i+13]) * (2.0 * spec[i+5] - spec[i+13] - spec[i-3])) / (2.0 * h2) + 1;

        m = 0.333333333333 * (peakfr + tmpfr1 + tmpfr2);
        v = 0.5 * ((peakfr-m)*(peakfr-m) +
                   (tmpfr1-m)*(tmpfr1-m) + (tmpfr2-m)*(tmpfr2-m));

        totalfreq = (i >> 2) + m;
        if (v * totalpower > THRSH * height ||
            v < 1.0e-30)
        {
            continue;
        }

        stdev = (float)sqrt((float)v);
        if (totalfreq < 4) {
            totalfreq = 4;
        }

        peaklist[*npeak].pwidth = stdev;
        peaklist[*npeak].ppow = height;
        peaklist[*npeak].ploudness = sqrt(sqrt(height));
        peaklist[*npeak].pfreq = totalfreq;
        (*npeak)++;
    }
}
