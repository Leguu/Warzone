#include <iostream>
#include <filesystem>
#include "Map.h"

using std::cout;
using std::runtime_error;
using std::endl;
using std::filesystem::directory_iterator;

int main() {
  for (const auto &file : directory_iterator{"../assets/"}) {
    auto path = file.path().string();
    try {
      MapLoader::importMap(path);
      cout << "Map file '" << path << "' completely valid!" << endl;
    } catch (runtime_error &e) {
      cout << "Error caught for file '" + path << "': " << e.what() << endl;
    }
  }

  auto moon = MapLoader::importMap("../assets/Moon.map");

  cout << *moon << endl;
}
