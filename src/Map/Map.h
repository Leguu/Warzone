#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

class Territory;

#include <string>
#include <ostream>
#include <vector>
#include "../Player/Player.h"

using std::string;
using std::vector;


class Continent {
public:
    const string name;
    const int bonus;
    vector<Territory *> ownedTerritories;

    // TODO: Calculate the owner
    Player *owner();

    // Default constructor
    Continent();

    // Constructor with necessary name and optional bonus
    Continent(string name, int bonus = 0);

    // Constructor with name, list of pointers to territories and an optional bonus
    Continent(string name, vector<Territory *> territories, int bonus = 0);

    // Copy constructor
    Continent(const Continent &orgContinent);

    // Assigment constructor
    Continent &operator=(const Continent &continent);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

    // Destructor
    virtual ~Continent();

protected:
    friend class MapLoader;
};

class Territory {
public:
    const int id = idIncrement++;
    const string name;
    const string continentName;
    Player *owner = nullptr;
    vector<Territory *> adjacentTerritories;
    // int armies = 10;

    // Default constructor
    Territory();

    // Constructor with necessary parameters
    Territory(int id, string name, string continentName);

    // Copy constructor
    Territory(const Territory &orgTerritory);

    // Assignment operator
    Territory &operator=(const Territory &territory);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    // Destructor
    virtual ~Territory();

    // To string method
    [[nodiscard]] string toString() const;


protected:
    /// Global variable for assigning territory ids.
    /// The ids for allTerritories need to be globally unique, so this can be static.
    static int idIncrement;

    /// Todo is this really necessary?
    Continent *continent = nullptr;

    friend class MapLoader;
};

class Map {
public:
    // TODO Map owns these pointers, they should be destructed properly.
    // Could probably be unique_ptrs to make that clear
    // no unique pointers will be used
    const string name;
    const std::vector<Continent *> continents;
    const std::vector<Territory *> allTerritories;

    // Default Constructors
    Map();

    // Constructor with necessary parameters
    Map(std::vector<Continent *> continents, std::vector<Territory *> territories);

    // Copy Constructor
    Map(const Map &orgMap);

    // Assignment operator
    Map &operator=(const Map &map);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Map &map);

    /// Figure out whether every continents have an owner?
    inline bool allContinentsOwned() { return false; }

    [[nodiscard]] Territory *findById(int id) const;

    // TODO: find by name?

    // TODO: Implement the tostring
    // Format:
    // Territory: adjacent, adjacent, adjacent
    // Territory1: adjacent, adjacent, adjacent
    // Territory2: adjacent, adjacent, adjacent
    friend std::ostream &operator<<(std::ostream &os, const Map &map);

    // todo
    virtual ~Map();

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
