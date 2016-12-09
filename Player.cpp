#include "stdafx.h"

#include "Player.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<iterator>
#include <vector>

Player::Player(std::string name) :
handWon(0), handLost(0), chipCount(20) {
    playerName = name;
    std::ifstream ifs(name);
    std::string line;
	chipCount = 20;
    // for now I asssume in the player file the lines contains name, won, lost (string, int, int)
    if (ifs.is_open()) {
        //int to record if Name, Wins, and Losses have been read in
        int nwlRead = 0;
        while (getline(ifs, line)) {
            if (line.empty()) {
            }
            else {
                std::istringstream iss(line);
                std::string s;
                while (iss >> s && nwlRead != 3) {
                    if (s == "Name") {
                        iss >> s;
                        playerName = s;
                        ++nwlRead;
                    }
                    else if (s == "Wins") {
                        iss >> s;
                        handWon = stoul(s);
                        ++nwlRead;
                    }
                    else if (s == "Losses") {
                        iss >> s;
                        handLost = stoul(s);
                        ++nwlRead;
                    }
                }
            }
        }
        //Input file did not contain Name, wins, and losses
        if (nwlRead != 3) {
            handWon = 0;
            handLost = 0;
        }
    }
}


std::ostream& operator<<(std::ostream& out, const Player& p) {
    
    out << p.playerName << " games won: " << p.handWon << " games lost " << p.handLost;
    return out;
    
}