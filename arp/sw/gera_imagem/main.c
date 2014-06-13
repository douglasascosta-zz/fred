#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int dx = atoi(argv[1]);
  int dy = atoi(argv[2]);

  printf("P2\n");
  printf("%d %d\n", dx, dy);
  printf("256\n");

  int i,j;
  for (j = 0; j < dy; j++) {
    int val = (int)((((double) j) / ((double)dy))*256.0);
    for (i = 0; i < dx-1; i++)
      printf("%d ", val);
    printf("%d\n", val);
  }
  return 0;
}
