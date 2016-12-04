#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>

FiveCardDraw::FiveCardDraw() : dealer(0) {
    
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            Card c = Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit));
            main_deck.add_card(c);
        }
    }
}

int FiveCardDraw::before_turn(Player &player) {
    
    std::cout << endl << player.playerName << "'s hand: " << player.playerHand << endl;
    bool discardInput = false;
    size_t badCardNum = -1;
    while (discardInput == false) {
        std::cout << "How many cards would you like to discard?" << endl;
        
        std::string response;
        std::cin >> response;
        
        int errorcount = 0;
        
        try {
            badCardNum = stoi(response);
        }
        catch (...) {
            std::cout << "Invalid argument. Didn't enter a number" << endl;
        }
        if (badCardNum <= 5 && badCardNum >= 0) {
            discardInput = true;
        }
        
        
        else {
            std::cout << "Invalid argument. Not a number between 1-5" << endl;
            ++errorcount;
            
        }
        
        if (errorcount > 5) {
            throw "too many errors trying to enter the number of cards to discard";
            
            break;
            
        }
        
    }
    
    // now we need to take in which specific card to discard from the player hand
    
    bool cardDiscardInput = false;
    if (badCardNum == 5) {
        for (int i = 0; i<5; ++i) {
            Card discardCard = player.playerHand[0];
            discard_deck.add_card(discardCard);
            player.playerHand.remove_card(0);
            --badCardNum;
        }
    }
    //while (cardDiscardInput == false && badCardNum > 0) {
    while (badCardNum > 0) {
        int error = 0;
        
        std::cout << "Which card(s) do you want to discard? Enter the position one at a time. " << endl;
        
        std::string cardPosResponse;
        std::cin >> cardPosResponse;
        size_t cardPos = -1;
        try {
            cardPos = stoi(cardPosResponse);
        }
        catch (...) {
            std::cout << "invalid argument" << endl;
        }
        if (cardPos < (unsigned int)player.playerHand.size() && cardPos >= 0) {
            //cardDiscardInput = true;
            Card discardCard = player.playerHand[cardPos];
            discard_deck.add_card(discardCard);
            player.playerHand.remove_card(cardPos);
            --badCardNum;
            
        }
        else {
            std::cout << "Invalid input. Not a valid card position." << endl;
            ++error;
            
        }
        if (error > 10) {
            throw " too many errors while trying to decide which card to discard";
            break;
        }
        
        
    }
    return 0;
}

int FiveCardDraw::before_round() {
    
    
    
    
    
    main_deck.shuffle();
    
    // start position should be one past the dealer.
    
    size_t pos = 0;
    if (dealer != (playervec.size() - 1)) {
        
        pos = dealer + 1;
        
    }
    
    
    
    int totalCards = 5 * playervec.size();
    while (totalCards > 0) {
        
        // now deal the card when there are still cards to deal.
        
        
        (playervec[pos])->playerHand << main_deck;
        
        
        pos = (pos + 1) % playervec.size();
        --totalCards;
        
    }
    //for (auto p : playervec) {
    //	try {
    //		before_turn(*p);
    //	}
    //	catch (string e) {
    //		throw;
    //	}
    
    //}
    vector<std::shared_ptr<Player>>::iterator posIter = playervec.begin() + (dealer + 1) % playervec.size();
    for (unsigned int i = 0; i < playervec.size(); ++i) {
        try {
            before_turn(**posIter);
        }
        catch (string e) {
            throw;
        }
        if (posIter == playervec.end() - 1) {
            posIter = playervec.begin();
        }
        else {
            ++posIter;
        }
    }
    
    return 0;
    
}


