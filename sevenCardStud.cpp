#include "stdafx.h"
#include "Game.h"
#include "sevenCardStud.h"
#include <algorithm>
#include <iostream>

SevenCardStud::SevenCardStud() : dealer(0), game_bet(0) {
    
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            Card c = Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit));
            main_deck.add_card(c);
        }
    }
}

int SevenCardStud::turn(Player &p) {
    int cardsNeeded = cardsPerHand - p.playerHand.size();
    while (cardsNeeded != 0) {
        if (main_deck.size() == 0) {
            if (discard_deck.size() == 0) {
                return 99; //not enough cards in main deck and discard deck to refill player's hand
            }
            else {
                discard_deck.shuffle();
                p.playerHand << discard_deck;
            }
        }
        else {
            p.playerHand << main_deck;
        }
        cardsNeeded = cardsNeeded - 1;
    }
    return 0;
}


int SevenCardStud::after_turn(Player &p) {
    cout << p.playerName << " " << p.playerHand << endl;
    return 0;
}