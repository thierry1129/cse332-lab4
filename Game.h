#pragma once
#ifndef GAME_H
#define GAME_H


#include "Cards.h"
#include "Hand.h"
#include "Deck.h"
#include "Player.h"
#include <memory>
#include <fstream>
#include <iostream>

class Game {

protected:
	static std::shared_ptr <Game> gamePointer;


	Deck main_deck;
	std::vector<std::shared_ptr<Player>> playervec;

	unsigned int pot;
	Player* bet_leader = nullptr;

public:


	static std::shared_ptr<Game> instance();
	static void start_game(const std::string type);
	static void stop_game();

	void add_player(const std::string name);

	std::shared_ptr<Player> find_player(const std::string playername);
	virtual ~Game();

	virtual int before_turn(Player &p) = 0;
	virtual int turn(Player &p) = 0;
	virtual int after_turn(Player &p) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;
	virtual void add_pot(Player& p, unsigned int chipNum);
	virtual void bankrupt(Player& p);
	int size();
	bool ifgamebet = false;
};





#endif