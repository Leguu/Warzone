#include <iostream>
#include <filesystem>
#include "Map.h"

using namespace std;

int main() {
  for (const auto &file : std::filesystem::directory_iterator{"../assets/"})  //loop through the current folder
  {
    try {
      MapLoader::importMap(file.path().string());
      cout << "Map file '" << file.path().string() << "' completely valid!" << endl;
    } catch (runtime_error &e) {
      cout << "Error caught for file '" + file.path().string() << "': " << e.what() << endl;
    }
  }

  return 0;
}
