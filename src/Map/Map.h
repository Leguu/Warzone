#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

class Territory;

#include <string>
#include <ostream>
#include <vector>
#include "../Player/Player.h"

using std::string;
using std::vector;

class Territory {
public:
    int id = territoryIdIncrement++;
    string name;
    // I feel like the continent name has no use
    string continentName;
    Player *owner = nullptr;
    vector<Territory *> adjacentTerritories;
    bool visited;
    int armies = 10;

    // Default constructor
    Territory();

    // Constructor with necessary parameters
    Territory( string name, string continentName);

    // Copy constructor
    Territory(const Territory &orgTerritory);

    // Destructor
    virtual ~Territory();

    // Assignment operator
    Territory &operator=(const Territory &territory);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    // To string method
   // [[nodiscard]] string toString() const;


protected:
    /// Global variable for assigning territory ids.
    /// The ids for allTerritories need to be globally unique, so this can be static.
    static int territoryIdIncrement;

    // Todo is this really necessary? Not for now, will come back later
   // Continent *continent = nullptr;

    friend class MapLoader;
};


class Continent {
public:
     int id = continentIdIncrement++;
     string name;
     int bonus;
    vector<Territory*> ownedTerritories;

    // TODO: Calculate the owner
//    Player *owner();

    // Default constructor
    Continent();

    // Constructor with necessary name and optional bonus
    explicit Continent(string name, int bonus );

    // Constructor with name, list of pointers to territories and an optional bonus
    Continent(string name, vector<Territory*> territories, int bonus);

    // Copy constructor
    Continent(const Continent &orgContinent);

    // Destructor
    virtual ~Continent();


    // Assigment constructor
    Continent &operator=(const Continent &continent);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);


    // A method to add a territory to a continent
    void addTerritory(Territory* territory);

protected:
    friend class MapLoader;
    static int continentIdIncrement;
};


class Map {
public:
    // TODO Map owns these pointers, they should be destructed properly.
     string name;
     std::vector<Continent *> continents;
     std::vector<Territory *> territories;

    // Default Constructors
    Map();

    // Constructor with necessary parameters
    Map(std::string name, std::vector<Continent *> continents, std::vector<Territory *> territories);

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

    // A method to add a territory
    void addTerritory(Territory* territory);

    // A method to connect territories
    void connectNeighbors (Territory* firstTerr, Territory* secondTerr);

    // A method to add a continent
    void addContinent (Continent* continent);

    // A method to add a territory to a continent
    void addTerritoryToContinent (Continent* continent, Territory* territory);

    // A method to check if a territory has one continent
    bool uniqueContinent();

    // A method to check if all territories are connected
    void DfsTraverseTerritories(Territory * tp);

    // Reset the visitations and increase the vector if more are added
    void resetVisitedTerritories();

    // A method to check if continent subgraphs are connected
    bool areContinentsConnected();

    // A method that validates maps
    bool validate();

    /// Figure out whether every continents have an owner?
    inline bool allContinentsOwned() { return false; }

    [[nodiscard]] Territory *findById(int id) const;

    // TODO: find by name?


private:
    friend class MapLoader;
};

/// Exception for when the file cannot be read, or it's garbled
class InvalidMapFileException : public std::runtime_error {
public:
    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

/// Exception for when the map does not follow the game rules
class InvalidGameMapException : public std::runtime_error {
public:
    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

class MapLoader {
public:
    // TODO
    // TODO Throws InvalidMapFileException or InvalidGameMapException
    // Throw when:
    // - territories doubly adjacent
    // - territories are adjacent to at least one thing
    // - territories have a continent
    // - continents have at least one territory
    static std::unique_ptr<Map> importMap(const string &path) noexcept(false);
};

#endif //WARZONE_MAP_H
