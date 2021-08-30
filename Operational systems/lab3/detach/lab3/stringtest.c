#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char* argv[]) {
  int i, j, k;
  i = 6;
  j = 2*i + 1;
  int row;
  char buf[20];
  char verify[13] = "Vertex matrix";
  memset(buf, '0', 20);
  FILE* fd = fopen(argv[1], "r");
  if (fd == NULL)
    perror("open file");
 
  while(strncmp(buf, verify, 12) != 0) {
    fgets(buf, j, fd);
  }
  while(1) {
    printf("enter row \n");
    scanf("%d", &row);
    if (row == 0)
      break;
    for(k = 1; k < row; k++)
      fgets(buf, j, fd);
    printf("%s \n", buf);
  }
  return 0;
}
