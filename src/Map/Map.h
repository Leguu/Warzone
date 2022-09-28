#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <memory>
#include "../Utils/Utils.h"

using std::string;
using std::vector;

class Player;
class Continent;

extern bool debug;

class Territory {
public:

  // Default constructor
  Territory();

  // Constructor with necessary parameters
  Territory(string territory, Continent *continent);

  explicit inline Territory(string name) : name(std::move(name)) {}

  // Copy constructor
  Territory(const Territory &orgTerritory);

  // Destructor
  virtual ~Territory();

  string longDescription();

  // Assignment operator
  Territory &operator=(const Territory &territory);

  // Insertion operator
  friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

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
  /// The ids for allTerritories need to be globally unique, so this can be static.
  static int idIncrement;
private:
  vector<Territory *> adjacentTerritories;
  // Variables
  int id = idIncrement++;
  string name;
  Continent *continent = nullptr;
  int armies{0};
  Player *owner = nullptr;
};

class Continent {
public:

  // Default constructor
  Continent();

  // Constructor with necessary name and optional bonus
  Continent(string name, int bonus);

  Continent(string name, int armies, vector<Territory *>);

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
  int bonus = 0;
};

class Map {
public:
  // Default Constructors
  Map();

  // Constructor with necessary parameters
  Map(string name, vector<Continent *> continents);

  Map(string name, vector<Territory *> territories, vector<Continent *> continents);

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

  void setAllTerritories(vector<Territory *>);

  vector<Continent *> getContinents();

  void addTerritoryToMap(Territory *);

  // A method to add a continent
  void addContinent(Continent *continent);

  Continent *findContinentByName(const string &continentName);

  Territory *findTerritoryByName(const string &continentName);

  // A method to connect territories
  static void addEdge(Territory *source, Territory *dest);

  bool allContinentsOwned();

  bool validate();

  [[nodiscard]] Territory *findById(int id) const;

  // private properties for Map class
private:
  string name;
  vector<Continent *> continents;
  vector<Territory *> territories;

  void resetTerr();

  bool isConnected();
  int traverseTerr(Territory *territory, int visited);

  bool isSubgraphConnected();
  int traverseSubgraph(Territory *territory, const string &continent, int visited);

  bool isUniqueContinent();

  bool assertEveryEdgeIsTwoWay();

};

class MapLoader {
public:
  static Map *importMap(const string &path) noexcept(false);

private:
  inline MapLoader() = default;
};

#endif //WARZONE_MAP_H
