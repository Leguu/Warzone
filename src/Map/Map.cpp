#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include "Map.h"

// ------------------ Territory ------------------------

using namespace std;

//int Territory::territoryIdIncrement = 0;
//int Continent::continentIdIncrement = 0;

Map::Territory::Territory() {}

Map::Territory::Territory(int id, string name, int continentID) {
    pID = new int(id);
    pName = new string(name);
    pOwner = new int(-1);
    pContinentID = new int(continentID - 1);
    pAdjacentTerritories = new vector<Territory *>;
    pArmies = new int(0);

}


Map::Territory::Territory(const Territory &orgTerritory) {
    this->pID = new int(*orgTerritory.pID);
    this->pName = new string(*orgTerritory.pName);
    this->pArmies = new int(*orgTerritory.pArmies);
    this->pContinentID = new int(*orgTerritory.pContinentID);
    this->pOwner = new int(*orgTerritory.pOwner);
    this->pAdjacentTerritories = new vector<Territory *>();
    this->pAdjacentTerritories->reserve(orgTerritory.pAdjacentTerritories->size());
    for (auto &p: *orgTerritory.pAdjacentTerritories) {
        this->pAdjacentTerritories->push_back(new Territory(*p));
    }
}

Map::Territory::~Territory() {
    delete pID;
    delete pName;
    delete pArmies;
    delete pContinentID;
    delete pOwner;
    delete pAdjacentTerritories;
}

Map::Territory &Map::Territory::operator=(const Map::Territory &territory) {
    this->pID = territory.pID;
    this->pName = territory.pName;
    this->pContinentID = territory.pContinentID;
    this->pOwner = territory.pOwner;
    this->pArmies = territory.pArmies;
    this->pAdjacentTerritories = territory.pAdjacentTerritories;
    return *this;
}


ostream &operator<<(ostream &os, const Map::Territory &territory) {
    os << "ID of territory: " << *territory.pID << "\n"
       << "Name of territory: " << *territory.pName << "\n"
       << "Continent ID: " << *territory.pContinentID << "\n"
       << "Owner of territory: " << *territory.pOwner << "\n"
       << "Number of armies: " << *territory.pArmies << "\n";

    return os;
}


vector<Map::Territory *> *Map::Territory::getAdjTerritories() {
    return pAdjacentTerritories;
}

string Map::Territory::getTerritoryName() {
    return *pName;
}

int Map::Territory::getOwner() {
    return *pOwner;
}

void Map::Territory::setOwner(int id) {
    *pOwner = id;
}

int Map::Territory::getArmies() {
    return *pArmies;
}

void Map::Territory::setArmies(int armies) {
    *pArmies = armies;
}

int Map::Territory::getContinentID() {
    return *pContinentID;
}

