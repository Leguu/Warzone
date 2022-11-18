#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

#include "../Utils/Utils.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;
using std::filesystem::directory_iterator;

class Player;

class Continent;

class Territory {
public:
  // Constructor with necessary parameters
  Territory(const string &territory, Continent *continent);

  explicit inline Territory(const string &name) : name(Utils::trim(name)) {}

  // Copy constructor
  Territory(const Territory &orgTerritory);

  // Destructor
  virtual ~Territory();

  string listNameAndAdjacent();

  string longDescription();

  // Assignment operator
  Territory &operator=(const Territory &territory);

  // Insertion operator
  friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

  bool operator==(const Territory &b) { return this->getId() == b.getId(); }

  // Method to get neighbors
  [[nodiscard]] vector<Territory *> getAdjTerritories() const;

  void addAdjacent(Territory *);

  bool visited = false;

  // Getters and setters
  string getName();

  [[nodiscard]] int getId() const;

  Continent *getContinent();

  void setContinent(Continent *continent);

  [[nodiscard]] int getArmies() const;

  void setArmies(int);

  Player *getOwner();

  void setOwner(Player *);

  // To string method
  [[nodiscard]] string toString() const;

protected:
  /// Global variable for assigning territory ids.
  /// The ids for allTerritories need to be globally unique, so this can be
  /// static.
  static int idIncrement;

private:
  vector<Territory *> adjacentTerritories;
  // Variables
  int id = idIncrement++;
  string name;
  Continent *continent = nullptr;
  int armies = 0;
  Player *owner = nullptr;

  friend class Map;
};

class Continent {
public:
  // Constructor with necessary name and optional bonus
  Continent(string name, int bonus);

  // Copy constructor
  Continent(const Continent &orgContinent);

  // Destructor
  virtual ~Continent();

  // Assigment constructor
  Continent &operator=(const Continent &continent);

  // Insertion operator
  friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

  // Getters and setters

  vector<Territory *> getTerritories();

  Player *owner();

  string getName();

  [[nodiscard]] int getBonus() const;

  // A method to add a territory to a continent
  void addTerritoryToContinent(Territory *territory);

private:
  vector<Territory *> territories;
  string name;
  int bonus;
};

class Map {
public:
  // Default Constructors
  Map();

  // Copy Constructor
  Map(const Map &orgMap);

  // Destructor
  virtual ~Map();

  // Assignment operator
  Map &operator=(const Map &map);

  // Insertion operator
  // Format:
  // Territory: adjacent, adjacent, adjacent
  // Territory1: adjacent, adjacent, adjacent
  // Territory2: adjacent, adjacent, adjacent
  friend std::ostream &operator<<(std::ostream &os, const Map &map);

  // Getters and setters
  vector<Territory *> getAllTerritories();

  vector<Continent *> getContinents();

  void addTerritoryToMap(Territory *);

  Territory *getInputTerritory(bool cancelable);

  Territory *getInputTerritory(const std::string &inputRequest);

  Territory *getInputTerritory(const std::string &inputRequest,
							   bool cancelable);

  // A method to add a continent
  void addContinent(Continent *continent);

  Continent *findContinentByName(const string &continentName);

  Territory *findTerritoryByName(const string &continentName);

  Territory *findTerritory(const string &name);

  bool validate();

  [[nodiscard]] Territory *findById(int id) const;

  // private properties for Map class
private:
  string name;
  vector<Continent *> continents;
  vector<Territory *> territories;

  void resetTerr();

  void assertConnected();

  int traverseTerr(Territory *territory, int visited);

  void assertSubgraphConnected();

  void assertEachTerritoryHasUniqueContinent();

  void assertEveryEdgeIsTwoWay();

  void assertEveryTerritoryHasContinent();

  friend class MapLoader;
};

class MapLoader {
public:
  static Map *importMap(const string &path) noexcept(false);

private:
  inline MapLoader() = default;
};

void testLoadMaps();

#endif // WARZONE_MAP_H