int FiveCardDraw::turn(Player &p) {
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

int FiveCardDraw::after_turn(Player &p) {
    cout << p.playerName << " " << p.playerHand << endl;
    return 0;
}

int FiveCardDraw::round() {
    for (auto p : playervec) {
        
        if (turn(*p) != 0) {
            return turn(*p);
            
        }
        if (after_turn(*p) != 0) {
            return after_turn(*p);
            
        }
    }
    
    return 0;
    
    
    
    
}

bool FiveCardDraw::handCompare(std::shared_ptr<Player> a, std::shared_ptr<Player> b) {
    if (a == NULL) {
        return false;
        
    }
    if (b == NULL) {
        return true;
    }
    else {
        return poker_rank(a->playerHand, b->playerHand);
    }
}

int FiveCardDraw::after_round() {
    
    std::vector<std::shared_ptr<Player>> tempplayervec(playervec);
    
    sort(tempplayervec.begin(), tempplayervec.end(), FiveCardDraw::handCompare);
    auto winner = tempplayervec.begin();
    
    for (auto player = tempplayervec.begin(); player != tempplayervec.end(); ++player) {
        //if player has a better hand
        if (poker_rank((**player).playerHand, (**winner).playerHand)) {
            (*(*player)).handWon++;
            //			std::cout << " player " << (*player)->playerName << " won with hand " << (*player)->playerHand
            //				<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
            //				<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
            cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost
            << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            
            //HERE
            
        }
        //The hands are already sorted using poker rank
        //so here we're just testing for two hands that have the same type of hand
        else if ((**player).playerHand.handInt == (**winner).playerHand.handInt) {
            //and player and winner are both not better than each other
            if (!(poker_rank((**player).playerHand, (**winner).playerHand)) && !(poker_rank((**winner).playerHand, (**player).playerHand))) {
                (*(*player)).handWon++;
                //				std::cout << " player " << (*player)->playerName << " won with hand " << (*player)->playerHand
                //					<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
                //					<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
                cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            }
            //Player is worse
            else {
                (*(*player)).handLost++;
                //				std::cout << " player " << (*player)->playerName << " lost with hand " << (*player)->playerHand
                //					<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
                //					<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
                cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            }
            
            
            //HERE
            
            
        }
        //player is strictly worse (doesn't have the same type of hand
        else {
            
            (*(*player)).handLost++;
            //			std::cout << " player " << (*player)->playerName << " lost with hand " << (*player)->playerHand
            //				<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
            //				<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
            cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost
            << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            //HERE
            
            
        }
        
        
        // collect cards from discard deck
        
        std::copy(discard_deck.cardvec.begin(), discard_deck.cardvec.end(), std::back_inserter(main_deck.cardvec));
        discard_deck.cardvec.erase(discard_deck.cardvec.begin(), discard_deck.cardvec.end());
        
        
    }
    for (auto player = tempplayervec.begin(); player != tempplayervec.end(); ++player) {
        std::copy((*player)->playerHand.cardvec.begin(), (*player)->playerHand.cardvec.end(), std::back_inserter(main_deck.cardvec));
        (*player)->playerHand.cardvec.erase((*player)->playerHand.cardvec.begin(), (*player)->playerHand.cardvec.end());
    }
    
    // collect cards from winner hand
    
    // now finished processing the cards, , after game, time to remove players
    
    bool leave = true;
    std::cout << "Enter 'yes' to leave, 'no' to stay." << endl;
    
    //FIXME not sure ifleave should be char * or string
    string ifleave;
    std::cin >> ifleave;
    while (leave) {
        if (ifleave == "yes") {
            std::cout << "Enter the name of player leaving..." << endl;
            string leavename;
            //std::getline(std::cin, leavename);
            std::cin >> leavename;
            
            std::shared_ptr<Player> leaveplayer = find_player(leavename);// leavename.c_str());
            if (leaveplayer != NULL) {
                //FIXME 
                //need to save player data to file
                std::ofstream ofs(leavename + ".txt", std::ofstream::out);
                ofs << "Name " << (*leaveplayer).playerName << endl;
                ofs << "Wins " << (*leaveplayer).handWon << endl;
                ofs << "Losses " << (*leaveplayer).handLost << endl;
                ofs.close();
                for (std::vector<std::shared_ptr<Player>>::iterator p = playervec.begin(); p != playervec.end(); ++p) {
                    if ((*p)->playerName == leavename) {
                        playervec.erase(p);
                        return 0;
                        
                    }
                }
                
            }
        }
        
        else if (ifleave == "no") {
            leave = false;
        }
        
        else {
            std::cout << "Please enter 'yes' or 'no'." << endl;
            std::cin >> ifleave;
            
        }
    }
    
    // after finding out who wants to leave, we need to find out who wants to join 
    
    
    bool dojoin = true;
    while (dojoin) {
        std::cout << endl << "Do any players wish to join the game? (yes/no)" << endl;
        string join;
        std::cin >> join;
        if (join == "yes") {
            
            std::cout << "Enter name of joining player..." << endl;
            string joinname;
            //std::getline(std::cin, joinname);
            std::cin >> joinname;
            
            std::shared_ptr<Player> joinplayer = find_player(joinname.c_str());
            if (joinplayer != NULL) {
                // same name exists in the game
                std::cout << "User name exists, please restart the process and enter a different name." << endl;
            }
            else {
                
                add_player(joinname.c_str());
                
            }
        }
        
        else if (join == "no") {
            dojoin = false;
            
        }
        
        else {
            std::cout << "Enter 'yes' or 'no' to decide if joining." << endl;
        }
    }
    ++dealer;
    dealer = dealer % playervec.size();
    return 0;
}


