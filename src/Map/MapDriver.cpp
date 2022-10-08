//
// Created by danie on 2022-10-08.
//

#include "MapDriver.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "Map.h"
#include "../Player/Player.h"

int main(){
    MapDriver::testLoadMaps();
}

void MapDriver::testLoadMaps() {
    for (const auto &file : directory_iterator{"../assets/"}) {
        auto path = file.path().string();
        try {
            MapLoader::importMap(path);
            Utils::assertCondition(!path.contains("Invalid"), "map is valid");
        } catch (runtime_error &e) {
            Utils::assertCondition(path.contains("Invalid"), path + " map is invalid");
            cout << "Map '" << path << "' was invalid because " << e.what() << endl;
        }
    }

    cout << "All tests passed" << endl;
}
