#include <iostream>
#include <map>
#include "../GameEngine/GameEngine.h"
#include "Player.h"
#include <set>


/**
 * Get all the enemy territories adjacent to your own
 * @return all the enemy territories adjacent to yours
 */
vector<Territory *> Player::getAdjacentEnemyTerritories() {
  vector<Territory *> enemyTerritoriesAdjacent;
  for (auto friendlyTerritory: this->ownedTerritories) {
    for (auto adjacentTerritory: friendlyTerritory->getAdjTerritories()) {
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

<<<<<<< HEAD
=======
void Player::issueOrder() {
  this->strategy->issueOrder();
}
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6

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

void DefaultPlayerStrategy::issueOrder() {
<<<<<<< HEAD
    // Current Mechanism:
    // ------------------
    // 	1. Keep deploying until zero reinforcements are left. Random number each turn.
    //    2. If user has a card in their hand, they play it.
    //    3. If player has not advanced an order this round, they advance order.
    // NOTE: All orders that are issued follow random moves for now.

    if (p->reinforcementsAfterDeploy > 0) {
        issueDeployOrder();
    } else if (!p->cardOrderIssued && !p->hand->cards.empty()) {
        issueCardOrder();
        p->cardOrderIssued = true;
    } else if (!p->advanceOrderIssued) {
        issueAdvanceOrder();
        p->advanceOrderIssued = true;
    }
=======
  // Current Mechanism:
  // ------------------
  // 	1. Keep deploying until zero reinforcements are left. Random number each turn.
  //    2. If user has a card in their hand, they play it.
  //    3. If player has not advanced an order this round, they advance order.
  // NOTE: All orders that are issued follow random moves for now.

  if (p->reinforcementsAfterDeploy > 0) {
    issueDeployOrder();
  } else if (!p->cardOrderIssued && !p->hand->cards.empty()) {
    issueCardOrder();
    p->cardOrderIssued = true;
  } else if (!p->advanceOrderIssued) {
    issueAdvanceOrder();
    p->advanceOrderIssued = true;
  }
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
}

/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
std::vector<std::pair<Territory *, Territory *>> DefaultPlayerStrategy::toAttack() const {
    auto adjacentEnemies = std::vector<std::pair<Territory *, Territory *>>();
    for (auto t: p->ownedTerritories) {
        for (auto adj: t->getAdjTerritories()) {
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

void DefaultPlayerStrategy::issueDeployOrder() {
  auto ge = GameEngine::instance();

  auto target = Utils::accessRandomPair(toAttack()).second;

  int armies = p->reinforcementsAfterDeploy == 1 ? 1 : Utils::randomNumberInRange(1, p->reinforcementsAfterDeploy);

  if (ge->debugMode)
    cout << "Issued Deploy Order: " << armies << " units to " + target->getContinent()->getName() << endl;

  p->orders->push(new DeployOrder(p, armies, target));
  p->reinforcementsAfterDeploy -= armies;
}

void DefaultPlayerStrategy::issueCardOrder() {
  auto ge = GameEngine::instance();
  auto randomCardName = p->hand->cards[0]->name;

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

      if (ge->debugMode)
        cout << "Issued BombOrder on: " << attack.first->getName() << endl;

      auto c = p->hand->removeByName("Bomb");
      ge->deck->put(c);

      break;
    }

    case 1: {
      Territory *target = Utils::accessRandomElement(p->ownedTerritories);
      p->orders->push(new BlockadeOrder(p, target));

      if (ge->debugMode)
        cout << "Issued BlockadeOrder on: " << target->getName() << endl;

      auto c = p->hand->removeByName("Blockade");
      ge->deck->put(c);

      break;
    }

    case 2: {
      Territory *source = nullptr;
      Territory *target = Utils::accessRandomElement(p->ownedTerritories);

      for (auto t: p->ownedTerritories) {
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

      if (ge->debugMode)
        cout << "Issued AirliftOrder " << armies
             << " units from: " << source->getName() << " to "
             << target->getName() << endl;

      auto c = p->hand->removeByName("Airlift");
      ge->deck->put(c);

      break;
    }

    case 3: {
      Player *randomPlayer;
      do {
        randomPlayer = Utils::accessRandomElement(ge->players);
      } while (randomPlayer == p);

      p->orders->push(new NegotiateOrder(p, randomPlayer));

      if (ge->debugMode)
        cout << "Issued NegotiateOrder by " << p->name
             << " against " << randomPlayer->name << endl;

      auto c = p->hand->removeByName("Negotiate");
      ge->deck->put(c);

      break;
    }

    default:
      throw InvalidCardException(randomCardName + " is not a legal card.");
  }
}

void DefaultPlayerStrategy::issueAdvanceOrder() {
  auto ge = GameEngine::instance();

  Territory *source = nullptr;
  Territory *target = nullptr;

  for (auto pair: toAttack()) {
    if (pair.second->getArmies() > 5) {
      target = pair.first;
      source = pair.second;
    }
  }

  if (target == nullptr || source == nullptr) {
    auto rng = std::default_random_engine();
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(p->ownedTerritories.begin(), p->ownedTerritories.end(), rng);

    for (auto t: p->ownedTerritories) {
      if (t->getArmies() > 0) {
        source = t;
      }
    }

    if (!source) {
      return;
    }

    for (auto t: source->getAdjTerritories()) {
      if (t->getOwner() == p) {
        target = t;
      }
    }
  }

  if (!target) {
    return;
  }

  int armies = source->getArmies();
  if (armies < 5) {
    //    std::random_device rd;
    //    std::mt19937 rng(rd());
    //    std::uniform_int_distribution<int> dis(1, armies);
    //    armies = dis(rng);
    return;
  }
  armies *= 0.90;// NOLINT(cppcoreguidelines-narrowing-conversions)

  p->orders->push(new AdvanceOrder(p, armies, source, target));

  if (ge->debugMode)
    cout << "Issued Advance Order: " << armies << " units from "
         << *source << " to " << *target << endl;
}
DefaultPlayerStrategy::DefaultPlayerStrategy(Player *pPlayer) : PlayerStrategy(pPlayer) {
}
std::vector<std::pair<Territory *, Territory *>> AggressivePlayer::toAttack() const {
    return std::vector<std::pair<Territory *, Territory *>>();
}
vector<Territory *> AggressivePlayer::toDefend() const {
    return vector<Territory *>();
}
void AggressivePlayer::issueOrder() {
    cout << "Need to implement" << endl;
}
void AggressivePlayer::issueDeployOrder() {
    cout << "Need to implement" << endl;
}
void AggressivePlayer::issueAdvanceOrder() {
    cout << "Need to implement" << endl;
}
void AggressivePlayer::issueCardOrder() {
    cout << "Need to implement" << endl;
}
AggressivePlayer::AggressivePlayer(Player *pPlayer) : PlayerStrategy(pPlayer) {
}

/**
 * Advances and immediately conquers with 2x army size each adjacent enemy territory
 * @param  If debugging show more information
 */
void CheaterStrategy::issueAdvanceOrder() {
  auto ge = GameEngine::instance();
    Territory *source = nullptr, *target = nullptr;
    std::set<std::string> territoriesAdvanceIssued;
    auto targetTerritories = toAttack();
    for (auto targetTerritory: targetTerritories) {
        source = targetTerritory.second;
        target = targetTerritory.first;
        if (territoriesAdvanceIssued.find(target->getName()) != territoriesAdvanceIssued.end()) {
            territoriesAdvanceIssued.insert(target->getName());
            p->orders->push(new AdvanceOrder(p, target->getArmies() * 2, source, target));
        }
        if(ge->debugMode){
            cout << "Issued Advance Order: " << target->getArmies() << " units from "
                 << source->getName() << " [armies = " << target->getArmies() * 2 << "] to "
                 << target->getName() << " [armies = " << target->getArmies() << "]"
                 << endl;
        }
    }
}
/**
 * CheaterStrategy constructor
 * @param pPlayer The player using this strategy
 */
CheaterStrategy::CheaterStrategy(Player *pPlayer) : AggressivePlayer(pPlayer) {
}

/**
 * NeutralStrategy constructor
 * @param pPlayer The player using this strategy
 */

NeutralStrategy::NeutralStrategy(Player *pPlayer) : AggressivePlayer(pPlayer) {
    hasBeenAttacked = false;
}

/**
 * Issue orders similarly to an aggressive player ONLY if it has been attacked
 * @param  If debugging show more information
 */
void NeutralStrategy::issueOrder() {
    if(hasBeenAttacked)
        AggressivePlayer::issueOrder();
}
/**
 * Set whether the player has been attacked
 */
void NeutralStrategy::setHasBeenAttacked() {
    hasBeenAttacked = true;
}
/**
 * Get whether the player has been attacked
 * @return Boolean representing if the player has been attacked
 */
bool NeutralStrategy::getHasBeenAttacked() const {
    return hasBeenAttacked;
}

void Player::issueOrder() {
  // Current Mechanism:
  // ------------------
  // 	1. Keep deploying until zero reinforcements are left. Random number each turn.
  //    2. If user has a card in their hand, they play it.
  //    3. If player has not advanced an order this round, they advance order.
  // NOTE: All orders that are issued follow random moves for now.

  if (reinforcementsAfterDeploy > 0) {
    issueDeployOrder();
  } else if (!cardOrderIssued && !hand->cards.empty()) {
    issueCardOrder();
    cardOrderIssued = true;
  } else if (!advanceOrderIssued) {
    issueAdvanceOrder();
    advanceOrderIssued = true;
  }
}

/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
std::vector<std::pair<Territory *, Territory *>> Player::toAttack() const {
  auto adjacentEnemies = std::vector<std::pair<Territory *, Territory *>>();
<<<<<<< HEAD
  for (auto t: ownedTerritories) {
    for (auto adj: t->getAdjTerritories()) {
      if (adj->getOwner() != this) {
=======
  for (auto t: p->ownedTerritories) {
    for (auto adj: t->getAdjTerritories()) {
      if (adj->getOwner() != p) {
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
        adjacentEnemies.emplace_back(std::pair(adj, t));
      }
    }
  }

  return adjacentEnemies;
}

/**
 * Find all territories the player owns
 * @return all territories the player owns
 */

<<<<<<< HEAD
vector<Territory *> Player::toDefend() const {
  return ownedTerritories;
}

void Player::issueDeployOrder() {
  auto ge = GameEngine::instance();

  auto target = Utils::accessRandomPair(toAttack()).second;

  int armies = reinforcementsAfterDeploy == 1 ? 1 : Utils::randomNumberInRange(1, reinforcementsAfterDeploy);

  if (ge->debugMode)
    cout << "Issued Deploy Order: " << armies << " units to " + target->getContinent()->getName() << endl;

  orders->push(new DeployOrder(this, armies, target));
  reinforcementsAfterDeploy -= armies;
}

void Player::issueCardOrder() {
  auto ge = GameEngine::instance();
  auto randomCardName = hand->cards[0]->name;

=======
vector<Territory *> DefaultPlayerStrategy::toDefend() const {
  return p->ownedTerritories;
}

void DefaultPlayerStrategy::issueDeployOrder() {
  auto ge = GameEngine::instance();

  auto target = Utils::accessRandomPair(toAttack()).second;

  int armies = p->reinforcementsAfterDeploy == 1 ? 1 : Utils::randomNumberInRange(1, p->reinforcementsAfterDeploy);

  if (ge->debugMode)
    cout << "Issued Deploy Order: " << armies << " units to " + target->getContinent()->getName() << endl;

  p->orders->push(new DeployOrder(p, armies, target));
  p->reinforcementsAfterDeploy -= armies;
}

void DefaultPlayerStrategy::issueCardOrder() {
  auto ge = GameEngine::instance();
  auto randomCardName = p->hand->cards[0]->name;

>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
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
<<<<<<< HEAD
      orders->push(new BombOrder(this, attack.first));
=======
      p->orders->push(new BombOrder(p, attack.first));
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6

      if (ge->debugMode)
        cout << "Issued BombOrder on: " << attack.first->getName() << endl;

<<<<<<< HEAD
      auto c = hand->removeByName("Bomb");
=======
      auto c = p->hand->removeByName("Bomb");
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
      ge->deck->put(c);

      break;
    }

    case 1: {
<<<<<<< HEAD
      Territory *target = Utils::accessRandomElement(ownedTerritories);
      orders->push(new BlockadeOrder(this, target));
=======
      Territory *target = Utils::accessRandomElement(p->ownedTerritories);
      p->orders->push(new BlockadeOrder(p, target));
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6

      if (ge->debugMode)
        cout << "Issued BlockadeOrder on: " << target->getName() << endl;

<<<<<<< HEAD
      auto c = hand->removeByName("Blockade");
=======
      auto c = p->hand->removeByName("Blockade");
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
      ge->deck->put(c);

      break;
    }

    case 2: {
      Territory *source = nullptr;
<<<<<<< HEAD
      Territory *target = Utils::accessRandomElement(ownedTerritories);

      for (auto t: ownedTerritories) {
=======
      Territory *target = Utils::accessRandomElement(p->ownedTerritories);

      for (auto t: p->ownedTerritories) {
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
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

<<<<<<< HEAD
      orders->push(new AirliftOrder(this, armies, source, target));
=======
      p->orders->push(new AirliftOrder(p, armies, source, target));
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6

      if (ge->debugMode)
        cout << "Issued AirliftOrder " << armies
             << " units from: " << source->getName() << " to "
             << target->getName() << endl;

<<<<<<< HEAD
      auto c = hand->removeByName("Airlift");
=======
      auto c = p->hand->removeByName("Airlift");
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
      ge->deck->put(c);

      break;
    }

    case 3: {
      Player *randomPlayer;
      do {
        randomPlayer = Utils::accessRandomElement(ge->players);
<<<<<<< HEAD
      } while (randomPlayer == this);

      orders->push(new NegotiateOrder(this, randomPlayer));

      if (ge->debugMode)
        cout << "Issued NegotiateOrder by " << name
             << " against " << randomPlayer->name << endl;

      auto c = hand->removeByName("Negotiate");
=======
      } while (randomPlayer == p);

      p->orders->push(new NegotiateOrder(p, randomPlayer));

      if (ge->debugMode)
        cout << "Issued NegotiateOrder by " << p->name
             << " against " << randomPlayer->name << endl;

      auto c = p->hand->removeByName("Negotiate");
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
      ge->deck->put(c);

      break;
    }

    default:
      throw InvalidCardException(randomCardName + " is not a legal card.");
  }
}

<<<<<<< HEAD
void Player::issueAdvanceOrder() {
=======
void DefaultPlayerStrategy::issueAdvanceOrder() {
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
  auto ge = GameEngine::instance();

  Territory *source = nullptr;
  Territory *target = nullptr;

  for (auto pair: toAttack()) {
    if (pair.second->getArmies() > 5) {
      target = pair.first;
      source = pair.second;
    }
  }

  if (target == nullptr || source == nullptr) {
    auto rng = std::default_random_engine();
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
<<<<<<< HEAD
    std::shuffle(ownedTerritories.begin(), ownedTerritories.end(), rng);

    for (auto t: ownedTerritories) {
=======
    std::shuffle(p->ownedTerritories.begin(), p->ownedTerritories.end(), rng);

    for (auto t: p->ownedTerritories) {
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
      if (t->getArmies() > 0) {
        source = t;
      }
    }

    if (!source) {
      return;
    }

    for (auto t: source->getAdjTerritories()) {
<<<<<<< HEAD
      if (t->getOwner() == this) {
=======
      if (t->getOwner() == p) {
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
        target = t;
      }
    }
  }

  if (!target) {
    return;
  }

  int armies = source->getArmies();
  if (armies < 5) {
    //    std::random_device rd;
    //    std::mt19937 rng(rd());
    //    std::uniform_int_distribution<int> dis(1, armies);
    //    armies = dis(rng);
    return;
  }
  armies *= 0.90;// NOLINT(cppcoreguidelines-narrowing-conversions)

  orders->push(new AdvanceOrder(this, armies, source, target));

  if (ge->debugMode)
    cout << "Issued Advance Order: " << armies << " units from "
         << *source << " to " << *target << endl;
}
<<<<<<< HEAD

bool Player::isDoneIssuing() {
  return advanceOrderIssued && (cardOrderIssued || hand->cards.empty());
=======
DefaultPlayerStrategy::DefaultPlayerStrategy(Player *pPlayer) : PlayerStrategy(pPlayer) {
}

bool DefaultPlayerStrategy::isDoneIssuing() {
  return p->advanceOrderIssued && (p->cardOrderIssued || p->hand->cards.empty());
>>>>>>> f3e11f44e7ff767df141e6b7c4eb6a0c3ae46cc6
}

