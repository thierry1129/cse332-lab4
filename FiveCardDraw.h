#pragma once



#ifndef FIVECARDDRAW_H
#define FIVECARDDRAW_H

#include "Cards.h"
#include "Hand.h"
#include "Deck.h"
#include "Player.h"

#include "Game.h"
#include <memory>


class Game;
class FiveCardDraw : public Game {


protected:

	Deck discard_deck;
	Deck main_deck;
	// this is an int tracking the current bet on table
	unsigned int game_bet;
	// this is an int trakcing how many players have folded in this round 
	unsigned int folded_players;

private:

	int cardsPerHand = 5;
	

public:
	FiveCardDraw();
	virtual int before_turn(Player &p);
	virtual int before_round();
	virtual void bet(Player &p);

	virtual int turn(Player &p);
	virtual int after_turn(Player &p);
	virtual int round();
	virtual int after_round();
	virtual int allFoldWinner();
	static bool handCompare(std::shared_ptr<Player> a, std::shared_ptr<Player> b);

};

#endif