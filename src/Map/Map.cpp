#include "Map.h"
#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <iostream>

Continent::Continent() : bonus(0) {}

Continent::Continent(std::string name, int bonus) : name(std::move(name)), bonus(bonus) {}

Continent::Continent(string name, vector<Territory *> territories, int bonus)
        : name(std::move(name)), ownedTerritories(std::move(territories)), bonus(bonus) {}

Continent::Continent(const Continent &orgContinent) {
    this->name = orgContinent.name;
    this->bonus = orgContinent.bonus;
    this->ownedTerritories = orgContinent.ownedTerritories;
}

// Should we go with intellisense and make = default?
Continent &Continent::operator=(const Continent &continent) {
    this->name = continent.name;
    this->bonus = continent.bonus;
    this->ownedTerritories = continent.ownedTerritories;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    std::cout << "Name of continent: " << continent.name << "\n"
              << "Bonus: " << continent.bonus << std::endl;
}

Continent::~Continent() {
    for (auto p : ownedTerritories) {
        delete p;
    }
}

Player *Continent::owner() {
    return nullptr;
}

void Continent::addTerritory(Territory *territory) {
    ownedTerritories.push_back(territory);
}

/*
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
*/


int Territory::idIncrement = 0;

Territory::Territory() {

}

Territory::Territory(std::string name, std::string continentName)
: name(std::move(name)), continentName(std::move(continentName)) {}

Territory::Territory(const Territory &orgTerritory) {
    this->id = orgTerritory.id;
    this->name = orgTerritory.name;
    this->continentName = orgTerritory.continentName;
    this->owner = orgTerritory.owner;
    this->adjacentTerritories = orgTerritory.adjacentTerritories;
    this->armies = orgTerritory.armies;

}

Territory &Territory::operator=(const Territory &territory) {
    this->id = territory.id;
    this->name = territory.name;
    this->continentName = territory.continentName;
    this->owner = territory.owner;
    this->adjacentTerritories = territory.adjacentTerritories;
    this->armies = territory.armies;
    return *this;
}


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

Territory::~Territory() {

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
