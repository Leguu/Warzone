#include "Map.h"
#include "../Player/Player.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ------------------ Territory ------------------------

using namespace std;
int Territory::idIncrement = 0;

/**
 * Territory constructor
 * @param name name of the territory
 * @param continent name of the continent it bleongs
 */
Territory::Territory(const string &name, Continent *continent)
    : name(Utils::trim(name)), continent(continent) {}

/**
 * Copy constructor for Territory
 * @param territory The territory it's copying
 */
Territory::Territory(const Territory &territory) {
  this->id = territory.id;
  this->name = territory.name;
  this->continent = territory.continent;
  this->adjacentTerritories = territory.adjacentTerritories;
  this->armies = territory.armies;
  this->owner = territory.owner;
  this->visited = territory.visited;
}

/**
 * Overload for the = operator on territory
 * @param territory the territory we are comparing to
 * @return Copy constructor used to create new territory
 */
Territory &Territory::operator=(const Territory &territory) = default;

/**
 * Overload of the output operator
 * @param os The OSStream we will be appending to
 * @param territory The territory we will print
 * @return A string containing the content's of the territory
 */
ostream &operator<<(ostream &os, const Territory &territory) {
  os << territory.toString();

  return os;
}

/**
 * Territory destructor
 */
Territory::~Territory() = default;

/**
 * Get all adjacent territories
 * @return all adjacent territories
 */
vector<Territory *> Territory::getAdjTerritories() const {
  return adjacentTerritories;
}

/**
 * Get the name of the territory
 * @return
 */
string Territory::getName() {
  return name;
}

/**
 * Get the id of the territory
 * @return id of the territory
 */
int Territory::getId() const {
  return id;
}

/**
 * Get the continent the territory belongs to
 * @return the continent the territory belongs to
 */
Continent *Territory::getContinent() {
  return continent; }

/**
 * Get the armies on the territory
 * @return the armies on the territory
 */
int Territory::getArmies() const {
  return armies;
}

/**
 * Set the armies on the territories
 * @param num the new armies on the territories
 */
void Territory::setArmies(int num) {
  armies = num;
}

/**
 * Get the owner of the territory
 * @return the owner (Player object) of the territory
 */
Player *Territory::getOwner() {
  return owner;
}

/**
 * Set the owner of a territory
 * @param newOwner The new player object who owns this territory
 */
void Territory::setOwner(Player *newOwner) {
  if (owner) {
    std::remove(owner->ownedTerritories.begin(), owner->ownedTerritories.end(),
                this); // NOLINT(bugprone-unused-return-value)
  }
  owner = newOwner;
  if (newOwner) {
    newOwner->ownedTerritories.push_back(this);
  }
}

/**
 * Print a territory to string
 * @return the territory information in string format
 */
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

/**
 * Set the continent of a territory
 * @param pContinent the new continent it belongs to
 */
void Territory::setContinent(Continent *pContinent) {
  continent = pContinent;
}

/**
 * List the name of adjacent territories
 * @return the name of adjacent territories
 */
string Territory::listNameAndAdjacent() {
  auto str = name + ": ";
  for (auto territory : adjacentTerritories) {
    str += "* " + territory->getName() + " ";
  }
  return str;
}

/**
 * Add a territory as one of this territory's adjacent territories
 * @param territory the new adjacent territory
 */
void Territory::addAdjacent(Territory *territory) {
  if (std::find(adjacentTerritories.begin(), adjacentTerritories.end(),
                territory) != adjacentTerritories.end()) {
    throw runtime_error(name + " is already adjacent to " + territory->name);
  }
  adjacentTerritories.push_back(territory);
}

/**
 * Get a large description of the territory
 * @return String with the long description of the territory
 */
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
/**
 * Constructor for a continent
 * @param name the name of the continent
 * @param bonus the bonus rewarded from owning this continent
 */
Continent::Continent(string name, int bonus)
    : name(Utils::trim(name)), bonus(bonus) {}

/**
 * Copy constructor for a continent
 * @param orgContinent The continent that will be copied
 */
