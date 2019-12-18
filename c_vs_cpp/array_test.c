#include <stdlib.h>

#define ARRAY_SIZE (512*1024*1024) // 512MB of data

int main() {
  size_t i;
  unsigned* data = malloc(ARRAY_SIZE);
  for (i = 0 ; i < (ARRAY_SIZE / sizeof(unsigned)) ; ++i) {
    data[i] = 1;
  }
  for (i = 0 ; i < (ARRAY_SIZE / sizeof(unsigned)) ; ++i) {
    data[i];
  }
  free(data);
  return 0;
}
