#ifndef WARZONE_CARD_H
#define WARZONE_CARD_H

class CardManager;

class Card;

#include <utility>
#include <vector>
#include <string>
#include <ostream>

class Hand {
public:

    void remove(int index);

    std::vector<Card *> cards;

    Hand() {};
    Hand(const Hand& h);
    Hand& operator=(const Hand& hand);

    void listHand();

    Card* draw();

    ~Hand();

    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);

private:
    void add(Card *card);
};

#include "../Player/Player.h"

class Card {
public:
    const std::string name;
    const std::string description;

    virtual bool play(Player *issuer) const = 0;

    Card(std::string name, std::string description) : name(std::move(name)), description(std::move(description)){}


    virtual ~Card();

private:
    friend std::ostream &operator<<(std::ostream &os, const Card &card);
};

class BombCard : public Card {
public:
    inline BombCard() : Card("BombCard", "Use to destroy half of an enemy territory's army") {}

    bool play(Player *issuer) const override;

    ~BombCard() override;
};

class BlockadeCard : public Card {
public:
    inline BlockadeCard() : Card("BlockadeCard", "Use to lose control of a territory but triple its army size") {}

    bool play(Player *issuer) const override;

    ~BlockadeCard() override;
};

class AirliftCard : public Card {
public:
    inline AirliftCard() : Card("AirliftCard", "Use to move armies from any territory to any other") {}

    bool play(Player *issuer) const override;

    ~AirliftCard() override;
};

class NegotiateCard : public Card {
public:
    inline NegotiateCard() : Card("NegotiateCard", "Use to prevent attacks between you and another player") {}

    bool play(Player *issuer) const override;

    ~NegotiateCard() override;
};


class Deck {
public:

    explicit Deck(std::vector<Card *> cards) : cards(std::move(cards)){}
    Deck(){}
    Deck(const Deck& deck);

    Deck& operator=(const Deck& deck);

    ~Deck();

    Card *draw();

    void put(Card *card);

    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

    int getCardsSize();

    int getRandomLocation();

private:
    std::vector<Card *> cards;
};


#endif //WARZONE_CARD_H
