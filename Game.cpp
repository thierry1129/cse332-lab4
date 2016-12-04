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
    if (gamePointer != NULL)
    {
        throw "this game has already started";
    }
    
    std::string game_type = type;
    if (!game_type.find("FiveCardDraw")) {
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
    if (duplicate_player = NULL) {
        throw "There is already a player with that name";
    }
    else {
        playervec.push_back(std::make_shared<Player>(name));
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