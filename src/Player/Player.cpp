#include <algorithm>
#include <iostream>
#include <map>
#include <random>
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

void Player::issueOrder() {
  this->strategy->issueOrder();
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


void PlayerStrategy::issueOrder() {
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
}

std::vector<std::pair<Territory *, Territory *>> PlayerStrategy::toAttack() const {
  auto adjacentEnemies = std::vector<std::pair<Territory *, Territory *>>();
  for (auto t: p->ownedTerritories) {
    for (auto adj: t->getAdjTerritories()) {
      if (adj->getOwner() != p) {
        adjacentEnemies.emplace_back(std::pair(adj, t));
      }
    }
  }
  return adjacentEnemies;
}

void PlayerStrategy::issueDeployOrder() {
  auto ge = GameEngine::instance();

  auto target = toDefend()[0];

  int armies = p->reinforcementsAfterDeploy == 1 ? 1 : Utils::randomNumberInRange(1, p->reinforcementsAfterDeploy);

  if (ge->debugMode)
    cout << "Issued Deploy Order: " << armies << " units to " + target->getContinent()->getName() << endl;

  p->orders->push(new DeployOrder(p, armies, target));
  p->reinforcementsAfterDeploy -= armies;
}


void PlayerStrategy::issueCardOrder() {
  auto ge = GameEngine::instance();
  auto randomCardName = p->hand->cards[0]->name;
  auto cardNameMap = getCardNameMap();
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

PlayerStrategy::PlayerStrategy(Player *P) : p(P) {}


bool PlayerStrategy::isDoneIssuing() {
  return p->advanceOrderIssued && (p->cardOrderIssued || p->hand->cards.empty());
}

void DefaultPlayerStrategy::issueOrder() {
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
}

void CheaterStrategy::issueAdvanceOrder() {
  //do nothing
}
/**
 * CheaterStrategy constructor
 * @param pPlayer The player using this strategy
 */
CheaterStrategy::CheaterStrategy(Player *pPlayer) : PlayerStrategy(pPlayer) {
}
void CheaterStrategy::issueOrder() {
  auto ge = GameEngine::instance();
  Territory *source = nullptr, *target = nullptr;
  auto targetTerritories = toAttack();
  for (auto targetTerritory: targetTerritories) {
    source = targetTerritory.second;
    target = targetTerritory.first;
    target->setOwner(this->p);
    if(ge->debugMode){
      cout << "Player " << source->getName() << " cheated and captured Territory: " << target->getName() << " [armies = " << target->getArmies() << "]" << endl;
    }
  }
  p->advanceOrderIssued = true;
}
void CheaterStrategy::issueDeployOrder() {
  //do nothing
}
void CheaterStrategy::issueCardOrder() {
  //do nothing
}
bool CheaterStrategy::isDoneIssuing() {
  return p->advanceOrderIssued;
}

/**
 * NeutralStrategy constructor
 * @param pPlayer The player using this strategy
 */

NeutralStrategy::NeutralStrategy(Player *pPlayer) : PlayerStrategy(pPlayer) {
}

/**
 * Issue orders similarly to an aggressive player ONLY if it has been attacked
 * @param  If debugging show more information
 */
void NeutralStrategy::issueOrder() {
  //do nothing
}


/**
 * Find all adjacent enemy territories
 * @return all adjacent enemy territories
 */
std::vector<std::pair<Territory *, Territory *>> DefaultPlayerStrategy::toAttack() const {
  auto adjacentEnemies = std::vector<std::pair<Territory *, Territory *>>();
  for (auto t: p->ownedTerritories) {
    for (auto adj: t->getAdjTerritories()) {
      if (adj->getOwner() != p) {
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

bool DefaultPlayerStrategy::isDoneIssuing() {
  return p->advanceOrderIssued && (p->cardOrderIssued || p->hand->cards.empty());
}


// ------------------ Benevolent strategy -------------------------

/*
 * Benevolent player deploys or advances armies on its weakest country,
 * then only defends and is never the aggressor with attack or cards.
 */


/**
 * custom comparator to get a list in ascending order of units
 */
struct compareTerritoriesAscending {
  inline bool operator()(Territory *t1, const Territory *t2) {
    return (t1->getArmies() < t2->getArmies());
  }
} compAsc;


// NOTE: toAttack() <-> toDefend() for Benevolent Player

/**
 * Benevolent player will have a vector of territories that
 * are adjacent to an owned territory that is to be defended.
 * Ordered in ascending number of armies of target territory.
 * @return A list in ascending order of pairs of vectors <owned, owned>
 */
vector<std::pair<Territory *, Territory *>> BenevolentPlayer::toAttack() const {
  auto adjacentFriends = std::vector<std::pair<Territory *, Territory *>>();
  for (auto t: p->ownedTerritories) {
    for (auto adj: t->getAdjTerritories()) {
      if (adj->getOwner() == p) {
        adjacentFriends.emplace_back(std::pair(adj, t));
      }
    }
  }

  std::sort(adjacentFriends.begin(), adjacentFriends.end(),
            [](std::pair<Territory *, Territory *> &left, std::pair<Territory *, Territory *> &right) {
              return left.second->getArmies() < right.second->getArmies();
            });

  return adjacentFriends;
}

/**
 * Returns a list of owned territories.
 * @return list of territories to defend.
 */
std::vector<Territory *> BenevolentPlayer::toDefend() const {
  return p->ownedTerritories;
}

/**
 * Benevolent player will deploy reinforcements
 * to its strongest territory
 */
void BenevolentPlayer::issueDeployOrder() {
  auto ge = GameEngine::instance();

  vector<Territory *> weakestOwnedTerritories = p->ownedTerritories;
  std::sort(weakestOwnedTerritories.begin(), weakestOwnedTerritories.end(), compAsc);

  int armies = p->reinforcementsAfterDeploy == 1 ? 1 : Utils::randomNumberInRange(1, p->reinforcementsAfterDeploy);
  auto weakestTerritory = weakestOwnedTerritories[0];

  if (ge->debugMode)
    cout << "Issued Deploy Order: " << armies << " units to weakest territory (" << weakestTerritory->getName() << ")" << endl;

  p->orders->push(new DeployOrder(p, armies, weakestTerritory));
  p->reinforcementsAfterDeploy -= armies;
}

/**
 * Issue orders by the benevolent player,
 * they never attack and always defend the weakest regions.
 */
void BenevolentPlayer::issueAdvanceOrder() {
  cout << p->name + " is ready to advance.. but nothing more" << endl;

  auto pairs = toAttack();
  if (pairs.empty()) {
    cout << "No units to advance to, none of the owned territories are adjacent" << endl;
    return;
  }

  Territory *source = pairs[0].first;
  Territory *target = pairs[0].second;

  int armies = Utils::randomNumberInRange(0, source->getArmies());
  p->orders->push(new AdvanceOrder(p, armies, source, target));
}

/**
 * Check if the benevolent player has finished executing all orders
 * @return a bool if the benevolent player has exhausted all options
 */
bool BenevolentPlayer::isDoneIssuing() {
  return p->advanceOrderIssued && (p->cardOrderIssued || p->hand->cards.empty());
}


/**
 * Constructor to give a certain player a benevolent strategy
 * @param pPlayer Any player with units, territories and a deck
 */
BenevolentPlayer::BenevolentPlayer(Player *pPlayer) : PlayerStrategy(pPlayer) {}


// ------------------ Aggressive strategy -------------------------


/**
 * Aggressive player will have a vector of territories that
 * are adjacent to enemy territories that will focus on attacking
 * @return A list in descending order of pairs of vectors <enemy, owned>
 */
vector<std::pair<Territory *, Territory *>> AggressivePlayerStrategy::toAttack() const {
  auto sortedPairs = PlayerStrategy::toAttack();

  if (!sortedPairs.empty()) {

    std::sort(sortedPairs.begin(), sortedPairs.end(),
              [](const std::pair<Territory *, Territory *> &t1, std::pair<Territory *, Territory *> &t2) -> bool {
                return (t1.second->getArmies() > t2.second->getArmies());
              });
    return sortedPairs;

  } else {
    cout << " You currently don't have neighboring enemies." << endl;
    return sortedPairs;
  }
}

/**
 * Aggressive player will choose to defend the territories,
 * ordered in descending number of armies
 * @return list of territories to defend, not all owned territories.
 */

vector<Territory *> AggressivePlayerStrategy::toDefend() const {
  auto ge = GameEngine::instance();
  vector<Territory *> toDefendTerritories;
  auto vulnerableTerritories = toAttack();

  cout << "\nPlayer " << p->name << "'s territories that need defending:" << endl;
  for (auto t: vulnerableTerritories) {
    if (std::find(toDefendTerritories.begin(), toDefendTerritories.end(), t.second) == toDefendTerritories.end()) {
      if (ge->debugMode)
        cout << t.second->getName() + "   "
             << t.second->getArmies() << endl;
      toDefendTerritories.push_back(t.second);
    }
  }
  return toDefendTerritories;
}


/**
 * Issue orders by the aggressive player,
 * they attack first, otherwise they defend edge territories.
 */

void AggressivePlayerStrategy::issueAdvanceOrder() {

  Territory *target;
  Territory *source;

  cout << p->name + " is ready to issue attacks" << endl;

  if (nextToAttack >= toAttack().size())
    nextToAttack = 0;

  auto pair = toAttack()[nextToAttack];

  target = pair.first;
  source = pair.second;

  int armyInTerritory = source->getArmies();

  int attackNum = (armyInTerritory - 1) > 0 ? (armyInTerritory - 1) : 0;

  if (attackNum > 0) {
    cout << source->getName() << " chosen enemy to attack "
         << target->getName() << " with " << attackNum << " units" << endl;

    p->orders->push(new AdvanceOrder(p, attackNum, source, target));

    cout << "----" << endl;
  } else {
    cout << source->getName() << " does not have enough units to attack " << endl;
  }
  nextToAttack++;
}


/**
 * Constructor to give a certain player an aggressive strategy
 * @param pPlayer Any player with units, territories and a deck
 */
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *pPlayer) : PlayerStrategy(pPlayer) {
}
