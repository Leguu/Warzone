//
// Created by Legu on 2022-09-13.
//

#include "Map.h"

#include <utility>

Continent::Continent(std::string name, int bonus) : name(std::move(name)), bonus(bonus) {}

Player *Continent::owner() {
    // TODO: Calculate the owner
    return nullptr;
}

int Map::idIncrement = 0;

// TODO
const Map *MapLoader::importMap(const std::string &path) {
    auto allTerritories = std::vector<Territory *>{};

    auto expel = new Continent("Expel", 4);

    auto arlia = new Territory("Arlia");
    arlia->continent = expel;
    auto krosse = new Territory("Krosse");
    krosse->continent = expel;

    std::vector<Continent *> continents{expel};

    allTerritories.push_back(arlia);
    allTerritories.push_back(krosse);

    expel->territories = allTerritories;

    auto map = new Map(continents, allTerritories);

    // Make sure to throw an exception if the map is invalid.

    return map;
}

Territory::Territory(std::string name) : name(std::move(name)) {}

Territory *Map::findById(int id) const {
    for (auto territory: territories) {
        if (territory->id == id) {
            return territory;
        }
    }
    return nullptr;
}

Map::Map(const std::vector<Continent *> &continents, const std::vector<Territory *> &territories) : continents(
        continents), territories(territories) {}