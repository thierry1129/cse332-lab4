#ifndef DECK_H
#define DECK_H

#include "stdafx.h"

#include "Cards.h"
#include <vector>
#include <iostream>
#include <random>



class Hand;

class FiveCardDraw;
class Deck {
    
    friend Hand& operator<<(Hand& h, Deck& d);
    friend std::ostream& operator<<(std::ostream& out, const Deck& deck);
    
    friend FiveCardDraw;
public:
    
    Deck() = default;
    Deck(const char* &filename);
    
    
    int load(const char* &filename);
    void shuffle();
    const int size();
    void add_card(Card);
    
private:
    Deck(const Deck& d) = delete;
    std::vector<Card> cardvec;
    
};

std::ostream& operator<<(std::ostream& out, const Deck& deck);


#endif