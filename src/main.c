#include "utils.c"
#include <stdio.h>
#include "radix2r.c"
#include "ssp.c"

int main() {
  for (int i = 1; i < 1024; i++) {
    int bones = 0;
    char* binv = tobinaryl(i, bitlength(i), &bones);

    int sones = 0;
    char* sspv = ssp(i, 4, &sones);

    int rones = 0;
    char* radv = radix2r(i, 4, &rones);
    printf("Number (%d):\n - Bin: %s -> %d\n - SSP: %s -> %d \n - Radix-2r: %s -> %d\n\n", i, binv, bones, sspv, sones, radv, rones);

    free(binv);
    free(sspv);
    free(radv);
  }
}

void testbench(int start, int stop) {

}
