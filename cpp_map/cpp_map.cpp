#include <iostream>
#include <string>
#include <map>

int main() {
  std::map<int, std::string> word_map;
  word_map.insert({1, "A"});
  word_map.insert({2, "B"});
  word_map.insert({3, "C"});

  auto rit = word_map.rbegin();
  while (rit != word_map.rend()) {
    std::cout << rit->first << ", " << rit->second << std::endl;
    rit++;
  }
}
