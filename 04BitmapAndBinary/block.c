#include <stdio.h>

int cprint(char buffer[], int buflen, int red, int green, int blue) {
  return snprintf(buffer, buflen, "\033[48;2;%d;%d;%dm \033[0m", red, green, blue);
}

int main(int argc, char** argv) {
  char buffer[100];
  cprint(buffer, 100, 255, 0, 0);
  printf(buffer);
  printf(buffer);
  return 0;
}
