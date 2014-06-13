#include <stdio.h>

#define WAKE_ADDR 0x60000000
#define WAKE (*((int *)WAKE_ADDR))
#define GET_LOCK_ADDR 0x60010000
#define GET_LOCK (*((int *)GET_LOCK_ADDR))
#define FREE_LOCK_ADDR 0x60020000
#define FREE_LOCK (*((int *)FREE_LOCK_ADDR))

int main(int argc, char *argv[]){
  int core = WAKE;
  int print_lock = 0;
  int i;
  GET_LOCK = (print_lock << 16) | core << 24;
  for(i=0;i<10;i++)
    printf("Hi from processor %d MIPS!\n", core);
  FREE_LOCK = (print_lock << 16) | core << 24;

  WAKE = core << 24;
  if (core == 0)
    printf("e.g. printando output\n");
  WAKE = core << 24;
  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

