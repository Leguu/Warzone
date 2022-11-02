#include "GameEngine/GameEngine.h"
#include "Cards/Card.h"
#include "Orders/OrdersDriver.cpp"
#include "Player/PlayerDriver.cpp"
#include "Map/MapDriver.cpp"
#include "Cards/CardsDriver.cpp"
#include "GameEngine/GameEngineDriver.cpp"
#include "Logging/LoggingObserverDriver.cpp"
#include "Logging/LogObserver.h"
#include "Utils/Utils.h"

int main() {
    auto log = new LogObserver();
    while (true) {
        auto
                input = Utils::getInputString(
                "What do you want to test? cards, map, orders, players, game. Type quit to quit");

        if (Utils::isEqualLowercase(input, "cards")) {
            testCards();
        } else if (Utils::isEqualLowercase(input, "map")) {
            testLoadMaps();
        } else if (Utils::isEqualLowercase(input, "orders")) {
            testOrdersLists();
        } else if (Utils::isEqualLowercase(input, "players")) {
            testPlayers();
        } else if (Utils::isEqualLowercase(input, "logging")) {
            testLoggingObserver();
        } else if (Utils::isEqualLowercase(input, "game")) {
            auto ge = new GameEngine();
            ge->startupPhase();
        } else if (Utils::isEqualLowercase(input, "quit")) {
            break;
        } else {
            cout << "Unrecognised command" << endl;
        }
    }
}
