#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[]) {
  int i = 10;
  int *ip;

  ip = &i;

  char endString[10];

  printf("Value of int i %d\n", i);
  printf("Value of memory address for int i in *ip %d\n", ip);
  printf("Value of int i stored in *ip %d\n", *ip);

  printf("Press Enter to End Program");
  scanf("%s", endString);

  exit(0);
  return 0;
}
