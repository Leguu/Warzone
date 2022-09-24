#include <string>
#include <vector>
#include <iostream>
#include "Map.h"


int Territory::territoryIdIncrement = 0;
int Continent::continentIdIncrement = 0;

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
    this->visited = orgTerritory.visited;

}

Territory &Territory::operator=(const Territory &territory) {
    this->id = territory.id;
    this->name = territory.name;
    this->continentName = territory.continentName;
    this->owner = territory.owner;
    this->adjacentTerritories = territory.adjacentTerritories;
    this->armies = territory.armies;
    this->visited = territory.visited;
    return *this;
}


std::ostream &operator<<(std::ostream &os, const Territory &territory) {
    os << "ID of territory: " << territory.id << "\n"
       << "Name of territory: " << territory.name << "\n"
       << "Continent it belongs to: " << territory.continentName << "\n"
       << "Owner of territory: " << territory.owner << "\n"
       << "Number of armies: " << territory.armies << "\n"
       << "List of neighbors" << "\n";

    for (auto p: territory.adjacentTerritories) {
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
    this->id = orgContinent.id;
    this->name = orgContinent.name;
    this->bonus = orgContinent.bonus;
    this->ownedTerritories = orgContinent.ownedTerritories;
}

Continent &Continent::operator=(const Continent &continent) {
    this->id = continent.id;
    this->name = continent.name;
    this->bonus = continent.bonus;
    this->ownedTerritories = continent.ownedTerritories;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    os << "Name of continent: " << continent.name << "\n"
       << "Bonus: " << continent.bonus << "\n"
       << "Owned territories: " << "\n";

    for (auto p: continent.ownedTerritories) {
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
    for (auto territory: territories) {
        if (territory->id == id) {
            return territory;
        }
    }
    return nullptr;
}

Map::Map() {}

Map::Map(std::string name, std::vector<Continent *> continents, std::vector<Territory *> territories) :
        name(std::move(name)), continents(std::move(
        continents)), territories(std::move(territories)) {}

Map::Map(const Map &orgMap) {
    this->name = orgMap.name;
    this->continents = orgMap.continents;
    this->territories = orgMap.territories;
}

Map &Map::operator=(const Map &map) {
    this->name = map.name;
    this->continents = map.continents;
    this->territories = map.territories;
    return *this;
}


std::ostream &operator<<(std::ostream &os, const Map &map) {
    os << "Name of map:\n" << map.name << "\n"
       << "List of Continents in map:" << "\n";
    for (auto p: map.continents) {
        os << p->name << "\n";
    }

    // TODO remove last comma
    os << "List of all territories and their neighbors in map:" << "\n";
    for (auto p: map.territories) {
        os << p->name << ": ";
        for (auto pp: p->adjacentTerritories) {
            os << pp->name << ", ";
        }
        os << "\n";
    }
    return os;
}

Map::~Map() {
    for (auto p: continents) {
        for (auto pp: p->ownedTerritories) {
            delete pp;
        }
        delete p;
    }

    for (auto p: territories) {
        for (auto pp: p->adjacentTerritories) {
            delete pp;
        }
        delete p;
    }
}

void Map::addTerritory(Territory *territory) {
    territories.push_back(territory);
}

void Map::connectNeighbors(Territory *firstTerr, Territory *secondTerr) {
    firstTerr->adjacentTerritories.push_back(secondTerr);
    secondTerr->adjacentTerritories.push_back(firstTerr);
}

void Map::addContinent(Continent *continent) {
    continents.push_back(continent);
}

void Map::addTerritoryToContinent(Continent *continent, Territory *territory) {
    continent->ownedTerritories.push_back(territory);
}

bool Map::uniqueContinent() {

    int numOfContinents;

    for (auto p1: territories) {
        numOfContinents = 0;
        for (auto p2: continents) {
            for (auto p3: p2->ownedTerritories) {
                if (p3->id == p1->id)
                    numOfContinents++;
            }
        }
        if (numOfContinents != 1)
            return false;
    }

    return true;
}

void Map::resetVisitedTerritories() {
    for (auto p: territories) {
        p->visited = false;
    }
}

void Map::DfsTraverseTerritories(Territory *tp) {

    std::cout << "Started Traversing " << tp->name << std::endl;
    tp->visited = true;
    vector<Territory *> neighbors = tp->adjacentTerritories;

    for (size_t i = 0; i < neighbors.size(); i++) {
        if (!neighbors[i]->visited) {
            neighbors[i]->visited = true;
            std::cout << neighbors[i]->name << " has been visited!" << std::endl;
            DfsTraverseTerritories(neighbors[i]);
        }
    }
    std::cout << "Finished Traversing " << tp->name << std::endl;
}


const char *InvalidMapFileException::what() const noexcept {
    return "This map file has an invalid format or cannot be read.";
}

const char *InvalidGameMapException::what() const noexcept {
    return "This map file is unwinnable!";
}
