/* See COPYING file for copyright and license details. */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "wav.h"
#include "signal_classifier.h"

size_t wav_read_f(WavFile* self, void *buffer, size_t count) {
  size_t readframes;
  int i, j;
  int nch = wav_get_num_channels(self);
  float factor = 1.0 / (32768);
  short *pread = (short *)wav_get_buf(self);
  float *pbuf = (float*)buffer;
  readframes = wav_read(self, pread, count);

  for (i = 0; i < readframes; i++) {
    for (j = 0; j < nch; j++) {
      pbuf[i * nch + j] = pread[i * nch + j] * factor;
    }
  }

  return readframes;
}

size_t wav_write_f(WavFile* self, void *buffer, size_t count) {
  int i, j;
  int nch = wav_get_num_channels(self);
  short *pwrite = (short *)wav_get_buf(self);
  float factor = 32768.0;
  float tmp;  
  float *pbuf = (float*)buffer;

  for (i = 0; i < count; i++) {
    for (j = 0; j < nch; j++) {
      tmp = pbuf[i * nch + j] * factor;
      pwrite[i * nch + j] = tmp > 32767 ? (short)32767:tmp < -32768?(short)-32768:(short)tmp;
    }
  }

  return wav_write(self, pwrite, count);
}


int main(int ac, const char* av[]) {
  WavFile *infile;
  WavFile *outfile;
  FILE* indfile;
  int nr_frames_read;

  float* buffer;
  float* outbuffer;
  int i;
  clock_t begin;
  clock_t end;
  double secs;
  double inLen;
  WavU32 samplerate;
  WavU16 channels, bits;

  size_t num_bytes;  
  CLASSIFICATION cls_st;  

  int cls_out;

  

  if (ac < 4) {
    fprintf(stderr, "usage: %s in.wav sad_index.txt combine.wav\n", av[0]);
    exit(1);
  }

  infile = wav_open(av[1], "rb");
  if (!infile) {
    fprintf(stderr, "Could not open file %s !\n", av[1]);
    return 1;
  }
  samplerate = wav_get_sample_rate(infile);
  channels = wav_get_num_channels(infile);
  bits = wav_get_valid_bits_per_sample(infile);
  printf("samplerate: %d\n", samplerate);
  printf("channels: %d\n", channels);
  printf("bits: %d\n", bits);

  if (samplerate != 48000 || channels != 1 || bits != 16) {
    printf("wav format is not correct (48000, mono, 16)!\n");
    return 1;
  }


  indfile = fopen(av[2], "wt");
  if (!indfile) {
    fprintf(stderr, "Could not open index file %s!\n", av[2]);
    return 1;
  }

  outfile = wav_open(av[3], "wb");
  if (!outfile) {
    fprintf(stderr, "Could not open file %s !\n", av[3]);
    return 1;
  }  

  buffer = (float*) malloc(FRAMELEN * 2 * sizeof(float));
  outbuffer = (float*)malloc(FRAMELEN * 2 * sizeof(float));
  if (!buffer || !outbuffer) {
    fprintf(stderr, "ERROR: Memory allocation failed.\n");
    return 1;
  }

    /* Allocate memory for CLASSIFICATION struct */
  init_classification(&cls_st);

  begin = clock();
  inLen = 0;
  while ((nr_frames_read = wav_read_f(infile, buffer, FRAMELEN))) { 
    for (i = 0; i < FRAMELEN; i++) {
      cls_st.input_samples[i] = buffer[i];
    }
    classification(&cls_st);

    cls_out = 1 - cls_st.coding_mode;    
    fprintf(indfile, "%d\n", cls_out);

    for (i = 0; i < nr_frames_read; i++) {
      outbuffer[2 * i] = buffer[i];
      outbuffer[2 * i + 1] = cls_out * 0.5;
    }

    wav_write_f(outfile, outbuffer, nr_frames_read);    
    inLen += nr_frames_read;
  }

  end = clock();
  secs = (double)(end - begin) / CLOCKS_PER_SEC + 0.0001;
  inLen = inLen / wav_get_sample_rate(infile);
  printf("----------------------------------------\n");
  printf("frames: %f\n", inLen);
  printf("time: %f\n", secs);
  printf("Estimated speed is %.1f x real-time\n", inLen/secs);



  /* clean up */ 

  free(buffer);
  buffer = NULL;

  if (infile) {
    wav_close(infile);
  }

  if (outfile) {
    wav_close(outfile);
  }

  if (indfile) {
    fclose(indfile);
  }

  
  
  return 0;
}
