#include <iostream>
#include "Map.h"

using namespace std;

int main() {
  auto moonMap = MapLoader::importMap("../assets/Moon.map");

  cout << *moonMap << endl;

  try {
    MapLoader::importMap("../assets/MoonMalformed1.map");
  } catch (runtime_error &e) {
    cout << "Error for malformed 1 caught! " << e.what() << endl;
  }

  try {
    MapLoader::importMap("../assets/MoonMalformed2.map");
  } catch (runtime_error &e) {
    cout << "Error for malformed 2 caught! " << e.what() << endl;
  }

  return 0;
}
