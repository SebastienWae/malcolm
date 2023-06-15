#include <stdbool.h>
#include <string.h>

bool hextodec(char *src, long *dest, int len) {
  int cnt = 0, digit = 0;

  for (int i = len - 1; i >= 0; i--) {
    switch (src[i]) {
    case 'a':
    case 'A':
      digit = 10;
      break;
    case 'b':
    case 'B':
      digit = 11;
      break;
    case 'c':
    case 'C':
      digit = 12;
      break;
    case 'd':
    case 'D':
      digit = 13;
      break;
    case 'e':
    case 'E':
      digit = 14;
      break;
    case 'f':
    case 'F':
      digit = 15;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      digit = src[i] - '0';
      break;
    default:
      return false;
    }

    *dest += (digit << (cnt * 4));
    cnt++;
  }

  return true;
}
