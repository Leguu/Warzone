//
// Created by Legu on 2022-09-13.
//

#ifndef WARZONE_CARD_H
#define WARZONE_CARD_H

#include <vector>
#include <string>
#include <ostream>
#include "../Orders/Order.h"

class Card {
public:
    const std::string name;
    const std::string description;

    virtual void play() const = 0;

    Card(std::string name, std::string description);

private:
    friend std::ostream &operator<<(std::ostream &os, const Card &card);
};

class BombCard : public Card {
public:
    inline BombCard() : Card("Bomb", "Use to destroy half of an enemy territory's army") {}

    void play() const override;
};

class BlockadeCard : public Card {
public:
    inline BlockadeCard() : Card("Blockade", "Use to lose control of a territory but triple its army size") {}

    void play() const override;
};

class AirliftCard : public Card {
public:
    inline AirliftCard() : Card("Airlift", "Use to move armies from any territory to any other") {}

    void play() const override;
};

class NegotiateCard : public Card {
public:
    inline NegotiateCard() : Card("Negotiate", "Use to prevent attacks between you and another player") {}

    void play() const override;
};

class Hand {
public:
    std::vector<const Card *> cards;

    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);
};

class Deck {
public:
    /// TODO
    const Card *draw();

    /// TODO
    void put(Card *);

    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

private:
    std::vector<Card *> cards;
};

class CardManager {
public:
    void listHand();

    void listDeck();

    /// TODO: Play a card from hand (maybe use the name of the card?), put it in deck
    void play();

    void draw();

private:
    Hand hand;
    Deck deck;
};


#endif //WARZONE_CARD_H
