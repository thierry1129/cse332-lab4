#include "stdafx.h"
#include "Deck.h"
#include "Cards.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <ostream>

Deck::Deck(const char* &filename) {
    
    int loadErr =
    load(filename);
    
    if (loadErr) {
        throw loadErr;
    }
    
}
int Deck::load(const char* &filename) {
    
    
    std::ifstream ifs(filename);
    int a = 0;
    std::string line;
    std::vector<Card> cardvec2;
    
    
    if (ifs.is_open()) {
        
        while (getline(ifs, line)) {
            if (line.empty()) {
                
            }
            else {
                
                char readRank;
                char readSuit;
                std::istringstream iss(line);
                
                int cardcount = 0;
                
                std::string s;
                while (iss >> s) {
                    if (s == "//") {
                        break;
                    }
                    else if (s[1] == '0') {
                        // special case for ten
                        readRank = 't';
                        readSuit = s[2];
                    }
                    else {
                        readRank = s[0];
                        readSuit = s[1];
                    }
                    
                    
                    if (readSuit == '/' || readRank == '/') {
                        break;
                    }
                    
                    Card::Rank rank;
                    Card::Suit suit;
                    /*  call the helper method here to get the proper format for the suits and ranks */
                    
                    int suiterr = getSuit(readSuit, suit);
                    int rankerr = getRank(readRank, rank);
                    
                    if (rankerr != 0 || suiterr != 0) {
                        
                        if (rankerr != 0) {
                            cout << "there is an error with your rank, reading the next card now " << endl;
                            iss.unget();
                            iss >> ws;
                            a = rankerr;
                            
                        }
                        else {
                            cout << "there is an error with your suit" << endl;
                            iss >> ws;
                            a = suiterr;
                        }
                        
                    }
                    else {
                        
                        cardvec2.push_back(Card(rank, suit));
                        
                        cardcount++;
                    }
                }
                
                cardvec.insert(cardvec.end(), cardvec2.begin(), cardvec2.end());
                cardcount = 0;
                cardvec2.clear();
                
                
            }
            
        }
        
    }
    
    return a;
    
}


void Deck::shuffle() {
    unsigned int seed = (unsigned int) std::chrono::system_clock::now().time_since_epoch().count();
    
    std::shuffle(cardvec.begin(), cardvec.end(), std::default_random_engine(seed));
}

const int Deck::size() {
    return cardvec.size();
}

std::ostream& operator<<(std::ostream& out, const Deck& deck) {
    for (auto i = deck.cardvec.cbegin(); i != deck.cardvec.cend(); i++) {
        out << Card::rankArray[i->rank] << Card::suitArray[i->suit] << std::endl;
    }
    return out;
}

void Deck::add_card(Card newcard) {
    this->cardvec.push_back(newcard);
}