Continent::Continent(const Continent &orgContinent) {
  this->name = orgContinent.name;
  this->bonus = orgContinent.bonus;
  this->territories = orgContinent.territories;
}

/**
 * Destructor for the continent
 */
Continent::~Continent() = default;

/**
 * = operator overload for a continent
 * @param continent The continent that this continent will be equal to
 * @return new Continent based on the copy constructor
 */
Continent &Continent::operator=(const Continent &continent) = default;

/**
 * Function to print to console the content's of a continent
 * @param os The OSStream we will be appending to
 * @param hand  The continent that will be examined
 * @return A string containing the content's of the continent
 */
std::ostream &operator<<(ostream &os, const Continent &continent) {
  os << "Name of continent: " << continent.name << "\n"
     << "Bonus: " << continent.bonus << "\n";
  return os;
}

/**
 * Get all territories a continent owns
 * @return
 */
vector<Territory *> Continent::getTerritories() {
  return territories;
}

/**
 * Get the name of a continent
 * @return  the name of the continent
 */
string Continent::getName() {
  return name;
}

/**
 * Get the bonus a continent has
 * @return the bonus a continent has
 */
int Continent::getBonus() const {
  return bonus;
}

/**
 * Add a territory to a continent
 * @param territory The territory that will be added
 */
void Continent::addTerritoryToContinent(Territory *territory) {
  territories.push_back(territory);
}

/**
 * Find the owner of a continent if there is any
 * @return the owner of a continent if there is any
 */
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

/**
 * Find a territory by ID
 * @param id the id used to look for a territory
 * @return The territory if there exists one with this ID
 */
Territory *Map::findById(int id) const {
  for (auto territory : territories) {
    if (territory->getId() == id) {
      return territory;
    }
  }
  return nullptr;
}

// ------------------ Maps ------------------------

/**
 * Constructor for a map
 */
Map::Map() = default;

/**
 * Copy constructor for a map
 * @param orgMap The map that will be copied
 */
Map::Map(const Map &orgMap) {
  this->territories = orgMap.territories;
  this->continents = orgMap.continents;
}

/**
 * Destructor for a map
 */
Map::~Map() {
  for (auto p : continents) {
    delete p;
  }
  for (auto p : territories) {
    delete p;
  }
}

/**
 * = operator overload for a map
 * @param continent The map that this map will be equal to
 * @return new map based on the copy constructor
 */
Map &Map::operator=(const Map &map) {
  this->territories = map.territories;
  this->continents = map.continents;
  return *this;
}

/**
 * Function to print to console the content's of a map
 * @param os The OSStream we will be appending to
 * @param hand  The map that will be examined
 * @return A string containing the content's of the map
 */
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

/**
 * Get all territories within a map
 * @return all territories within a map
 */
vector<Territory *> Map::getAllTerritories() {
  return territories;
}

/**
 * Get all continents within a map
 * @return all continents within a map
 */
vector<Continent *> Map::getContinents() {
  return continents;
}

/**
 * Add a territory to a map
 * @param terr The territory that will be added to the map
 */
void Map::addTerritoryToMap(Territory *terr) {
  territories.push_back(terr);
}

/**
 * Add a continent to a map
 * @param terr The continent that will be added to the map
 */
void Map::addContinent(Continent *continent) {
  continents.push_back(continent);
}
/**
 * Reset the visited status of a territory
 */
void Map::resetTerr() {
  for (auto &territory : territories) {
    if (territory->visited) {
      territory->visited = false;
    }
  }
}

/**
 * Verify which maps are isolated
 */
void Map::assertConnected() {
  resetTerr();

  auto previous = 0;

  for (auto &territory : territories) {
    if (territory->visited)
      continue;

    territory->visited = true;

    auto visited = traverseTerr(territory, previous);

    if (previous && visited != previous) {
      throw runtime_error("Territory " + territory->getName() +
                          " is isolated from the rest of the map!");
    }

    previous = visited;
  }
}

