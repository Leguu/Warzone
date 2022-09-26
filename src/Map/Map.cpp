#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include "Map.h"
#include "../Player/Player.h"

// ------------------ Territory ------------------------

using namespace std;
int Territory::idIncrement = 0;
bool debug = false;

Territory::Territory() {}

Territory::Territory(string name, string continent) {
    this->name = name;
    this->continent = continent;
    this->visited = false;
}


Territory::Territory(string name, string continent, vector<Territory *> adjacentTerritories) {
    this->name = name;
    this->continent = continent;
    this->adjacentTerritories = adjacentTerritories;
    this->visited = false;
}

Territory::Territory(const Territory &territory) {
    this->id = territory.id;
    this->name = territory.name;
    this->continent = territory.continent;
    this->adjacentTerritories = territory.adjacentTerritories;
    this->armies = territory.armies;
    this->owner = territory.owner;
    this->visited = territory.visited;
}

Territory &Territory::operator=(const Territory &territory) {
    this->id = territory.id;
    this->name = territory.name;
    this->continent = territory.continent;
    this->adjacentTerritories = territory.adjacentTerritories;
    this->armies = territory.armies;
    this->owner = territory.owner;
    this->visited = territory.visited;
    return *this;
}

ostream &operator<<(ostream &os, const Territory &territory) {
    os << "ID of territory: " << territory.id << "\n"
       << "Name of territory: " << territory.name << "\n"
       << "Continent ID: " << territory.continent << "\n"
       << "Owner of territory: " << territory.owner << "\n"
       << "Number of armies: " << territory.armies << "\n";

    return os;
}

Territory::~Territory() {
    delete owner;
}

vector<Territory *> Territory::getAdjTerritories() const {
    return adjacentTerritories;
}

string Territory::getName() {
    return name;
}

int Territory::getId() const {
    return id;
}

string Territory::getContinent() {
    return continent;
}

int Territory::getArmies() const {
    return armies;
}

void Territory::setArmies(int num) {
    armies = num;
}

Player *Territory::getOwner() {
    return owner;
}

void Territory::setOwner(Player *newOwner) {
    owner = newOwner;

}

string Territory::toString() const {
    string str;
    str += "[" + to_string(id) + "] ";
    str += name;
    if (owner) {
        str += " (" + to_string(armies) + ", " + owner->name + ")";
    } else {
        str += " (" + to_string(armies) + ")";
    }
    return str;
}

/*string Territory::toString() const {
    string str;
    str += "[" + to_string(id) + "] ";
    str += name;
    if (owner) {
        str += " (" + to_string(armies) + ", " + owner->name + ")";
    } else {
        str += " (" + to_string(armies) + ")";
    }
    return str;
}*/

// ------------------ Continents ------------------------

Continent::Continent() {}

Continent::Continent(string name, int bonus) {
    this->name = name;
    this->bonus = bonus;
}

Continent::Continent(string name, int armies, vector<Territory *> territories) {
    this->name = name;
    this->bonus = armies;
    this->territories = territories;
}


Continent::Continent(const Continent &orgContinent) {
    this->name = orgContinent.name;
    this->bonus = orgContinent.bonus;
    this->territories = orgContinent.territories;
}

Continent::~Continent() {
}

Continent &Continent::operator=(const Continent &continent) {
    this->name = continent.name;
    this->bonus = continent.bonus;
    this->territories = continent.territories;
    return *this;
}

std::ostream &operator<<(ostream &os, const Continent &continent) {
    os << "Name of continent: " << continent.name << "\n"
       << "Bonus: " << continent.bonus << "\n";
    return os;
}

vector<Territory *> Continent::getTerritories() {
    return territories;
}

string Continent::getName() {
    return name;
}

int Continent::getBonus() const {
    return bonus;
}

void Continent::addTerritoryToContinent(Territory *territory) {
    territories.push_back(territory);
}


/*Player *Continent::owner() {
    return nullptr;
}*/


Territory *Map::findById(int id) const {
    for (auto territory: territories) {
        if (territory->getId() == id) {
            return territory;
        }
    }
    return nullptr;
}


// ------------------ Maps ------------------------

Map::Map() {}

Map::Map(string name, vector<Continent *> continents) {
    this->name = name;
    this->continents = continents;
}

Map::Map(string name, vector<Territory *> territories, vector<Continent *> continents) {
    this->name = name;
    this->territories = territories;
    this->continents = continents;

}

Map::Map(const Map &orgMap) {
    this->territories = orgMap.territories;
    this->continents = orgMap.continents;
}

Map::~Map() {
    for (auto &p: continents) {
        delete p;
    }
    for (auto &p: territories) {
        delete p;
    }
}

Map &Map::operator=(const Map &map) {
    this->territories = map.territories;
    this->continents = map.continents;
    return *this;
}

std::ostream &operator<<(ostream &os, const Map &map) {
    for (auto &pContinent: map.continents) {
        string currContinent = pContinent->getName();
        vector<Territory *> TerrInContinent = pContinent->getTerritories();
        os << "--------------\n"
           << "Territories owned in this continent: " << currContinent << endl;

        for (auto &j: TerrInContinent) {
            string currTerrName = j->getName();

            os << "\t" << currTerrName << " occupied with " << j->getArmies() << " troops "
               << " and is owned by " << j->getOwner() << endl;
        }
        os << "--------------\n" << endl;
    }

    return os;
}


vector<Territory *> Map::getAllTerritories() {
    return territories;
}

void Map::setAllTerritories(vector<Territory *> newT) {
    territories = newT;
}

