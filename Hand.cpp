#include "stdafx.h"
#include "Cards.h"
#include "Hand.h"

#include <sstream>
#include <iterator>
#include <algorithm>


Hand& Hand::operator=(const Hand& h) {
    
    if (*this == h) {
        return *this;
    }
    else {
        
        cardvec.clear();
        
        std::copy(h.cardvec.begin(), h.cardvec.end(), std::back_inserter(cardvec));
        (*this).handInt = h.handInt;
        
        return *this;
    }
}

const int Hand::size() {
    return cardvec.size();
}

std::string Hand::asString() const {
    std::string to_return = "\n";
    for (Card c : cardvec) {
        to_return += cardToString(c) + "\n";
    }
    return to_return;
}



std::ostream& operator<<(std::ostream&out, const Hand& h) {
    out << h.asString();
    return out;
}

Hand& operator<<(Hand& h, Deck& d) {
    
    h.cardvec.push_back(d.cardvec.back());
    
    d.cardvec.pop_back();
    
    std::sort(h.cardvec.begin(), h.cardvec.end());
    
    
    checkHand(h);
    
    return h;
}




bool Hand::operator ==(const Hand& h) {
    
    if (cardvec.size() != h.cardvec.size()) {
        return false;
    }
    else {
        auto ca = cardvec.cbegin();
        auto cb = h.cardvec.begin();
        
        
        while (ca != cardvec.end()) {
            if (!(*(ca) == *(cb))) {
                return false;
            }
            ca++;
            cb++;
        }
        return true;
    }
}


bool Hand::operator<(const Hand& h) {
    
    std::vector<Card> ::const_iterator beginning;
    std::vector<Card> ::const_iterator altbeginning;
    beginning = cardvec.cbegin();
    altbeginning = h.cardvec.cbegin();
    
    
    
    while (beginning != cardvec.cend())
    {
        if (*beginning < *altbeginning) {
            return true;
            
        }
        
        beginning++;
        altbeginning++;
        
    }
    return false;
}



Card& Hand::operator[](const size_t x) {
    
    if (x >= 0 && x < (unsigned int) size()) {
        //if (x < size()) {
        return cardvec[x];
        
    }
    
    else {
        // throw an error that the card trying to find is out of bounds
        throw "the card trying to access in hand is out of bounds";
    }
    
}


void Hand::remove_card(const size_t x) {
    
    if (x >= 0 && x < (unsigned int) size()) {
        cardvec.erase(cardvec.begin() + x);
        
        
    }
    
    else {
        // throw an error that the card trying to find is out of bounds
        cout << "the card trying to delete in hand is out of bounds" << endl;
    }
    
    
    
}