/**
 * Traverse the entire map
 * @param territory The starting territory
 * @param visited the count of the territories that have been visited
 * @return the number of territories visited
 */
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

/**
 * Verify if a territory can be reached within each continents
 */
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
      throw runtime_error("Territory " + territory->getName() +
                          " cannot be reached from within its continent!");

    t:;
    }
  }
}

/**
 * Verify that each territory belongs to only one continent
 */
void Map::assertEachTerritoryHasUniqueContinent() {
  map<string, string> listOfContinents;
  for (auto continent : continents) {
    for (auto &terr : continent->getTerritories())
      if (listOfContinents.count(terr->getName()) > 0) {
        throw runtime_error("Territory " + terr->getName() +
                            " appears in more than one continent");
      } else {
        listOfContinents[terr->getName()] = terr->getContinent()->getName();
      }
  }
}

/**
 * Validate the map
 * @return boolean whether or not the map is valid
 */
bool Map::validate() {
  // Prof says that one-way adjacencies are allowed
  //  assertEveryEdgeIsTwoWay();
  assertConnected();
  assertSubgraphConnected();
  assertEveryTerritoryHasContinent();
  assertEachTerritoryHasUniqueContinent();
  return true;
}

/**
 * Find a continent by name
 * @param continentName  the name of the continent
 * @return The continent
 */
Continent *Map::findContinentByName(const string &continentName) {
  auto trimmed = Utils::trim(continentName);
  for (auto continent : continents) {
    if (Utils::isEqualLowercase(continent->getName(), trimmed)) {
      return continent;
    }
  }
  return nullptr;
}

/**
 * Find a territory by name
 * @param territoryName  the name of the territory
 * @return the territory
 */
Territory *Map::findTerritoryByName(const string &territoryName) {
  auto trimmed = Utils::trim(territoryName);
  for (auto territory : territories) {
    if (Utils::isEqualLowercase(territory->getName(), trimmed)) {
      return territory;
    }
  }
  return nullptr;
}

/**
 * Verify if every edge is two way
 */
void Map::assertEveryEdgeIsTwoWay() {
  for (auto t : territories) {
    for (auto adj : t->getAdjTerritories()) {
      auto adjacents = adj->getAdjTerritories();
      if (std::find(adjacents.begin(), adjacents.end(), t) == adjacents.end()) {
        throw runtime_error("Territory " + t->getName() + " is adjacent to " +
                            adj->getName() + ", but it's not adjacent back!");
      }
    }
  }
}

/**
 * Verify all continents that are owned by a specific player
 * @return
 */
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

/**
 * Ask user for input to find a territory
 * @param cancelable boolean to see if this action is cancelable
 * @return The territory if it has been found
 */
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
      cout << "Your input did not correspond to any territory. Try again"
           << endl;
      continue;
    }

    return territory;
  }
  throw Utils::CancelledInputException();
}

/**
 * Ask user for input to find a territory
 * @param inputRequest The input the user asked for
 * @param cancelable boolean to see if this action is cancelable
 * @return Territory user looked for
 */
Territory *Map::getInputTerritory(const string &inputRequest, bool cancelable) {
  cout << inputRequest;
  return getInputTerritory(cancelable);
}

/**
 * Find a territory based on a user input
 * @param input the user input
 * @return the territory
 */
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

/**
 * Verify that every territory belongs to a continent
 */
void Map::assertEveryTerritoryHasContinent() {
  for (auto t : territories) {
    if (!t->getContinent()) {
      throw runtime_error(
          "Territory " + t->getName() +
          " does not have a continent, was it initialised properly?");
    }
  }
}

/**
 * Read a map file and import it
 * @param path The path to the map file
 * @return A loaded map object
 */
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
    delete map;
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
      delete map;
      throw runtime_error("Territory " + name + " references a continent " +
                          continentName + " that does not exist!");
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

  try {
    map->validate();
  } catch (runtime_error &e) {
    delete map;
    throw e;
  }

  auto rng = std::default_random_engine();
  rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  std::shuffle(map->territories.begin(), map->territories.end(), rng);

  return map;
}
