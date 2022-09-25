#ifndef WARZONE_MAP_H
#define WARZONE_MAP_H


#include <string>
#include <ostream>
#include <vector>
#include <set>

using std::string;
using std::vector;

class Map {
public:
    class Territory;
    class Continent;


    class Territory {
    public:
        // Default constructor
        Territory();

        // Constructor with necessary parameters
        Territory(int id, string name, int continentID);

        // Copy constructor
        Territory(const Territory &orgTerritory);

        // Destructor
        virtual ~Territory();

        // Assignment operator
        Territory &operator=(const Territory &territory);

        // Insertion operator
        friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

        // Method to get neighbors
        inline vector<Territory *> *getAdjTerritories();

        // Getters and setters
        inline string getTerritoryName();

        inline int getOwner();

        inline void setOwner(int id);

        inline int getArmies();

        inline void setArmies(int armies);

        inline int getContinentID();

        inline int getTerritoryID();

        // To string method
        // [[nodiscard]] string toString() const;

    private:
        /// Global variable for assigning territory ids.
        /// The ids for allTerritories need to be globally unique, so this can be static.
        // static int territoryIdIncrement;

        int *pID{};
        string *pName{};
        int *pContinentID{};
        int *pOwner = nullptr;
        vector<Territory *> *pAdjacentTerritories{};
        int *pArmies{};

        //  bool visited = false;
        //  friend class MapLoader;
    };


    class Continent {
    public:

        // Default constructor
        Continent();

        // Constructor with necessary name and optional bonus
        Continent(string name, int bonus);

        // Copy constructor
        Continent(const Map::Continent &orgContinent);

        // Destructor
        virtual ~Continent();

        // Assigment constructor
        Continent &operator=(const Continent &continent);

        // Insertion operator
        friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

        // Getters and setters

        inline vector<Territory *> *getOwnedTerritories();

        inline string getName();

        inline int getBonus();

        // A method to add a territory to a continent
        inline void addTerritory(Territory *territory);

    private:
        vector<Territory *> *pOwnedTerritories{};
        string *pName{};
        int *pBonus{};

    };


    // Default Constructors
    Map();

    // Constructor with necessary parameters
    // Note that the vector in a vector stores Continents that
    // Store pointers that point to territories it owns
    Map(string name, vector<vector<string>> continentsWithTerritories);

    Map(string &mapName, vector<Continent *> continents);

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
    inline vector<Territory *> *getAllTerritories();

    inline void setAllTerritories(vector<Territory *> *);

    inline vector<Continent *> *getContinents();

    // A method to add a territory
    Territory *addTerritory(int id, string name, int continent);

    // A method to connect territories
    void connectNeighbors(int source, int dest);

    // A traversal method using DFS
    static void DFS(std::set <string> *visitedTerritories, Territory *territory, bool test);

    // A method to check if all territories are connected
    static bool traverse(vector<Territory *> *startingPoint, bool isContinent);

    // A method to check both territories and continents
    bool validate();

    // A method to check if a territory has one continent
     bool uniqueContinent();

    // Print the map
    void toStringMap();


    /// Figure out whether every continents have an owner?
    inline bool allContinentsOwned() { return false; }

    [[nodiscard]] Territory *findById(int id) const;


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

        string* pFile;

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


       // static std::unique_ptr<Map> importMap(const string &path) noexcept(false);

        // parse the .map file
        bool parse();

        // create a Map obj
        Map *createMap();

    private:
        bool parseContinent(string line); // parse continents block in the .map file
        bool parseTerritory(string line); // parse territories block in the .map file
        void convertTerritoryToInt(); // change territory name to its index in vector
        int getTerritoryId(const string &name); // get territory index in the vector
        vector<string> getContinentsData(int id); // return struct continentsData
        vector<string> getTerritoriesData(int id); // return struct territoriesData
        vector<vector<string>> getBordersData(); // return struct bordersData

        vector<string> splitString(const string &line, char delim);
    };


    // private properties for Map class
private:
    string *pName;
    vector<Continent *> *pContinents;
    vector<Territory *> *pTerritories;


};

#endif //WARZONE_MAP_H
