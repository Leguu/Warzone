//
// Created by Legu on 2022-09-13.
//

#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

class Territory;

#include <string>
#include "../Player/Player.h"

class Continent {
public:
    const std::string name;
    const int bonus;

    Player *owner();

    Continent(std::string name, int bonus);

protected:
    std::vector<Territory *> territories = {};

    friend class MapLoader;
};

class Map {
public:
    const std::vector<Continent *> continents;
    const std::vector<Territory *> territories;

    Map(const std::vector<Continent *> &continents, const std::vector<Territory *> &territories);

    [[nodiscard]] Territory *findById(int id) const;

private:
    static int idIncrement;

    friend class MapLoader;

    friend class Territory;
};

class Territory {
public:
    const int id = Map::idIncrement++;
    const std::string name;

    explicit Territory(std::string name);

    Player *owner = nullptr;
    int armies = 10;

protected:
    const std::vector<Territory *> adjacentTerritories;
    const Continent *continent = nullptr;

    friend class MapLoader;
};

class MapLoader {
public:
    const static Map *importMap(const std::string &path);
};

#endif //WARZONE_MAP_H
