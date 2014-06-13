#include <stdio.h>
#include <stdlib.h>

#define WAKE_ADDR 0x60000000
#define WAKE (*((int *)WAKE_ADDR))
#define GET_LOCK_ADDR 0x60010000
#define GET_LOCK (*((int *)GET_LOCK_ADDR))
#define FREE_LOCK_ADDR 0x60020000
#define FREE_LOCK (*((int *)FREE_LOCK_ADDR))
#define OFFLOAD_ADDR 0x60030000
#define OFFLOAD (*((int *)OFFLOAD_ADDR))

#define NUM_CORES 2
#define PRINT_LOCK 0
#define OFFLOAD_LOCK 1

static int dx, dy, n, v;
static int *val;

int getNextNum( FILE *fp )
{
  char buf[32] = {0};
  char c;
  int i = 0;
  while (1) {
    c = fgetc(fp);
    if (!isdigit(c)) {
      if (i == 0)
        continue;
      else
        break;
    }
    buf[i++] = c;
  }
  buf[i] = '\0';
  return atoi(buf);
}

int main(int argc, char *argv[]){
  int core = WAKE;
  GET_LOCK = (PRINT_LOCK << 16) | core << 24;
  printf("Core %d running!\n", core);
  FREE_LOCK = (PRINT_LOCK << 16) | core << 24;

  // Core 0 carrega imagem
  if (core == 0) {
    FILE *fp = NULL;
    char type[10];

    fp = fopen(argv[1], "r");
    fgetc(fp); fgetc(fp); fgetc(fp); // "P2\n"
    dx = getNextNum(fp);
    dy = getNextNum(fp);
    n = dx * dy;
    val = (int *) malloc(n * sizeof *val);
    v = getNextNum(fp);

    int i;
    for (i = 0; i < n; i++)
      val[i] = getNextNum(fp);

    fclose(fp);
  }
  
  // Barreira de sincronizacao
  WAKE = core << 24;

  // Cada core inverte um segmento da imagem
  int start, end;
  start = core * (n / NUM_CORES);
  if (core == NUM_CORES - 1)
    end = n;
  else
    end = (core+1) * (n / NUM_CORES);
  int p;
  for (p = start; p < end; p++) {

    GET_LOCK = (OFFLOAD_LOCK << 16) | core << 24;
    OFFLOAD = ( v << 24 ) | ( val[p] << 16 );
    val[p] = OFFLOAD;
    FREE_LOCK = (OFFLOAD_LOCK << 16) | core << 24;
  }
  
  // Barreira de sincronizacao
  WAKE = core << 24;

  // Core 0 salva imagem resultante
  if (core == 0) {
    FILE *fp = NULL;
    int x,y;
    
    fp = fopen(argv[2], "w");
    fprintf(fp, "P2\n");
    fprintf(fp,"%d %d\n", dx,dy);
    fprintf(fp,"%d\n", v);
    for (y = 0; y < dy; y++) {
      for(x = 0; x < dx-1; x++) {
        fprintf(fp,"%03d ", val[y*dx+x]);
      }
      fprintf(fp, "%03d\n", val[y*dx+x]);
    }

    fclose(fp);
  }

  // Barreira de sincronizacao
  WAKE = core << 24;
  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

