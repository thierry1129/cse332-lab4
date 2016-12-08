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
    unsigned int folded_players = 0;
    int ante;
    unsigned int game_bet;
    
public:
    SevenCardStud();
    
    virtual int before_round();
    virtual int round();
    
    virtual void bet(Player &p);
    
    //virtual int before_turn(Player &p);
    //virtual int turn(Player &p);
    virtual int after_turn(Player &p);
    
    virtual int after_round();
    static bool handCompare(std::shared_ptr<Player> a, std::shared_ptr<Player> b);
    
    virtual int firstTurn(Player &p);
    virtual int twoThrFourTurn(Player &p);
    virtual int finalTurn(Player &p);
    
};

#endif
