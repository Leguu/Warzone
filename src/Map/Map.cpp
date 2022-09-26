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

Territory::Territory() {}

Territory::Territory(string name, string continent) {
    this->name = std::move(name);
    this->continent = std::move(continent);
    this->visited = false;
}


Territory::Territory(string name, string continent, vector<Territory *> adjacentTerritories) {
    this->name = std::move(name);
    this->continent = std::move(continent);
    this->adjacentTerritories = std::move(adjacentTerritories);
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
    this->name = std::move(name);
    this->bonus = bonus;
}

Continent::Continent(string name, int armies, vector<Territory *> territories) {
    this->name = std::move(name);
    this->bonus = armies;
    this->territories = std::move(territories);
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


unique_ptr<Map> MapLoader::importMap(const string &path) {
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

    auto map = make_unique<Map>("Map", allTerritories, continents);

    // Make sure to throw an exception if the map is invalid.

    return map;
}

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
    this->name = std::move(name);
    this->continents = std::move(continents);
}

Map::Map(string name, vector<Territory *> territories, vector<Continent *> continents) {
    this->name = std::move(name);
    this->territories = std::move(territories);
    this->continents = std::move(continents);

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
    territories = std::move(newT);
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
    auto newTerritory = new Territory(std::move(newName), continent);
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
    if(std::find(source->adjacentTerritories.begin(), source->adjacentTerritories.end(), dest)
                !=source->adjacentTerritories.end()){
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
                cerr << "\nMap is NOT a connected graph!" << endl;
                return false;
            }
            visited = traverseTerr(territory, visited);
        }
    }
    cout << "\nTotal territories in map: " << visited << endl;
    if (visited == territories.size()) {
        cout << "\nMap is a connected graph!" << endl;
        return true;
    } else {
        cout << "\nMap is NOT a connected graph!" << endl;
        return false;
    }
}
int Map::traverseTerr(Territory *territory, int visited) {
    vector<Territory *> adjacentTerritories = territory->adjacentTerritories;
    for (auto & adjTerr : adjacentTerritories) {
        if (!adjTerr->visited) {
            adjTerr->visited = true;
            visited = traverseTerr(adjTerr, visited);
        }
    }
    cout << "\nVisiting " << territory->getName() << endl;
    cout << "Total territories visited: " << visited + 1 << endl;
    return visited + 1;
}

bool Map::isSubgraphConnected() {
    resetTerr();
    for (auto & continent : continents) {
        string continentName = continent->getName();
        vector<Territory *> continentTerr = continent->getTerritories();
        cout << "\nChecking " << continentName << " which has " << to_string(continentTerr.size()) << " members" << endl;
        int visited = 0;
        for (auto &terr: continentTerr) {
            if (!terr->visited) {
                terr->visited = true;
                if (terr->adjacentTerritories.empty()) {
                    cerr << "\nContinent " << continent->getName() << " is NOT a connected sub graph!" << endl;
                    return false;
                }
                visited = traverseSubgraph(terr, continentName, visited);
            }
        }
        cout << "\nTotal territories in continent: " << to_string(visited) << endl;
        if (visited == continentTerr.size()) {
            cout << "\nContinent " << continent->getName() << " is a connected subgraph!" << endl;
        } else {
            cerr << "\nContinent " << continent->getName() << " is NOT a connected subgraph!" << endl;
            return false;
        }
    }
    cout << "\nAll continents are connected sub graphs!" << endl;
    return true;
}


int Map::traverseSubgraph(Territory *territory, const string& continent, int visited) {
    vector<Territory *> adjacentTerritories = territory->adjacentTerritories;
    for (auto & adjTerr : adjacentTerritories) {
        if (!adjTerr->visited && adjTerr->getContinent() == continent) {
            adjTerr->visited = true;
            visited = traverseSubgraph(adjTerr, continent, visited);
        }
    }
    cout << "\nVisiting " << territory->getName() << "..." << endl;
    cout << "Total territories visited: " << visited + 1 << endl;
    return visited + 1;
}

