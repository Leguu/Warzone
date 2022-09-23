#include <string>
#include <vector>
#include <iostream>
#include "Map.h"


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
    os << "ID of territory: " << territory.id << "\n"
       << "Name of territory: " << territory.name << "\n"
       << "Continent it belongs to: " << territory.continentName << "\n"
       << "Owner of territory: " << territory.owner << "\n"
       << "Number of armies: " << territory.armies << "\n"
       << "List of neighbors" << "\n";

    for (auto p : territory.adjacentTerritories) {
        os << p->name << "\n";
    }

    return os;
}

/*std::string Territory::toString() const {
    std::string str;
    str += "[" + std::to_string(id) + "] ";
    str += name;
    if (owner) {
        str += " (" + std::to_string(armies) + ", " + owner->name + ")";
    } else {
        str += " (" + std::to_string(armies) + ")";
    }
    return str;
}*/

Territory::~Territory() {
    delete owner;
    for (auto p: adjacentTerritories) {
        delete p;
    }
}

Continent::Continent() : bonus(0) {}

Continent::Continent(std::string name, int bonus) : name(std::move(name)), bonus(bonus) {}

Continent::Continent(string name, vector<Territory *> territories, int bonus)
        : name(std::move(name)), ownedTerritories(std::move(territories)), bonus(bonus) {}

Continent::Continent(const Continent &orgContinent) {
    this->name = orgContinent.name;
    this->bonus = orgContinent.bonus;
    this->ownedTerritories = orgContinent.ownedTerritories;
}

Continent &Continent::operator=(const Continent &continent) {
    this->name = continent.name;
    this->bonus = continent.bonus;
    this->ownedTerritories = continent.ownedTerritories;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    os << "Name of continent: " << continent.name << "\n"
              << "Bonus: " << continent.bonus << "\n"
              << "Owned territories: " << "\n";

    for (auto p : continent.ownedTerritories) {
        os << p->name << "\n";
    }
    return os;
}

Continent::~Continent() {
    for (auto p: ownedTerritories) {
        delete p;
    }

}

/*Player *Continent::owner() {
    return nullptr;
}*/

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

Territory *Map::findById(int id) const {
    for (auto territory: allTerritories) {
        if (territory->id == id) {
            return territory;
        }
    }
    return nullptr;
}

Map::Map() {}

Map::Map(std::string name, std::vector<Continent *> continents, std::vector<Territory *> territories) :
        name(std::move(name)), continents(std::move(
        continents)), allTerritories(std::move(territories)) {}

Map::Map(const Map &orgMap) {
    this->name = orgMap.name;
    this->continents = orgMap.continents;
    this->allTerritories = orgMap.allTerritories;
}

Map &Map::operator=(const Map &map) {
    this->name = map.name;
    this->continents = map.continents;
    this->allTerritories = map.allTerritories;
    return *this;
}


std::ostream &operator<<(std::ostream &os, const Map &map) {
    os << "Name of map:\n" << map.name << "\n"
        << "List of Continents in map:" << "\n";
    for (auto p : map.continents) {
        os << p->name << "\n";
    }

    os << "List of all territories in map" <<  "\n";
    for (auto p : map.allTerritories) {
        os << p->name << "\n";
    }
    return os;
}

Map::~Map() {
    for (auto p: continents) {
        delete p;
    }

    for (auto p: allTerritories) {
        delete p;
    }
}


const char *InvalidMapFileException::what() const noexcept {
    return "This map file has an invalid format or cannot be read.";
}

const char *InvalidGameMapException::what() const noexcept {
    return "This map file is unwinnable!";
}
