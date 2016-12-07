#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Hand;
class Deck;

struct Card {
    
    
    enum Rank { two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
    enum Suit { clubs, diamonds, hearts, spades };
    /* this is an array that will help us print out the enums in the correct string forms instead of just numbers*/
    static const char* rankArray[13];
    static const char* suitArray[4];
    
    Rank rank;
    Suit suit;
    bool faceUp; //cards defaulted to be face up (ie, visible to other players)
    Card() = default;
    Card(Rank r, Suit s) {
        suit = s;
        rank = r;
        faceUp = true;
    }
    Card(Rank r, Suit s, bool facing) {
        suit = s;
        rank = r;
        faceUp = facing;
    }
};
bool operator > (const Card& a, const Card& b);
bool operator < (const Card& a, const Card& b);
bool operator == (const Card& a, const Card& b);
int usageMessage(string arg);

//int fileParser(vector<Card> &cardvec, char * filename);
int printCards(vector<Card> &cards);


Hand checkHand(Hand &prehand);
int getRank(char rChar, Card::Rank &rank);
int getSuit(const char sChar, Card::Suit &suit);

string cardToString(const Card & card);

Hand check7Hand(Hand &prehand);
#endif
