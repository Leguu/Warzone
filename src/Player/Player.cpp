#include <iostream>
#include <map>

#include "../GameEngine/GameEngine.h"
#include "Player.h"

/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
vector<Territory *> Player::getAdjacentEnemyTerritories() {
  vector<Territory *> enemyTerritoriesAdjacent;
  for (auto friendlyTerritory : this->ownedTerritories) {
    for (auto adjacentTerritory : friendlyTerritory->getAdjTerritories()) {
      if (adjacentTerritory->getOwner() != this &&
          adjacentTerritory->getOwner() &&
          find(enemyTerritoriesAdjacent.begin(), enemyTerritoriesAdjacent.end(),
               adjacentTerritory) == enemyTerritoriesAdjacent.end()) {
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
 * Player constructor
 * @param name The name of the player
 */
Player::Player(string name) : name(std::move(name)), orders(new OrderList()), strategy(new DefaultPlayerStrategy(this)) {
  this->hand = new Hand(this);
}

void Player::issueOrder(bool debugMode) {
  this->strategy->issueOrder(debugMode);
}

/**
 * Play destructor
 */
Player::~Player() = default;

/**
 * Exception for invalid card
 * @param arg The text that will be printed on error
 */
InvalidCardException::InvalidCardException(const std::string &arg)
    : runtime_error(arg) {}
PlayerStrategy::PlayerStrategy(Player *P) : p(P) {}

void DefaultPlayerStrategy::issueOrder(bool debugMode) {
  // Current Mechanism:
  // ------------------
  // 	1. Keep deploying until zero reinforcements are left. Random number each turn.
  //    2. If user has a card in their hand, they play it.
  //    3. If player has not advanced an order this round, they advance order.
  // NOTE: All orders that are issued follow random moves for now.

  if (p->reinforcementsAfterDeploy > 0) {
    issueDeployOrder(debugMode);
  } else if (!p->cardOrderIssued && !p->hand->cards.empty()) {
    issueCardOrder(debugMode);
    p->cardOrderIssued = true;
  } else if (!p->advanceOrderIssued) {
    issueAdvanceOrder(debugMode);
    p->advanceOrderIssued = true;
  }
}

/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
std::vector<std::pair<Territory *, Territory *>> DefaultPlayerStrategy::toAttack() const {
  auto adjacentEnemies = std::vector<std::pair<Territory *, Territory *>>();
  for (auto t : p->ownedTerritories) {
    for (auto adj : t->getAdjTerritories()) {
      if (adj->getOwner() && adj->getOwner() != p) {
        adjacentEnemies.emplace_back(adj, t);
      }
    }
  }

  return adjacentEnemies;
}

/**
 * Find all territories the player owns
 * @return all territories the player owns
 */

vector<Territory *> DefaultPlayerStrategy::toDefend() const { return p->ownedTerritories; }

void DefaultPlayerStrategy::issueDeployOrder(bool debugMode) {
  Territory *target = Utils::accessRandomElement(toDefend());
  int armies = p->reinforcementsAfterDeploy == 1
               ? 1
               : Utils::randomNumberInRange(1, p->reinforcementsAfterDeploy);

  if (debugMode)
    cout << "Issued Deploy Order: " << armies
         << " units to " + target->getContinent()->getName() << endl;

  p->orders->push(new DeployOrder(p, armies, target));
  p->reinforcementsAfterDeploy -= armies;
}

void DefaultPlayerStrategy::issueCardOrder(bool debugMode) {
  auto randomCardName = p->hand->cards[0]->name;

  debugMode ? p->hand->debugPlay(randomCardName)
            : p->hand->play(randomCardName);

  std::map<std::string, int> cardNameMap = {
      {"Bomb", 0},
      {"Blockade", 1},
      {"Airlift", 2},
      {"NegotiateCard", 3}};

  int cardIndex = (cardNameMap.count(randomCardName) > 0)
                  ? (*cardNameMap.find(randomCardName)).second
                  : -1;

  switch (cardIndex) {
  case 0: {
    std::pair<Territory *, Territory *> attack =
        Utils::accessRandomPair(toAttack());
    p->orders->push(new BombOrder(p, attack.first));

    if (debugMode)
      cout << "Issued BombOrder on: " << attack.first->getName() << endl;

    break;
  }

  case 1: {
    Territory *target = Utils::accessRandomElement(p->ownedTerritories);
    p->orders->push(new BlockadeOrder(p, target));

    if (debugMode)
      cout << "Issued BlockadeOrder on: " << target->getName() << endl;

    break;
  }

  case 2: {
    Territory *source = nullptr;
    Territory *target = Utils::accessRandomElement(p->ownedTerritories);

    for (auto t : p->ownedTerritories) {
      if (t->getArmies() > 0) {
        source = t;
      }
    }

    if (source == nullptr) {
      return;
    }

    auto armies = source->getArmies();

    if (armies != 1) {
      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_int_distribution<int> dis(1, armies);
      armies = dis(rng);
    }

    p->orders->push(new AirliftOrder(p, armies, source, target));

    if (debugMode)
      cout << "Issued AirliftOrder " << armies
           << " units from: " << source->getName() << " to "
           << target->getName() << endl;

    break;
  }

  case 3: {
    auto ge = GameEngine::instance();

    Player *randomPlayer;
    do {
      randomPlayer = Utils::accessRandomElement(ge->players);
    } while (randomPlayer == p);

    p->orders->push(new NegotiateOrder(p, randomPlayer));

    if (debugMode)
      cout << "Issued NegotiateOrder by " << p->name
           << " against: " << randomPlayer->name << endl;

    break;
  }

  default:throw InvalidCardException(randomCardName + " is not a legal card.");
  }
}

void DefaultPlayerStrategy::issueAdvanceOrder(bool debugMode) {
  // attack if option available
  Territory *source = nullptr, *target = nullptr;
  auto targetTerritories = toAttack();
  std::pair<Territory *, Territory *> randomPair;
  bool foundPair = false;
  for (auto t : targetTerritories) {
    randomPair = t;
    if (randomPair.second->getArmies() > 0) {
      target = randomPair.first;
      source = randomPair.second;
      foundPair = true;
      break;
    }
  }

  if (!foundPair) {
    target = Utils::accessRandomElement(toDefend());
    for (auto t : toDefend()) {
      if (t != target && t->getArmies() > 0) {
        source = t;
      }
    }
  }

  if (source == nullptr) {
    return;
  }

  int armies = source->getArmies();
  if (armies > 1) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dis(1, armies);
    armies = dis(rng);
  }

  p->orders->push(new AdvanceOrder(p, armies, source, target));

  if (debugMode)
    cout << "Issued Advance Order: " << armies << " units from "
         << source->getName() << " [armies = " << source->getArmies() << "] to "
         << target->getName() << " [armies = " << target->getArmies() << "]"
         << endl;
}
DefaultPlayerStrategy::DefaultPlayerStrategy(Player *pPlayer) : PlayerStrategy(pPlayer) {

}
