#include "Map.h"

#include <utility>

Continent::Continent(std::string name, int bonus) : name(std::move(name)), bonus(bonus) {}

Player *Continent::owner() {
    return nullptr;
}

int Territory::idIncrement = 0;

std::unique_ptr<Map> MapLoader::importMap(const std::string &path) {
    auto allTerritories = std::vector<Territory *>{};

    auto expel = new Continent("Expel", 4);

    auto arlia = new Territory("Arlia");
    arlia->continent = expel;
    auto krosse = new Territory("Krosse");
    krosse->continent = expel;

    arlia->adjacentTerritories = {krosse};
    krosse->adjacentTerritories = {arlia};

    std::vector<Continent *> continents{expel};

    allTerritories.push_back(arlia);
    allTerritories.push_back(krosse);

    expel->territories = allTerritories;

    auto map = std::make_unique<Map>(continents, allTerritories);

    // Make sure to throw an exception if the map is invalid.

    return map;
}

Territory::Territory(std::string name) : name(std::move(name)) {}

std::ostream &operator<<(std::ostream &os, const Territory &territory) {
    os << territory.toString();
    return os;
}

std::string Territory::toString() const {
    std::string str;
    str += "[" + std::to_string(id) + "] ";
    str += name;
    if (owner) {
        str += " (" + std::to_string(armies) + ", " + owner->name + ")";
    } else {
        str += " (" + std::to_string(armies) + ")";
    }
    return str;
}

Territory *Map::findById(int id) const {
    for (auto territory: allTerritories) {
        if (territory->id == id) {
            return territory;
        }
    }
    return nullptr;
}

Map::Map(std::vector<Continent *> continents, std::vector<Territory *> territories) : continents(std::move(
        continents)), allTerritories(std::move(territories)) {}

std::ostream &operator<<(std::ostream &os, const Map &map) {
    return os;
}

Map::~Map() {
}

const char *InvalidMapFileException::what() const noexcept {
    return "This map file has an invalid format or cannot be read.";
}

const char *InvalidGameMapException::what() const noexcept {
    return "This map file is unwinnable!";
}
