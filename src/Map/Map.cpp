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
  os << territory.toString();

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
  if (owner) {
    std::remove(owner->ownedTerritories.begin(),
                owner->ownedTerritories.end(),
                this); // NOLINT(bugprone-unused-return-value)
  }
  owner = newOwner;
  if (newOwner) {
    newOwner->ownedTerritories.push_back(this);
  }
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

string Territory::listNameAndAdjacent() {
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

string Territory::longDescription() {
  string str;
  str += "Description for territory [" + to_string(id) + "] " + name + "\n";
  str += "Continent: " + continent->getName() + "\n";
  str += "Owner:     " + (owner ? owner->name : "none") + "\n";
  str += "Armies:    " + to_string(armies) + "\n";
  str += "Adjacent:  ";
  for (auto t : adjacentTerritories) {
    str += "* " + t->getName() + " ";
  }
  return str;
}


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
      cout << territory->listNameAndAdjacent() << endl;
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

void Map::assertSubgraphConnected() {
  for (auto continent : continents) {
    if (continent->getTerritories().size() <= 1) {
      continue;
    }

    for (auto territory : continent->getTerritories()) {
      for (auto adj : territory->getAdjTerritories()) {
        if (adj->getContinent() == territory->getContinent())
          goto t;
      }
      throw runtime_error("Territory " + territory->getName() + " cannot be reached from within its continent!");

      t:;
    }
  }
}

void Map::assertEachTerritoryHasUniqueContinent() {
  map<string, string> listOfContinents;
  for (auto continent : continents) {
    for (auto &terr : continent->getTerritories())
      if (listOfContinents.count(terr->getName()) > 0) {
        throw runtime_error("Territory " + terr->getName() + " appears in more than one continent");
      } else {
        listOfContinents[terr->getName()] = terr->getContinent()->getName();
      }
  }
}

bool Map::validate() {
  // Prof says that one-way adjacencies are allowed
//  assertEveryEdgeIsTwoWay();
  assertConnected();
  assertSubgraphConnected();
  assertEachTerritoryHasUniqueContinent();
  return true;
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
            "Territory " + t->getName() + " is adjacent to " + adj->getName() +
                ", but it's not adjacent back!");
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
  return p;
}

Territory *Map::getInputTerritory(bool cancelable) {
  if (cancelable) {
    cout << " Type \"cancel\" to cancel this input." << endl;
  }

  while (true) {
    auto input = Utils::getInputString();

    if (cancelable && Utils::isEqualLowercase(input, "cancel")) {
      break;
    }

    auto territory = findTerritory(input);

    if (!territory) {
      cout << "Your input did not correspond to any territory. Try again" << endl;
      continue;
    }

    return territory;
  }
  throw Utils::CancelledInputException();
}

Territory *Map::getInputTerritory(const string &inputRequest, bool cancelable) {
  cout << inputRequest;
  return getInputTerritory(cancelable);
}

Territory *Map::findTerritory(const string &input) {
  try {
    auto value = std::stoi(input);
    auto territory = findById(value);
    if (territory)
      return territory;
  } catch (std::invalid_argument &e) {
    auto territory = findTerritoryByName(input);
    if (territory)
      return territory;
  }
  return nullptr;
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
    if (split.size() < 2) {
      continue;
    }

    auto name = split[0];
    auto bonus = stoi(split[1]);

    map->addContinent(new Continent(name, bonus));
  }
  if (input != "[Territories]") {
    throw runtime_error("No territories section found!");
  }

  while (getline(file, input)) {
    if (input[0] == ';')
      continue;

    auto split = Utils::tokenizer(input, ',');
    if (split.size() < 5) {
      continue;
    }

    auto name = split[0];
    auto continentName = split[3];
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

    for (int i = 4; i < split.size(); i += 1) {
      auto adjacentTerritoryName = split[i];
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
                            +
                                "' failed to validate. It has an unplayable map due to missing adjacencies, isolated territories, or territories that belong to more than one continent.");
  }

  return map;
}