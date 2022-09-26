#include <iostream>
#include <string>
#include <vector>
#include "Map.h"

using namespace std;

int main() {

    // Hardcoded map
    vector<Territory*> neighbours;
    vector<Continent*> continents;

    Map map("testMap",neighbours, continents);

    auto* na = new Continent("North America", 3);
    auto* ca = new Continent("Central America", 3);

    map.addContinent(na);
    map.addContinent(ca);

    auto* canada = new Territory("Canada", "North America");
    auto* us = new Territory("United States", "North America");
    auto* mexico = new Territory("Mexico", "North America");
    auto* guatemala = new Territory("Guatemala", "Central America");
    auto* belize = new Territory("Belize", "Central America");
    auto* elSalvador = new Territory("El Salvador", "Central America");
    auto* honduras = new Territory("Honduras", "Central America");
    auto* nicaragua = new Territory("Nicaragua", "Central America");

    map.addTerritoryToMap(canada);
    map.addTerritoryToMap(us);
    map.addTerritoryToMap(mexico);
    map.addTerritoryToMap(guatemala);
    map.addTerritoryToMap(belize);
    map.addTerritoryToMap(elSalvador);
    map.addTerritoryToMap(honduras);
    map.addTerritoryToMap(nicaragua);


    map.addEdge(canada, us);
    map.addEdge(canada, us);
    map.addEdge(us, mexico);
    map.addEdge(mexico, guatemala);
    map.addEdge(mexico, belize);
    map.addEdge(guatemala, belize);
    map.addEdge(guatemala, elSalvador);
    map.addEdge(guatemala, honduras);
    map.addEdge(belize, honduras);
    map.addEdge(elSalvador, honduras);
    map.addEdge(honduras, nicaragua);

        for (auto p: map.getAllTerritories()) {
            cout << "\n----------\n" << p->getName() << " " << p->getId() << endl;
            for (auto pp: p->getAdjTerritories()) {
                cout << pp->getName() << " - ";
            }
            cout << "\n";
        }

  // cout << map;

   cout << map.validate();


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

