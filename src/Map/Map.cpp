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

Territory::Territory(const string &name, Continent *continent) : name(Utils::trim(name)), continent(continent) {
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

Territory &Territory::operator=(const Territory &territory) = default;

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

Continent *Territory::getContinent() {
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
void Territory::setContinent(Continent *pContinent) {
  continent = pContinent;
}
string Territory::longDescription() {
  auto str = name + ": ";
  for (auto territory : adjacentTerritories) {
    str += "* " + territory->getName() + " ";
  }
  return str;
}

void Territory::addAdjacent(Territory *territory) {
  if (std::find(adjacentTerritories.begin(), adjacentTerritories.end(), territory) != adjacentTerritories.end()) {
    throw runtime_error(name + " is already adjacent to " + territory->name);
  }
  adjacentTerritories.push_back(territory);
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

Continent::Continent(string name, int bonus) : name(Utils::trim(name)), bonus(bonus) {
}

Continent::Continent(const Continent &orgContinent) {
  this->name = orgContinent.name;
  this->bonus = orgContinent.bonus;
  this->territories = orgContinent.territories;
}

Continent::~Continent() = default;

Continent &Continent::operator=(const Continent &continent) = default;

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
Player *Continent::owner() {
  Player *p = nullptr;
  for (auto t : territories) {
    if (!p) {
      p = t->getOwner();
    }

    if (p != t->getOwner()) {
      return nullptr;
    }
  }

  return p;
}

Territory *Map::findById(int id) const {
  for (auto territory : territories) {
    if (territory->getId() == id) {
      return territory;
    }
  }
  return nullptr;
}


// ------------------ Maps ------------------------

Map::Map() = default;

Map::Map(const Map &orgMap) {
  this->territories = orgMap.territories;
  this->continents = orgMap.continents;
}

Map::~Map() {
  for (auto p : continents) {
    delete p;
  }
  for (auto p : territories) {
    delete p;
  }
}

Map &Map::operator=(const Map &map) {
  this->territories = map.territories;
  this->continents = map.continents;
  return *this;
}

std::ostream &operator<<(ostream &os, const Map &map) {
  auto divider = "---------------\n";
  for (auto continent : map.continents) {
    cout << divider;
    cout << continent->getName() << endl;
    cout << divider;

    for (auto territory : continent->getTerritories()) {
      cout << territory->longDescription() << endl;
    }
  }

  return os;
}

vector<Territory *> Map::getAllTerritories() {
  return territories;
}

vector<Continent *> Map::getContinents() {
  return continents;
}

void Map::addTerritoryToMap(Territory *terr) {
  territories.push_back(terr);
}

void Map::addContinent(Continent *continent) {
  continents.push_back(continent);
}

void Map::resetTerr() {
  for (auto &territory : territories) {
    if (territory->visited) {
      territory->visited = false;
    }
  }
}

void Map::assertConnected() {
  resetTerr();

  auto previous = 0;

  for (auto &territory : territories) {
    if (territory->visited)
      continue;

    territory->visited = true;

    auto visited = traverseTerr(territory, previous);

    if (previous && visited != previous) {
      throw runtime_error("Territory " + territory->getName() + " is isolated from the rest of the map!");
    }

    previous = visited;
  }
}

int Map::traverseTerr(Territory *territory, int visited) {
  vector<Territory *> adjacentTerritories = territory->getAdjTerritories();

  for (auto t : adjacentTerritories) {
    if (!t->visited) {
      t->visited = true;
      visited = traverseTerr(t, visited);
    }
  }

  return visited + 1;
}

bool Map::isSubgraphConnected() {
  resetTerr();
  for (auto &continent : continents) {
    string continentName = continent->getName();
    vector<Territory *> continentTerr = continent->getTerritories();
    int visited = 0;
    for (auto &terr : continentTerr) {
      if (!terr->visited) {
        terr->visited = true;
        if (terr->getAdjTerritories().empty()) {
          return false;
        }
        visited = traverseSubgraph(terr, continentName, visited);
      }
    }
    if (visited != continentTerr.size()) {
      return false;
    }
  }
  return true;
}

int Map::traverseSubgraph(Territory *territory, const string &continent, int visited) {
  vector<Territory *> adjacentTerritories = territory->getAdjTerritories();
  for (auto adjTerr : adjacentTerritories) {
    if (!adjTerr->getContinent()) {
      throw runtime_error("Error while traversing subgraph: a territory doesn't have a continent!");
    }
    if (!adjTerr->visited && adjTerr->getContinent()->getName() == continent) {
      adjTerr->visited = true;
      visited = traverseSubgraph(adjTerr, continent, visited);
    }
  }
  return visited + 1;
}

bool Map::isUniqueContinent() {
  map<string, string> listOfContinents;
  for (auto &continent : continents) {
    vector<Territory *> currTerritories = continent->getTerritories();
    for (auto &terr : currTerritories)
      if (listOfContinents.count(terr->getName()) > 0) {
        return false;
      } else {
        listOfContinents[terr->getName()] = terr->getContinent()->getName();
      }
  }
  return true;
}

bool Map::validate() {
  assertEveryEdgeIsTwoWay();
  assertConnected();
  return (isUniqueContinent() && isSubgraphConnected());
}

Continent *Map::findContinentByName(const string &continentName) {
  auto trimmed = Utils::trim(continentName);
  for (auto continent : continents) {
    if (Utils::isEqualLowercase(continent->getName(), trimmed)) {
      return continent;
    }
  }
  return nullptr;
}

Territory *Map::findTerritoryByName(const string &territoryName) {
  auto trimmed = Utils::trim(territoryName);
  for (auto territory : territories) {
    if (Utils::isEqualLowercase(territory->getName(), trimmed)) {
      return territory;
    }
  }
  return nullptr;
}
void Map::assertEveryEdgeIsTwoWay() {
  for (auto t : territories) {
    for (auto adj : t->getAdjTerritories()) {
      auto adjacents = adj->getAdjTerritories();
      if (std::find(adjacents.begin(), adjacents.end(), t) == adjacents.end()) {
        throw runtime_error(
            "Territory " + t->getName() + " is adjacent to " + adj->getName() + ", but it's not adjacent back!");
      }
    }
  }
}
bool Map::allContinentsOwned() {
  Player *p = nullptr;
  for (auto c : continents) {
    if (!p) {
      p = c->owner();
    }

    if (p != c->owner()) {
      return false;
    }
  }
  return true;
}

Map *MapLoader::importMap(const string &path) {
  ifstream file(path);

  if (!file.is_open()) {
    throw runtime_error("File " + path + " could not be opened!");
  }

  string input;
  while (getline(file, input)) {
    if (input == "[Continents]") {
      break;
    }
  }
  if (input != "[Continents]") {
    throw runtime_error("This map file does not have a Continents section!");
  }

  auto map = new Map();

  while (getline(file, input)) {
    if (input[0] == ';')
      continue;

    if (input == "[Territories]") {
      break;
    }

    auto split = Utils::tokenizer(input, '=');
    if (split->size() < 2) {
      continue;
    }

    auto name = (*split)[0];
    auto bonus = stoi((*split)[1]);

    map->addContinent(new Continent(name, bonus));
  }
  if (input != "[Territories]") {
    throw runtime_error("No territories section found!");
  }

  while (getline(file, input)) {
    if (input[0] == ';')
      continue;

    auto split = Utils::tokenizer(input, ',');
    if (split->size() < 5) {
      continue;
    }

    auto name = (*split)[0];
    auto continentName = (*split)[3];
    auto continent = map->findContinentByName(continentName);

    if (!continent) {
      throw runtime_error(
          "Territory " + name + " references a continent " + continentName + " that does not exist!");
    }

    auto territory = map->findTerritoryByName(name);
    if (!territory) {
      territory = new Territory(name);
      map->addTerritoryToMap(territory);
    }
    territory->setContinent(continent);
    continent->addTerritoryToContinent(territory);

    for (int i = 4; i < split->size(); i += 1) {
      auto adjacentTerritoryName = (*split)[i];
      auto adjacentTerritory = map->findTerritoryByName(adjacentTerritoryName);

      if (!adjacentTerritory) {
        adjacentTerritory = new Territory(adjacentTerritoryName);
        map->addTerritoryToMap(adjacentTerritory);
      }

      territory->addAdjacent(adjacentTerritory);
    }
  }

  if (!map->validate()) {
    throw runtime_error("The map in '" + path
                            + "' failed to validate. It has an unplayable map due to missing adjacencies, isolated territories, or territories that belong to more than one continent.");
  }

  return map;
}