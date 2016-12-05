
#ifndef HAND_H
#define HAND_H
#include "Cards.h"
#include "Deck.h"
#include <vector>
#include <iostream>
#include <random>




class Deck;
class FiveCardDraw;

class Hand {
    
    friend std::ostream& operator<<(std::ostream& out, const Hand& h);
    friend Hand& operator<<(Hand&h, Deck& d);
    friend bool Hand::poker_rank(const Hand& a, const Hand& b);
    friend Hand checkHand(Hand &cards);
    friend FiveCardDraw;
    
public:
    
    Hand() = default;
    Hand& operator=(const Hand& h);
    const int size();
    
    int handInt;
    
    static const char* handrankArray[9];
    Hand(vector<Card> c, int i) {
        cardvec = c;
        handInt = i;
    }
    bool operator==(const Hand& h);
    bool operator<(const Hand& h);
    std::string asString() const;
    Card& operator[](const size_t);
    
    void remove_card(const size_t);
    void make_faceDown(const size_t);
    
private:
    std::vector<Card> cardvec;
    
    
    
    
};

std::ostream& operator<<(std::ostream& out, const Hand& h);
bool poker_rank(const Hand& a, const Hand& b);
#endif
