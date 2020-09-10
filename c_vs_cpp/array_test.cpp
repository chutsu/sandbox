#include <iostream>

#define ARRAY_SIZE (512*1024*1024) // 512MB of data

int main() {
	unsigned* data = new unsigned[ARRAY_SIZE / sizeof(unsigned)];
	for (size_t i = 0 ; i < ARRAY_SIZE / sizeof(unsigned) ; ++i) {
		data[i] = 1;
	}
	for (size_t i = 0 ; i < ARRAY_SIZE / sizeof(unsigned) ; ++i) {
		data[i];
	}
	delete [] data;
  return 0;
}