vector<Continent *> Map::getContinents() {
    return continents;
}

void Map::addTerritoryToMap(Territory *terr) {
    territories.push_back(terr);
    for (auto &it: continents) {
        if ((*it).getName() == terr->getContinent()) {
            it->addTerritoryToContinent(terr);
        }
    }
}

void Map::addTerritoryToMap(string newName, const string &continent) {
    auto newTerritory = new Territory(newName, continent);
    territories.push_back(newTerritory);
    for (auto &it: continents) {
        if ((*it).getName() == continent) {
            it->addTerritoryToContinent(newTerritory);
        }
    }
}

void Map::addContinent(Continent *continent) {
    continents.push_back(continent);
}

void Map::addEdge(Territory *source, Territory *dest) {
    if (std::find(source->adjacentTerritories.begin(), source->adjacentTerritories.end(), dest)
        != source->adjacentTerritories.end()) {
        cout << "Found existing edge" << endl;
    } else {
        source->adjacentTerritories.push_back(dest);
    }

}

void Map::resetTerr() {
    for (auto &territory: territories) {
        if (territory->visited) {
            territory->visited = false;
        }
    }
}

bool Map::isConnected() {
    resetTerr();
    int visited = 0;
    for (auto &territory: territories) {
        if (!territory->visited) {
            territory->visited = true;
            if (territory->adjacentTerritories.empty()) {
                if (debug)
                    cerr << "\nMap is NOT a connected graph!" << endl;
                return false;
            }
            visited = traverseTerr(territory, visited);
        }
    }
    if (debug)
        cout << "\nTotal territories in map: " << visited << endl;
    if (visited == territories.size()) {
        if (debug)
            cout << "\nMap is a connected graph!" << endl;
        return true;
    } else {
        if (debug)
            cout << "\nMap is NOT a connected graph!" << endl;
        return false;
    }
}

int Map::traverseTerr(Territory *territory, int visited) {
    vector<Territory *> adjacentTerritories = territory->adjacentTerritories;
    for (auto &adjTerr: adjacentTerritories) {
        if (!adjTerr->visited) {
            adjTerr->visited = true;
            visited = traverseTerr(adjTerr, visited);
        }
    }
    if (debug) {
        cout << "\nVisiting " << territory->getName() << endl;
        cout << "Total territories visited: " << visited + 1 << endl;
    }
    return visited + 1;
}

bool Map::isSubgraphConnected() {
    resetTerr();
    for (auto &continent: continents) {
        string continentName = continent->getName();
        vector<Territory *> continentTerr = continent->getTerritories();
        if (debug)
            cout << "\nChecking " << continentName << " which has " << to_string(continentTerr.size()) << " members"
                 << endl;
        int visited = 0;
        for (auto &terr: continentTerr) {
            if (!terr->visited) {
                terr->visited = true;
                if (terr->adjacentTerritories.empty()) {
                    if (debug)
                        cerr << "\nContinent " << continent->getName() << " is NOT a connected sub graph!" << endl;
                    return false;
                }
                visited = traverseSubgraph(terr, continentName, visited);
            }
        }
        if (debug)
            cout << "\nTotal territories in continent: " << to_string(visited) << endl;
        if (visited == continentTerr.size()) {
            if (debug)
                cout << "\nContinent " << continent->getName() << " is a connected subgraph!" << endl;
        } else {
            if (debug)
                cerr << "\nContinent " << continent->getName() << " is NOT a connected subgraph!" << endl;
            return false;
        }
    }
    if (debug)
        cout << "\nAll continents are connected sub graphs!" << endl;
    return true;
}


int Map::traverseSubgraph(Territory *territory, const string &continent, int visited) {
    vector<Territory *> adjacentTerritories = territory->adjacentTerritories;
    for (auto &adjTerr: adjacentTerritories) {
        if (!adjTerr->visited && adjTerr->getContinent() == continent) {
            adjTerr->visited = true;
            visited = traverseSubgraph(adjTerr, continent, visited);
        }
    }
    if (debug) {
        cout << "\nVisiting " << territory->getName() << "..." << endl;
        cout << "Total territories visited: " << visited + 1 << endl;
    }
    return visited + 1;
}

bool Map::isUniqueContinent() {
    map<string, string> listOfContinents;
    for (auto &continent: continents) {
        vector<Territory *> currTerritories = continent->getTerritories();
        for (auto &terr: currTerritories)
            if (listOfContinents.count(terr->getName()) > 0) {
                if (debug)
                    cout << "\nTerritory " << terr->getName() << " is assigned more than one continent!" << endl;
                return false;
            } else {
                listOfContinents[terr->getName()] = terr->getContinent();
            }
    }
    if (debug)
        cout << "\nEach territory has a unique continent!" << endl;
    return true;
}


bool Map::validate() {
    return (isUniqueContinent() && isConnected() && isSubgraphConnected());
}


Map* MapLoader::importMap(const string &path) {
    auto allTerritories = vector<Territory *>{};

    auto expel = new Continent("Expel", 4);

    auto arlia = new Territory("Arlia", "Expel");

    auto krosse = new Territory("Krosse", "Expel");


    arlia->adjacentTerritories = {krosse};
    krosse->adjacentTerritories = {arlia};

    vector<Continent *> continents{expel};

    allTerritories.push_back(arlia);
    allTerritories.push_back(krosse);

    expel->getTerritories() = allTerritories;

    auto map = new Map("Map", allTerritories, continents);

    // Make sure to throw an exception if the map is invalid.

    return map;
}

