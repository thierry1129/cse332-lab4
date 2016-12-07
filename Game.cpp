#include "stdafx.h"
#include "Game.h"
#include <memory>
#include "FiveCardDraw.h"


std::shared_ptr <Game> Game::gamePointer = NULL;



std::shared_ptr<Game> Game::instance() {
	if (gamePointer == NULL) {
		throw "Instance not available. Pointer not initialized.";
	}
	else {
		std::shared_ptr<Game> g_copy(gamePointer);
		return g_copy;
	}
}



void Game::start_game(const std::string type) {
	//std::cout << "test2.01" << endl;
	if (gamePointer != NULL)
	{
		//std::cout << "test2.1" << endl;
		throw "this game has already started";
	}

	std::string game_type = type;
	if (game_type.find("FiveCardDraw")) {
	//if (game_type!="FiveCardDraw") {
		//std::cout << "test2.2" << endl;
		throw "unknown game";		
	}
	gamePointer = std::make_shared<FiveCardDraw>();
}





void Game::stop_game() {
	if (gamePointer != NULL) {
		gamePointer = NULL;
	}
	else
	{
		throw "no game in progress";
	}
}


void Game::add_player(const std::string name) {
	auto duplicate_player = find_player(name);
	if (duplicate_player != NULL) {
		throw "There is already a player with that name";
	}
	else {
		playervec.push_back(std::make_shared<Player>(name));
	}
}
void Game::add_pot(Player& p, unsigned int amount) {
	p.chipCount -= amount;
	p.bet_put_in += amount;
	pot += amount;
}

void Game::bankrupt(Player &p) {
	
		std::cout << "you are bankrupt, reset chip type yes, quit game type no" << std::endl;
		std::string bankruptinput;
		std::cin >> bankruptinput;
		if (bankruptinput.find("no")) {
			


			std::cout << "you chose to leave, game lost" << std::endl;

			

			std::ofstream ofs(p.playerName + ".txt", std::ofstream::out);



			ofs << "Name " << (p).playerName << endl;
			ofs << "Wins " << (p).handWon << endl;
			ofs << "Losses " << (p).handLost << endl;
			ofs << "chipCount " << (p).chipCount << endl;
			ofs.close();



			for (std::vector<std::shared_ptr<Player>>::iterator p1 = playervec.begin(); p1 != playervec.end(); ++p1) {
				if ((*p1)->playerName == p.playerName) {
					playervec.erase(p1);
					//ifleave = "no";
					break;
				}
			}
		}





		else {
			p.chipCount = 20;
			cout << "system is offering you 20 chips, you now have " << p.chipCount << " chips" << endl;
			
		}
	
	
}

std::shared_ptr<Player> Game::find_player(const std::string playername) {
	for (std::vector<std::shared_ptr<Player>>::iterator p = playervec.begin(); p != playervec.end(); ++p) {
		if ((*p)->playerName == playername) {
			return *p;
		}
	}
	std::shared_ptr<Player> failed = NULL;
	return failed;
}

Game::~Game() {

}

int Game::size() {
	return playervec.size();
}