#ifndef WARZONE_CARD_H
#define WARZONE_CARD_H

class Hand;

class Card;

#include "../Map/Map.h"
#include "../Player/Player.h"
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using std::string;

class Hand {
public:
  void remove(Card *card);

  std::vector<Card *> cards = {};

  explicit Hand(Player *player);

  Hand(const Hand &h);

  Hand &operator=(const Hand &hand);

  void listHand() const;

  Card *draw();

  Card *removeByName(const string &name);

  void removeAll();

  ~Hand();

  friend std::ostream &operator<<(std::ostream &os, const Hand &hand);

  /**
   * Play a card from the deck and add it to the deck
   * @param cardName	The name of the card
   */
  void play(string const &cardName);

  void debugPlay(string const &name);

private:
  Player *player;

  void add(Card *card);
};

class Deck {
public:
  explicit Deck(std::vector<Card *> cards) : cards(std::move(cards)) {}

  Deck() = default;

  Deck(const Deck &deck);

  Deck &operator=(const Deck &deck);

  ~Deck();

  Card *draw();

  void put(Card *card);

  friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

  unsigned long long int getCardsSize();

private:
  std::vector<Card *> cards = vector<Card *>();
};

class Card {
public:
  const std::string name;
  const std::string description;
  const std::vector<std::string> aliases;

  virtual bool play(Player *issuer) const = 0;

  Card(std::string name, std::string description,
	   std::vector<std::string> aliases)
	  : name(std::move(name)), description(std::move(description)),
		aliases(std::move(aliases)) {}

  std::vector<std::string> getAliases();

  virtual ~Card();

private:
  friend std::ostream &operator<<(std::ostream &os, const Card &card);
};

class BombCard : public Card {
public:
  inline BombCard()
	  : Card("Bomb", "Use to destroy half of an enemy territory's army",
			 {"bomb", "bombcard", "bomb card"}) {}

  bool play(Player *issuer) const override;

  static void execute(Player *issuer, Territory *territory);

  ~BombCard() override;
};

class BlockadeCard : public Card {
public:
  inline BlockadeCard()
	  : Card("Blockade",
			 "Use to lose control of a territory but triple its army size",
			 {"blockade", "blockadecard", "blockade card", "block"}) {}

  bool play(Player *issuer) const override;

  static void execute(Player *issuer, Territory *territory);

  ~BlockadeCard() override;
};

class AirliftCard : public Card {
public:
  inline AirliftCard()
	  : Card("Airlift", "Use to move armies from any territory to any other",
			 {"airlift", "airliftcard", "air", "lift", "airlift card",
			  "air lift card", "air lift"}) {}

  bool play(Player *issuer) const override;

  static void execute(Player *issuer, int armiesSize,
					  Territory *territoryPlayer, Territory *territoryTarget);

  ~AirliftCard() override;
};

class NegotiateCard : public Card {
public:
  inline NegotiateCard()
	  : Card("NegotiateCard",
			 "Use to prevent attacks between you and another player",
			 {"negotiate", "negotiatecard", "negotiate card"}) {}

  bool play(Player *issuer) const override;

  static void execute(Player *issuer, Player *target);

  ~NegotiateCard() override;
};

#endif // WARZONE_CARD_H