int Map::Territory::getTerritoryID() {
    return *pContinentID;
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

Map::Continent::Continent() {}

Map::Continent::Continent(string name, int bonus) {
    pName = new string(std::move(name));
    pBonus = new int(bonus);
    pOwnedTerritories = new vector<Territory *>;
}

Map::Continent::Continent(const Map::Continent &orgContinent) {
    pName = new string(*orgContinent.pName);
    pBonus = new int(*orgContinent.pBonus);
    pOwnedTerritories = new vector<Territory *>(*orgContinent.pOwnedTerritories);
}

Map::Continent::~Continent() {
    delete pName;
    delete pBonus;
    delete pOwnedTerritories;
}

Map::Continent &Map::Continent::operator=(const Map::Continent &continent) {
    this->pName = continent.pName;
    this->pBonus = continent.pBonus;
    this->pOwnedTerritories = continent.pOwnedTerritories;
}

std::ostream &operator<<(ostream &os, const Map::Continent &continent) {
    os << "Name of continent: " << *continent.pName << "\n"
       << "Bonus: " << *continent.pBonus << "\n";
    return os;
}


void Map::Continent::addTerritory(Map::Territory *territory) {
    pOwnedTerritories->push_back(territory);
}

vector<Map::Territory *> *Map::Continent::getOwnedTerritories() {
    return pOwnedTerritories;
}

string Map::Continent::getName() {
    return *pName;
}

int Map::Continent::getBonus() {
    return *pBonus;
}


/*Player *Continent::owner() {
    return nullptr;
}*/

/*
unique_ptr<Map> MapLoader::importMap(const string &path) {
    auto allTerritories = vector<Territory *>{};

    auto expel = new Continent("Expel", 4);

    auto arlia = new Territory("Arlia");
    arlia->continent = expel;
    auto krosse = new Territory("Krosse");
    krosse->continent = expel;

    arlia->adjacentTerritories = {krosse};
    krosse->adjacentTerritories = {arlia};

    vector<Continent *> continents{expel};

    allTerritories.push_back(arlia);
    allTerritories.push_back(krosse);

    expel->territories = allTerritories;

    auto map = make_unique<Map>(continents, allTerritories);

    // Make sure to throw an exception if the map is invalid.

    return map;
}
*/

/*
Territory *Map::findById(int id) const {
    for (auto territory: territories) {
        if (territory->id == id) {
            return territory;
        }
    }
    return nullptr;
}
*/

// ------------------ Maps ------------------------

Map::Map() {}

Map::Map(std::string &mapName, vector<Continent *> continents) {
    pName = &mapName;
    pContinents = &continents;
    pTerritories = new vector<Territory *>();
}

Map::Map(string name, std::vector<std::vector<std::string>> continentsWithTerritories) {
    pName = new string(name);
    pTerritories = new vector<Territory *>;
    pContinents = new vector<Continent *>;
    for (auto p: continentsWithTerritories) {
        pContinents->push_back(new Continent(p[0], std::stoi(p[1])));
    }
}

Map::Map(const Map &orgMap) {
    pName = new string(*orgMap.pName);
    pTerritories = new vector<Territory *>(*orgMap.pTerritories);
    pContinents = new vector<Continent *>(*orgMap.pContinents);
}

Map::~Map() {
    delete pName;
    delete pTerritories;
    delete pContinents;
}

Map &Map::operator=(const Map &map) {
    this->pName = map.pName;
    this->pContinents = map.pContinents;
    this->pTerritories = map.pTerritories;
    return *this;
}

ostream &operator<<(ostream &os, const Map &map) {
    os << "Name of map:\n" << *map.pName << "\n"
       << "Number of continents: " << map.pTerritories->size() << "\n"
       << " Number of territories: " << map.pContinents->size();
    return os;
}

vector<Map::Territory *> *Map::getAllTerritories() {
    return pTerritories;
}

void Map::setAllTerritories(vector<Territory *> *newT) {
    pTerritories = newT;
}

vector<Map::Continent *> *Map::getContinents() {
    return pContinents;
}

Map::Territory *Map::addTerritory(int id, string name, int continent) {
    auto *newTerritory = new Territory(id, name, continent);
    pTerritories->push_back(newTerritory);
    (*pContinents)[continent - 1]->addTerritory(newTerritory);
    return newTerritory;
}

void Map::connectNeighbors(int source, int dest) {
    (*pTerritories)[source - 1]->getAdjTerritories()->push_back((*pTerritories)[dest - 1]);
}

void Map::DFS(std::set<std::string>* visitedTerritories, Territory* territory, bool test){
    if(visitedTerritories->find(territory->getTerritoryName()) == visitedTerritories->end()){
        visitedTerritories->insert(territory->getTerritoryName());
       // cout << territory->getTerritoryName() << " - ";
        for(auto& c : *territory->getAdjTerritories()) {
            if(!test || c->getContinentID() == territory->getContinentID()){
                DFS(visitedTerritories, c, test);
            }else{
                continue;
            }
        }
    }
}

bool Map::traverse(vector<Territory *> *startingPoint, bool isContinent) {
    auto *visitedTerritories = new std::set<std::string>;
    if(!startingPoint) {
        DFS(visitedTerritories, startingPoint[0][0], isContinent);
        bool connected = visitedTerritories->size() == startingPoint->size();
        delete (visitedTerritories);
        return connected;
    }
    else return false;

}

bool Map::validate() {
    if (traverse(this->pTerritories, false)) {
        for (auto c: *this->pContinents) {
            if (c->getOwnedTerritories()->size() == 1) {
                continue;
            } else if (!traverse(c->getOwnedTerritories(), true)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Map::toStringMap() {
    for (auto &pContinent: *this->pContinents) {
        string currContinent = pContinent->getName();
        vector<Map::Territory *> TerrInContinent = *pContinent->getOwnedTerritories();
        cout << "--------------\n"
             << "Territories owned in this continent: " << currContinent << endl;

        for (auto &j: TerrInContinent) {
            string currTerrName = j->getTerritoryName();

            cout << "\t" <<currTerrName << " occupied with " << j->getArmies() << " troops "
                 << " and is owned by " << j->getOwner() << endl;
        }
        cout << "--------------\n" << endl;
    }

}


/*
bool Map::uniqueContinent() {

    int numOfContinents;

    for (auto p1: territories) {
        numOfContinents = 0;
        for (auto p2: continents) {
            for (auto p3: p2->ownedTerritories) {
                if (p3->id == p1->id)
                    numOfContinents++;
            }
        }
        if (numOfContinents != 1)
            return false;
    }

    return true;
}



void Map::dfsTraverseTerritories(Territory *tp) {

    cout << "Started Traversing " << tp->name << endl;
    tp->visited = true;

    // checking subgraph at the same time
    for (auto p: continents) {
        if (tp->name == p->name) {
            p->visited = true;
        }
    }

    for (size_t i = 0; i < tp->adjacentTerritories.size(); i++) {
        if (!tp->adjacentTerritories[i]->visited) {
            cout << tp->adjacentTerritories[i]->name << " has been visited!" << endl;
            dfsTraverseTerritories(tp->adjacentTerritories[i]);
        }
    }
    cout << "Finished Traversing " << tp->name << endl;

}

bool Map::validate() {
    resetVisitedTerritories();
    if (!uniqueContinent())
        return false;

    dfsTraverseTerritories(territories[0]);

    for (auto p: territories) {
        if (!p->visited)
            return false;
    }
    return true;
}
*/



/*

MapLoader::MapLoader(string
                     path) {
    mapFile = std::move(path);
}

MapLoader::MapLoader(
        const MapLoader &orgMapLoader) {
    mapFile = orgMapLoader.mapFile;
}

MapLoader::~MapLoader() {
    mapFile = "";
}

MapLoader &MapLoader::operator=(const MapLoader &mapLoader) {
    mapFile = mapLoader.mapFile;
    return *this;
}

ostream &operator<<(ostream &os, const MapLoader &mapLoader) {
    os << "Map loader file location: " << mapLoader.mapFile;
}


bool MapLoader::parse() {
    ifstream inputStream{mapFile};
    if (!inputStream) {
        cerr << "File not found" << endl;
        return false;
    } else {
        int locationInMap = -100;
        string line{};
        while (getline(inputStream, line)) {
            if (line.find("[Continents]") != -1) {
                locationInMap = 0;
                continue;
                vector<string> continentLine = splitString(line, '=');
            } else if (line.find("[Territories]") != -1) {
                locationInMap = 1;
                continue;
            }

            if (!line.empty()) {
                switch (locationInMap) {
                    case 0:
                        parseContinent(line);
                        break;
                    case 1:
                        break;
                }

            }
        }

    }
}

vector<string> MapLoader::splitString(const string &line, char delim) {
    vector<string> result;
    stringstream in(line);
    string element;

    while (getline(in, element, delim)) {
        result.push_back(element);
    }
    return result;
}

bool MapLoader::parseContinent(std::string line) {
    vector<string> newLine = splitString(line, '=');

    if (newLine.size() == 2) {


        continentsLoad.name.push_back(newLine[0]);
        int armies = std::stoi(newLine[1]);
        continentsLoad.armies.push_back(armies);

    }

    return false;
}


*/


