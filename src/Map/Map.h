#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

class Territory;

#include <string>
#include <ostream>
#include "../Player/Player.h"

class Continent {
public:
    const std::string name;
    const int bonus;

    // TODO: Calculate the owner
    Player *owner();

    Continent(std::string name, int bonus);

protected:
    /// TODO list of references? also for Territory::adjacentTerritories
    std::vector<Territory *> territories = {};

    friend class MapLoader;
};

class Territory {
public:
    const int id = idIncrement++;
    const std::string name;

    Player *owner = nullptr;
    int armies = 10;

    explicit Territory(std::string name);

    [[nodiscard]] std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

protected:
    /// Global variable for assigning territory ids.
    /// The ids for allTerritories need to be globally unique, so this can be static.
    static int idIncrement;

    std::vector<Territory *> adjacentTerritories;
    /// Todo is this really necessary?
    Continent *continent = nullptr;

    friend class MapLoader;
};

class Map {
public:
    // TODO Map owns these pointers, they should be destructed properly.
    // Could probably be unique_ptrs to make that clear
    const std::vector<Continent *> continents;
    const std::vector<Territory *> allTerritories;

    Map(std::vector<Continent *> continents, std::vector<Territory *> territories);

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
    static std::unique_ptr<Map> importMap(const std::string &path) noexcept(false);
};

#endif //WARZONE_MAP_H
