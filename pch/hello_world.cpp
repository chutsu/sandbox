#include "hello_world.hpp"

int main() {
  std::map<int, int> map;
  map[0] = 0;
  map[1] = 1;
  map[2] = 2;
  map[3] = 3;
  map[4] = 4;

  std::cout << map[0] << std::endl;
  std::cout << map[1] << std::endl;
  std::cout << map[2] << std::endl;
  std::cout << map[3] << std::endl;
  std::cout << map[4] << std::endl;

  std::cout << "Hello World" << std::endl;
  return 0;
}
