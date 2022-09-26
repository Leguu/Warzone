#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <memory>

using std::string;
using std::vector;

class Player;

extern bool debug;

class Territory {
public:

    // Default constructor
    Territory();

    // Constructor with necessary parameters
    Territory(string territory, string continent);

    Territory(string name, string continent, vector<Territory *> adjacentTerritories);

    // Copy constructor
    Territory(const Territory &orgTerritory);

    // Destructor
    virtual ~Territory();

    // Assignment operator
    Territory &operator=(const Territory &territory);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    // Method to get neighbors
    vector<Territory *> getAdjTerritories() const;

    vector<Territory *> adjacentTerritories;
    bool visited;

    // Getters and setters
    string getName();

    int getId() const;

    string getContinent();

    int getArmies() const;

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
    // Variables
    int id = idIncrement++;
    string name;
    string continent;
    int armies{0};
    Player *owner = nullptr;

    //  friend class MapLoader;
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

    string getName();

    int getBonus() const;

    // A method to add a territory to a continent
    void addTerritoryToContinent(Territory *territory);

private:
    vector<Territory *> territories;
    string name;
    int bonus;

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

    // A method to add a territory
    void addTerritoryToMap(string newName, const string &continent);

    void addTerritoryToMap(Territory *);

    // A method to add a continent
    void addContinent(Continent *continent);

    // A method to connect territories
    void addEdge(Territory *source, Territory *dest);

    void resetTerr();

    bool isConnected();
    int traverseTerr(Territory *territory, int visited);

    bool isSubgraphConnected();
    int traverseSubgraph(Territory *territory, const string& continent, int visited);

    bool isUniqueContinent();
    bool validate();

    /// Figure out whether every continents have an owner?
    inline bool allContinentsOwned() { return false; }

    [[nodiscard]] Territory *findById(int id) const;


///// Exception for when the file cannot be read, or it's garbled
//    class InvalidMapFileException : public std::runtime_error {
//        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
//    };
//
///// Exception for when the map does not follow the game rules
//    class InvalidGameMapException : public std::runtime_error {
//        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
//    };

    // private properties for Map class
private:
    string name;
    vector<Continent *> continents;
    vector<Territory *> territories;

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

    string mapFile;

    // constructor
    MapLoader(string path);

    // copy constructor
    MapLoader(const MapLoader &orgMapLoader);

    // destructor
    virtual ~MapLoader();

    // Assignment operator
    MapLoader &operator=(const MapLoader &mapLoader);

    // Insertion operator
    friend std::ostream &operator<<(std::ostream &os, const MapLoader &mapLoader);

     static Map* importMap(const string &path) noexcept(false);

    // parse the .map file
    bool parse();

    // create a Map obj
    Map *createMap();

};

#endif //WARZONE_MAP_H
