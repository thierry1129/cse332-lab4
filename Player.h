#pragma once
#include "stdafx.h"
#include "Cards.h"
#include "Hand.h"
#include "Deck.h"

#include<iostream>

struct Player {
	
	std::string playerName;

	Hand playerHand;
	bool fold = false;
	unsigned int handWon;
	unsigned int handLost;
	unsigned int chipCount;
	unsigned int bet_put_in;
	Player(std::string name);
};



std::ostream& operator<<(std::ostream& out, const Player& p);