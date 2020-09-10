#include <vector>
#define ARRAY_SIZE (512*1024*1024) // 512MB of data

using namespace std;

int main() {
  vector<unsigned> data;
  data.resize(ARRAY_SIZE / sizeof(unsigned), 1);
  for (size_t i = 0 ; i < ARRAY_SIZE / sizeof(unsigned) ; ++i) {
    data[i];
  }
  return 0;
}
