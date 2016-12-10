
#ifndef HAND_H
#define HAND_H
#include "Cards.h"
#include "Deck.h"
#include <vector>
#include <iostream>
#include <random>

class Deck;
class FiveCardDraw;
class SevenCardStud; //added lab4

class Hand {
    
    friend std::ostream& operator<<(std::ostream& out, const Hand& h);
    friend Hand& operator<<(Hand&h, Deck& d);
    friend bool Hand::poker_rank(Hand& a, Hand& b);
    //friend bool Hand::sevenCard_poker_rank(Hand& a, Hand& b); //added lab4
    friend Hand checkHand(Hand &cards);
    friend Hand check7Hand(Hand &cards); //added lab4
    friend FiveCardDraw;
    friend SevenCardStud;
    
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
bool poker_rank(Hand& a, Hand& b);
//bool sevenCard_poker_rank(Hand& a, Hand& b); //added to check seven card stud
Hand check7Hand(Hand &cards);
#endif
