#include <iostream>
#include "Map/Map.h"

using namespace std;

int main() {

    string mapName = "Moon";
    vector<vector<string>> OkContinents = { {"Cresinion", "2"}};

    vector<vector<string>> OkTerritories = {{"1", "Bay of Dew",      "1"},
                                            {"2", "Byrgius",         "1"},
                                            {"3", "Ocean of Storms", "1"},
                                            {"4", "Sea of Rains",    "1"},
                                            {"5", "Harpalus",        "1"}};
    vector<vector<int>> validBorders = {{1, 2, 3, 4, 5}};

    Map validMap = Map(mapName, OkContinents);

    for (auto &i: OkTerritories) {
        validMap.addTerritory(stoi(i[0]), i[1], stoi(i[2]));
    }


    for (auto &i: validBorders) {
        for (unsigned int j = 1; j < i.size(); j++) {
            validMap.connectNeighbors(i[0], i[j]);
        }
    }


    string invalidMapName = "badMoon";
    vector<vector<string>> invalidContinent = {{"Sajon",     "4"},
                                               {"Cresinion", "2"},
                                               {"Delphot",   "2"}};

    vector<vector<string>> invalidTerritories = {{"1", "Bay of Dew",      "1"},
                                                 {"2", "Byrgius",         "1"},
                                                 {"3", "Ocean of Storms", "1"},
                                                 {"4", "Sea of Rains",    "1"},
                                                 {"5", "Harpalus",        "1"}};

    vector<vector<int>> invalidBorders = {{1, 2},
                                          {3, 4, 5}};

    Map invalidMap = Map(invalidMapName, invalidContinent);

    for (auto &i: invalidTerritories) {
        invalidMap.addTerritory(stoi(i[0]), i[1], stoi(i[2]));
    }


    for (auto &i: invalidBorders) {
        for (unsigned int j = 1; j < i.size(); j++) {
            invalidMap.connectNeighbors(i[0], i[j]);
        }
    }

    //test maps
   // validMap.toStringMap();
  // invalidMap.toStringMap();
     cout << "Testing valid map...\n Connected : " << validMap.validate() << "\n";
     cout << "Testing invalid map...\n Connected : " << invalidMap.validate() << "\n";
    return 0;
}


//    Map m1;
//    m1.name = "M1";
//
//    Continent c1, c2;
//    c1.name = "C1";
//    c2.name = "C2";
//
//    Territory t1 ("T1", "C1");
//    Territory t2 ("T2", "C1");
//    Territory t3 ("T3", "C1");
//
//    Continent *cp1 = &c1;
//    Territory *tp1 = &t1;
//    Territory *tp2 = &t2;
//    Territory *tp3 = &t3;
//
//    c1.addTerritory(tp1);
//    c1.addTerritory(tp2);
//    c1.addTerritory(tp3);
//
//    t1.adjacentTerritories.push_back(&t2);
//    t1.adjacentTerritories.push_back(&t3);
//
//      m1.addContinent(cp1);
//      // m1.addTerritoryToContinent(cp1, tp1);
//
//    m1.territories.push_back(&t1);
//    m1.territories.push_back(&t2);
//    m1.territories.push_back(&t3);
//
//   cout << m1.validate();

//    int comp = 0;
//    for (auto p : m1.continents) {
//        for (auto pp: p->ownedTerritories) {
//            m1.DfsTraverseTerritories(pp);
//            comp++;
//        }
//    }

//        cout << comp;
//    bool connected = true;
//    for (auto p : m1.territories) {
//        if (!p->visited)
//        connected= false;
//    }
//    if (connected)
//        cout << "Map is connected!!";


//MapLoader *loader = new MapLoader("../assets/Moon.map");
//    loader->parse();
//    for (auto p : loader->continentsLoad.name) {
//        cout << p << " ";
//    }

