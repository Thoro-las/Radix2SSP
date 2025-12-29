#include <stdio.h>

#include "radix2r.c"
#include "ssp.c"
#include "utils.c"

void testbench() {
  for (int i = 0; i <= (1 << 10); i++) {
    int bones = 0;
    int blen = bitlength(i);
    char *binv = tobinaryl(i, bitlength(i), &bones);

    int sones = 0;
    char *sspv = ssp(i, 4, &sones);

    int rones = 0;
    char *radv = radix2r(i, 4, &rones);

    printf("Number (%d):", i);
    // printf(" - Bin (%3d): %s\n", bones, binv);
    // printf(" - SSP (%3d): %s\n", sones, sspv);
    printf(" - R2r (%3d): %s\n", rones, radv);
    printf("\n");

    free(binv);
    free(sspv);
    free(radv);
  }
}

void calculate_averages() {
  long int ronesw3[2];
  long int sonesw3[2];

  long int ronesw4[5];
  long int sonesw4[5];

  long int rones = 0;
  long int sones = 0;

  for (uint64_t i = 0; i <= (1u << 12); i++) {
    int crones = 0;
    int csones = 0;

    free(radix2r(i, 3, &crones));
    free(ssp(i, 3, &csones));

    rones += crones;
    sones += csones;

    if (i == (1u << 8)) {
      ronesw3[0] = rones;
      sonesw3[0] = sones;

      printf("EA R2r: n = 8  | w = 3 : %f\n", (double)ronesw3[0] / (1u << 8));
      printf("EA SSP: n = 8  | w = 3 : %f\n", (double)sonesw3[0] / (1u << 8));
    }

    if (i == (1u << 12)) {
      ronesw3[1] = rones;
      sonesw3[1] = sones;

      printf("EA R2r: n = 12 | w = 3 : %f\n", (double)ronesw3[1] / (1u << 12));
      printf("EA SSP: n = 12 | w = 3 : %f\n", (double)sonesw3[1] / (1u << 12));
    }
  }

  for (uint64_t i = 0; i <= INT32_MAX; i++) {
    if (i % (UINT32_MAX / 10000) == 0) {
      double percent = (double)i * 100.0 / (double)UINT32_MAX;
      printf("progress: %s, %.2f%%\n", tobinary(i, NULL), percent);
    }

    int crones = 0;
    int csones = 0;

    free(radix2r(i, 3, &crones));
    free(ssp(i, 3, &csones));

    rones += crones;
    sones += csones;

    if (i == (1u << 16)) {
      ronesw4[0] = rones;
      sonesw4[0] = sones;
      printf("EA R2r: n = 16 | w = 4 : %f\n", (double)ronesw4[0] / (1u << 16));
      printf("EA SSP: n = 16 | w = 4 : %f\n", (double)sonesw4[0] / (1u << 16));
    }

    if (i == (1u << 20)) {
      ronesw4[1] = rones;
      sonesw4[1] = sones;
      printf("EA R2r: n = 20 | w = 4 : %f\n", (double)ronesw4[1] / (1u << 20));
      printf("EA SSP: n = 20 | w = 4 : %f\n", (double)sonesw4[1] / (1u << 20));
    }

    if (i == (1u << 24)) {
      ronesw4[2] = rones;
      sonesw4[2] = sones;
      printf("EA R2r: n = 24 | w = 4 : %f\n", (double)ronesw4[2] / (1u << 24));
      printf("EA SSP: n = 24 | w = 4 : %f\n", (double)sonesw4[2] / (1u << 24));
    }

    if (i == (1u << 28)) {
      ronesw4[3] = rones;
      sonesw4[3] = sones;
      printf("EA R2r: n = 28 | w = 4 : %f\n", (double)ronesw4[3] / (1u << 28));
      printf("EA SSP: n = 28 | w = 4 : %f\n", (double)sonesw4[3] / (1u << 28));
    }

    if (i == INT32_MAX) {
      ronesw4[4] = rones;
      sonesw4[4] = sones;
      printf("EA R2r: n = 32 | w = 4 : %f\n", (double)ronesw4[4] / (INT32_MAX));
      printf("EA SSP: n = 32 | w = 4 : %f\n", (double)sonesw4[4] / (INT32_MAX));
    }
  }
}

void calculate_qadditions() {
  int rq = 1;
  int sq = 1;

  for (int i = 1; i <= (1 << 28); i++) {
    int crones = 0;
    int csones = 0;

    free(radix2r(i, 4, &crones));
    free(ssp(i, 3, &csones));

    if (crones > rq) {
      rq = crones;
      printf("R2r: q: %d - n: %d\n", rq - 1, i);
    }

    if (csones > sq) {
      sq = csones;
      printf("SSP: q: %d - n: %d\n", sq - 1, i);
    }
  }
}

int main() {
  // testbench();
 // calculate_averages();
  calculate_qadditions();
}
