#include "Player.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
vector<Territory *> Player::toAttack() const {
  auto adjacentEnemies = vector<Territory *>();
  for (auto t : ownedTerritories) {
    for (auto adj : t->getAdjTerritories()) {
      if (adj->getOwner() && adj->getOwner() != this) {
        adjacentEnemies.push_back(adj);
      }
    }
  }
  return adjacentEnemies;
}

/**
 * Find all territories the player owns
 * @return all territories the player owns
 */

vector<Territory *> Player::toDefend() const {
  return ownedTerritories;
}

/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
vector<Territory *> Player::getAdjacentEnemyTerritories() {
  vector<Territory *> enemyTerritoriesAdjacent;
  for (auto friendlyTerritory : this->ownedTerritories) {
    for (auto adjacentTerritory : friendlyTerritory->getAdjTerritories()) {
      if (adjacentTerritory->getOwner() != this && adjacentTerritory->getOwner() &&
          find(enemyTerritoriesAdjacent.begin(), enemyTerritoriesAdjacent.end(), adjacentTerritory) ==
              enemyTerritoriesAdjacent.end()) {
        enemyTerritoriesAdjacent.push_back(adjacentTerritory);
      }
    }
  }
  return enemyTerritoriesAdjacent;
}

/**
 * Function to print to console a player's information
 * @param os The OSStream we will be appending to
 * @param hand  The player we will be examining
 * @return A string containing a player's information
 */
std::ostream &operator<<(std::ostream &os, const Player &player) {
  os << player.name << endl;
  return os;
}

/**
 * Draw a card from the deck
 */
void Player::drawFromDeck() const {
  auto ge = GameEngine::instance();

  auto card = ge->deck->draw();
  hand->cards.push_back(card);
}

/**
 * Issue all your orders while its your turn
 */
void Player::issueOrder() {
  auto ge = GameEngine::instance();
  while (true) {
    auto input = Utils::toLowercase(Utils::trim(Utils::getInputString()));

    auto tokenized = Utils::tokenizer(input, ' ');

    try {
      if (input == "help") {
        cout << GameEngine::helpText << endl;
      } else if (input == "list hand") {
        cout << *hand << endl;
      } else if (input == "list orders") {
        cout << *orders << endl;
      } else if (input == "advance") {
        issueAdvanceOrder();
      } else if (input == "deploy") {
        issueDeployOrder();
      } else if (tokenized[0] == "play") {
        auto cardName = input.substr(5);

//        play(cardName);
      } else if (input == "map") {
        cout << *ge->map << endl;
      } else if (tokenized[0] == "territory") {
        auto territoryName = input.substr(10);
        auto territory = ge->map->findTerritory(territoryName);

        if (!territory) {
          cout << "Couldn't find that territory, sorry" << endl;
          continue;
        }

        cout << territory->longDescription() << endl;
      } else if (input == "done") {
        return;
      }
    } catch (Utils::CancelledInputException &) {
      cout << "Cancelled that command." << endl;
    }
  }
}

/**
 * Issue an advance order
 */
void Player::issueAdvanceOrder() {
  auto ge = GameEngine::instance();
  cout << "You can advance from: ";
  for (auto t : ownedTerritories) {
    cout << "* " << t->getName() << " ";
  }
  cout << endl;
  auto source = ge->map->getInputTerritory("What's your source territory?", true);

  cout << "Where would you like to advance to? You can choose from: ";
  for (auto t : source->getAdjTerritories()) {
    cout << "* " << *t << " ";
  }
  Territory *target;
  while (true) {
    cout << endl;
    target = ge->map->getInputTerritory(true);

    if (std::find(source->getAdjTerritories().begin(), source->getAdjTerritories().end(), target)
        == source->getAdjTerritories().end()) {
      cout << "That territory is not adjacent to your chosen territory, sorry.";
      continue;
    }

    break;
  }

  int armies;
  while (true) {
    armies = Utils::getInputInt(
        "How many armies would you like to send? Territory " + source->getName() + " has "
            + std::to_string(source->getArmies())
            + " territories...", true);

    if (armies <= 0) {
      cout << "You have to choose a number of armies greater than 0." << endl;
      continue;
    } else if (armies > source->getArmies()) {
      cout << "Your source territory does not have that many armies!" << endl;
      continue;
    }

    break;
  }

  orders->push(new AdvanceOrder(this, armies, source, target));

  cout << "Advance order issued." << endl;
}

/**
 * Issue a deploy order
 */
void Player::issueDeployOrder() {
  auto ge = GameEngine::instance();
  cout << "You can deploy to: ";
  for (auto t : ownedTerritories) {
    cout << "* " << t->getName() << " ";
  }
  cout << endl;
  Territory *target;
  while (true) {
    target = ge->map->getInputTerritory("What's your target territory?", true);

    if (target->getOwner() != this) {
      cout << "You can't deploy on another player's territory." << endl;
      continue;
    }

    break;
  }

  int armies;
  while (true) {
    armies = Utils::getInputInt(
        "How many armies would you like to send? You have " + std::to_string(reinforcements) + " reinforcements.",
        true);

    if (armies <= 0) {
      cout << "You have to choose a number of armies greater than 0." << endl;
      continue;
    }

    break;
  }

  orders->push(new DeployOrder(this, armies, target));

  cout << "Deploy order issued." << endl;
}

/**
 * Player constructor
 * @param name The name of the player
 */
Player::Player(string name) : name(std::move(name)), orders(new OrderList()) {
  this->hand = new Hand(this);
}
/**
 * Play destructor
 */
Player::~Player() = default;
