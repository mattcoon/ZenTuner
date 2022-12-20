// Copyright AudioKit. All Rights Reserved. Revision History at http://github.com/AudioKit/AudioKit/

#ifndef ZENTUNER_H
#define ZENTUNER_H

#include <stdint.h>
#include <stdio.h>

#ifndef ZTFLOAT
#define ZTFLOAT float
#endif

typedef struct {
    size_t size;
    void *ptr;
} zt_auxdata;

typedef struct {
    float *out;
    int sr;
    unsigned long len;
    unsigned long pos;
} zt_data;

typedef struct {
    ZTFLOAT *utbl;
    int16_t *BRLow;
    int16_t *BRLowCpx;
} zt_fft;

void zt_fft_create(zt_fft **fft);
void zt_fft_init(zt_fft *fft, int M);
void zt_fft_cpx(zt_fft *fft, ZTFLOAT *buf, int FFTsize);
void zt_fft_destroy(zt_fft *fft);

typedef struct {
    ZTFLOAT freq, amp;
    ZTFLOAT size;
    zt_auxdata signal, prev, sin, spec1, spec2, peakarray;
    int numpks;
    int cnt;
    int histcnt;
    int hopsize;
    ZTFLOAT sr;
    ZTFLOAT cps;
    ZTFLOAT dbs[20];
    ZTFLOAT amplo;
    ZTFLOAT npartial;
    ZTFLOAT dbfs;
    zt_fft fft;
} zt_ptrack;

void zt_ptrack_init(zt_data *sp, zt_ptrack *p, int ihopsize, int ipeaks, float pi);
void ptrack(zt_data *sp, zt_ptrack *p);

#endif