bool Map::isUniqueContinent() {
    map<string, string> listOfContinents;
    for (auto & continent : continents) {
        vector<Territory *> currTerritories = continent->getTerritories();
        for (auto & terr : currTerritories)
            if (listOfContinents.count(terr->getName()) > 0) {
                cout << "\nTerritory " << terr->getName() << " is assigned more than one continent!" << endl;
                return false;
            } else {
                listOfContinents[terr->getName()] = terr->getContinent();
            }
    }
    cout << "\nEach territory has a unique continent!" << endl;
    return true;
}


bool Map::validate() {
    return (isUniqueContinent() && isConnected() && isSubgraphConnected()) ;
}


/*
Map(std::string &mapName, vector<Continent *> continents) {
    pName = &mapName;
    pContinents = &continents;
    pTerritories = new vector<Territory *>();
}

Map(string name, std::vector<std::vector<std::string>> continentsWithTerritories) {
    pName = new string(name);
    pTerritories = new vector<Territory *>;
    pContinents = new vector<Continent *>;
    for (auto p: continentsWithTerritories) {
        pContinents->push_back(new Continent(p[0], std::stoi(p[1])));
    }
}

Map(const Map &orgMap) {
    pName = new string(*orgMap.pName);
    pTerritories = new vector<Territory *>(*orgMap.pTerritories);
    pContinents = new vector<Continent *>(*orgMap.pContinents);
}

~Map() {
    delete pName;
    delete pTerritories;
    delete pContinents;
}

Map &operator=(const Map &map) {
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

vector<Territory *> *getAllTerritories() {
    return pTerritories;
}

void setAllTerritories(vector<Territory *> *newT) {
    pTerritories = newT;
}

vector<Continent *> *getContinents() {
    return pContinents;
}

Territory *addTerritory(int id, string name, int continent) {
    auto *newTerritory = new Territory(id, name, continent);
    pTerritories->push_back(newTerritory);
    (*pContinents)[continent - 1]->addTerritory(newTerritory);
    return newTerritory;
}

void connectNeighbors(int source, int dest) {
    (*pTerritories)[source - 1]->getAdjTerritories()->push_back((*pTerritories)[dest - 1]);
}

void DFS(std::set<std::string> *visitedTerritories, Territory *territory, bool test) {
    if (visitedTerritories->find(territory->getTerritoryName()) == visitedTerritories->end()) {
        visitedTerritories->insert(territory->getTerritoryName());
        //  cout << territory->getTerritoryName() << " - ";
        for (auto &c: *territory->getAdjTerritories()) {
            if (!test || c->getContinentID() == territory->getContinentID()) {
                DFS(visitedTerritories, c, test);
            } else {
                continue;
            }
        }
    }
}

bool traverse(vector<Territory *> *startingPoint, bool isContinent) {
    auto *visitedTerritories = new std::set<std::string>;
    if (startingPoint) {
        DFS(visitedTerritories, startingPoint[0][0], isContinent);
        bool connected = visitedTerritories->size() == startingPoint->size();
        delete (visitedTerritories);
        return connected;
    } else return false;

}

bool validate() {
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

void toStringMap() {
    for (auto &pContinent: *this->pContinents) {
        string currContinent = pContinent->getName();
        vector<Territory *> TerrInContinent = *pContinent->getOwnedTerritories();
        cout << "--------------\n"
             << "Territories owned in this continent: " << currContinent << endl;

        for (auto &j: TerrInContinent) {
            string currTerrName = j->getTerritoryName();

            cout << "\t" << currTerrName << " occupied with " << j->getArmies() << " troops "
                 << " and is owned by " << j->getOwner() << endl;
        }
        cout << "--------------\n" << endl;
    }

}
*/


/*
bool uniqueContinent() {

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



void dfsTraverseTerritories(Territory *tp) {

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

bool validate() {
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


