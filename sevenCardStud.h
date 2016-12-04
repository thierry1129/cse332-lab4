#pragma once

#ifndef sevenCardStud
#define sevenCardStud

#include "Cards.h"
#include "Hand.h"
#include "Deck.h"
#include "Player.h"

#include "Game.h"
#include <memory>


class Game;
class SevenCardStud : public Game {
    
protected:
    
    size_t dealer;
    Deck discard_deck;
    Deck main_deck;
private:
    
    int cardsPerHand = 7;
    int playersFolded = 0;
    int ante;
    unsigned int game_bet;
    
public:
    SevenCardStud();
    //virtual int before_turn(Player &p);
    //virtual int before_round();
    virtual int turn(Player &p);
    virtual int after_turn(Player &p);
    //virtual int round();
    //virtual int after_round();
    //static bool handCompare(std::shared_ptr<Player> a, std::shared_ptr<Player> b);
    
};

#endif
