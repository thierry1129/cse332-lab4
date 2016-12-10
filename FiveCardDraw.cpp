#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>



FiveCardDraw::FiveCardDraw() : game_bet(0) {
    dealer = 0;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            Card c = Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit));
            main_deck.add_card(c);
        }
    }
}

int FiveCardDraw::before_turn(Player &player) {
    if (player.fold) {
        // this player is folding, end
        return 0;
    }
    else {
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
                ++errorcount;
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
            for (int i = 0; i < 5; ++i) {
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
    }
    return 0;
}

int FiveCardDraw::before_round() {
    // before each round, we need to clear up everything, so we
    // need to set the pot equal to zero
    game_bet = 0;
    pot = 0;
    ifgamebet = false;
    around = false;
    allFold = false;
    checks = 0;
    bet_leader = nullptr;
    pot = 0;
    folded_players = 0;
    int ante = 1;
    for (auto a : playervec) {
        if (a->chipCount >= (unsigned int)ante) {
            a->bet_put_in = 0;
            // revert all folded player to unfold position
            a->fold = false;
            anteUp(*a);
            //add_pot(*a, ante);
            std::cout << a->playerName << " ante'd" << endl;
        }
        
        
        else {
            bankrupt(*a);
            //cout << "GETS BEYOND THE BANKRUPT" << endl;
            if (playervec.size() == 1) {
                //cout << "only one player left, game over." << endl; //printed at beginning of round
                return 0;
            }
            break; //only working for one player at a time
        }
    }
    std::cout << "The current pot is: " << pot << endl;
    main_deck.shuffle();
    
    // start position should be one past the dealer.
    
    size_t postDealer = 0;
    if (dealer != (playervec.size() - 1)) {
        postDealer = dealer + 1;
    }
    
    // need to ask player for bet before dealing them cards
    // because this is bet phase number one
    size_t pos = postDealer;
    
    while ((!around || bet_leader != nullptr) && allFold == false) {
        if (checks == (playervec.size() - folded_players)) {
            break;
        }
        //MW std::cout << "game bet is: " << game_bet << endl;
        //MW std::cout << (*playervec[0]).playerName << " bet amount is: " << (*playervec[0]).bet_put_in << endl;
        //MW std::cout << (*playervec[1]).playerName << " bet amount is: " << (*playervec[1]).bet_put_in << endl;
        //MW std::cout << (*playervec[2]).playerName << " bet amount is: " << (*playervec[2]).bet_put_in << endl;
        
        bet(*playervec[pos]);
        if (pos + 1 == playervec.size()) {
            pos = 0;
            //around = true;
        }
        
        else {
            pos = (pos + 1) % playervec.size();
        }
        if (bet_leader != nullptr) {
            if ((*playervec[pos]).playerName == (*bet_leader).playerName) {
                around = true;
            }
        }
    }
    if (allFold == false) {
        //reset pos to postDealer
        pos = postDealer;
        int playernum = 0;
        for (size_t i = 0; i < playervec.size(); ++i) {
            if ((*playervec[i]).fold == false) {
                ++playernum;
            }
        }
        int totalCards = 5 * playernum;
        while (totalCards > 0) {
            // now deal the card when there are still cards to deal.
            if ((*playervec[pos]).fold == false) {
                (playervec[pos])->playerHand << main_deck;
                pos = (pos + 1) % playervec.size();
                --totalCards;
            }
            else {
                pos = (pos + 1) % playervec.size();
            }
        }
        //int playerfold = 0;
        //for (size_t i = 0; i < playervec.size(); ++i) {
        //	if ((*playervec[i]).fold == false) {
        //		++playerfold;
        //	}
        //}
        //std::cout << "The number of player's that didn't fold " << playerfold << endl;
        vector<std::shared_ptr<Player>>::iterator posIter = (playervec.begin() + (postDealer % playervec.size()));
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
    }
    return 0;
    
}

void FiveCardDraw::bet(Player &p) {
    
    if (folded_players == playervec.size() - 1) {
        // now all but one players have folded
        std::cout << p.playerName << ": everyone else has folded, you won!" << std::endl;
        allFold = true;
        return;
    }
    if (p.fold == true) {
        return;
    }
    
    // a bool indicator whether this player has finished or not
    
    bool playerroundfinished = false;
    
    if (ifgamebet) {
        if (p.playerName == (*bet_leader).playerName) {
            ifgamebet = false;
            bet_leader = nullptr;
            return;
        }
        
        
        // now there is someone betting, you can fold, call, or reraise, or megareraise ;
        // reraise, extra one bet,
        // mega reraise, extra two bet
        std::cout << p.playerName << ": there is a bet out there , do you want to fold, call , or reraise, or megareraise " << endl;
        std::cout << p.playerName << ": your current chip count is: " << p.chipCount << endl;
        std::string action;
        std::cin >> action;
        while (!playerroundfinished) {
            if (action == "fold") {
                std::cout << p.playerName << " has folded" << endl;
                playerroundfinished = true;
                p.fold = true;
                ++folded_players;
                return;
                
            }
            else if (action == "call") {
                //if not enough money to call, going all in
                if (p.chipCount <= game_bet - p.bet_put_in) {
                    //p.bet_put_in += p.chipCount;
                    add_pot(p, p.chipCount);
                    playerroundfinished = true;
                    std::cout << p.playerName << ": don't have enough money to call, going all in for you " << std::endl;
                }
                
                else {
                    // now just call regularly
                    int betAmount = game_bet - p.bet_put_in;
                    //p.bet_put_in += betAmount;
                    add_pot(p, betAmount);
                    std::cout << p.playerName << ", you chose to call, putting in extra " << betAmount << " chips for you" << endl;
                    playerroundfinished = true;
                    
                }
                
            }
            else if (action == "reraise") {
                // player trying to reraise another one chip, checking their pot
                if (p.chipCount < (game_bet - p.bet_put_in) + 1) {
                    //p.bet_put_in += p.chipCount;
                    add_pot(p, p.chipCount);
                    playerroundfinished = true;
                    
                    std::cout << p.playerName << ": going all in" << std::endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                }
                
                
                else if (p.chipCount == (game_bet - p.bet_put_in) + 1) {
                    // reraise success, but already all in since no more chips left
                    //p.bet_put_in += p.chipCount;
                    add_pot(p, p.chipCount);
                    playerroundfinished = true;
                    ++game_bet;
                    bet_leader = &p;
                    ifgamebet = true;
                    std::cout << p.playerName << ": you reraise the pot, but gone all in " << std::endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                }
                else {
                    // reraise success,
                    int betAmount = (game_bet - p.bet_put_in) + 1;
                    //p.bet_put_in += betAmount;
                    add_pot(p, betAmount);
                    playerroundfinished = true;
                    ++game_bet;
                    bet_leader = &p;
                    ifgamebet = true;
                    std::cout << p.playerName << ": you reraise the pot by 1 chip " << std::endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                }
                
                
            }
            else if (action == "megareraise") {
                
                // player trying to reraise another one chip, checking their pot
                if (p.chipCount < (game_bet - p.bet_put_in) + 2) {
                    //p.bet_put_in += p.chipCount;
                    add_pot(p, p.chipCount);
                    playerroundfinished = true;
                    
                    std::cout << p.playerName << ": going all in for you " << std::endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                    if (p.chipCount >(game_bet - p.bet_put_in)) {
                        bet_leader = &p;
                        game_bet += 1;
                    }
                }
                
                
                else if (p.chipCount == (game_bet - p.bet_put_in) + 2) {
                    // reraise success, but already all in since no more chips left
                    //p.bet_put_in += p.chipCount;
                    add_pot(p, p.chipCount);
                    playerroundfinished = true;
                    game_bet += 2;
                    bet_leader = &p;
                    ifgamebet = true;
                    std::cout << p.playerName << ": you reraise the pot, but gone all in " << std::endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                }
                else {
                    // reraise success,
                    int betAmount = (game_bet - p.bet_put_in) + 2;
                    //p.bet_put_in += betAmount;
                    add_pot(p, betAmount);
                    playerroundfinished = true;
                    game_bet += 2;
                    bet_leader = &p;
                    std::cout << p.playerName << ": you reraise the pot by " << betAmount << " chips." << std::endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                    ifgamebet = true;
                }
                
                
            }
            else {
                std::cout << "Please enter in a valid action: fold, call, reraise, or megareraise" << endl;
                std::cin >> action;
            }
        }
    }
    
    else {// now there is no bet on the table, you can either bet,check or fold
        // bet will bet one chip, megabet bets two chips
        
        if (p.chipCount == 0) {
            ++checks;
            std::cout << p.playerName << ": no more chips, must check" << std::endl;
            return;
        }
        
        std::cout << p.playerName << ": now there is no bet on table, either fold, check or bet or megabet" << std::endl;
        std::cout << p.playerName << ": your current chip count is: " << p.chipCount << endl;
        std::string action;
        std::cin >> action;
        while (!playerroundfinished) {
            if (action == "fold") {
                folded_players += 1;
                std::cout << p.playerName << ": has folded" << endl;
                playerroundfinished = true;
                p.fold = true;
                return;
                
            }
            else if (action == "check") {
                ++checks;
                std::cout << p.playerName << ": has checked" << endl;
                std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                std::cout << "The current pot is: " << pot << endl;
                playerroundfinished = true;
                return;
                
            }
            else if (action == "bet") {
                checks = 0;
                std::cout << p.playerName << ": bet one chip" << endl;
                game_bet += 1;
                add_pot(p, 1);
                std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                std::cout << "The current pot is: " << pot << endl;
                //p.bet_put_in += 1;
                bet_leader = &p;
                
                ifgamebet = true;
                
                playerroundfinished = true;
                return;
                
            }
            else if (action == "megabet") {
                if (p.chipCount == 1) {
                    game_bet += 1;
                    add_pot(p, 1);
                    //p.bet_put_in += 1;
                    std::cout << p.playerName << ": chip not enough , betting one now" << std::endl;
                    action = "bet";
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                    
                }
                else {
                    game_bet += 2;
                    add_pot(p, 2);
                    //p.bet_put_in += 2;
                    std::cout << p.playerName << ": bet two chips" << endl;
                    std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
                    std::cout << "The current pot is: " << pot << endl;
                    
                }
                bet_leader = &p;
                
                ifgamebet = true;
                
                playerroundfinished = true;
                
                return;
                
            }
            else {
                std::cout << "Not a valid action. Please enter: fold, check, bet, or megabet." << endl;
                std::cin >> action;
            }
        }
    }
    
}

int FiveCardDraw::turn(Player &p) {
    if (p.fold) {
        // this player is folding, end
        return 0;
    }
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
    std::cout << p.playerName << " " << p.playerHand << endl;
    return 0;
}

int FiveCardDraw::round() {
    //MW std::cout << "prepre bet" << endl;
    //MW std::cout << (*playervec[0]).playerHand << endl;
    //MW std::cout << (*playervec[1]).playerHand << endl;
    //MW std::cout << (*playervec[2]).playerHand << endl;
    if (playervec.size() == 1) {
        cout << "only one player left, game over." << endl;
        return 0;
    }
    
    for (auto p : playervec) {
        
        if (turn(*p) != 0) {
            return turn(*p);
            
        }
        if (after_turn(*p) != 0) {
            return after_turn(*p);
            
        }
    }
    
    size_t pos = 0;
    if (dealer != (playervec.size() - 1)) {
        
        pos = dealer + 1;
        
    }
    while ((!around || bet_leader != nullptr) && allFold == false) {
        if (checks == playervec.size() - folded_players) {
            break;
        }
        
        bet(*playervec[pos]);
        if (pos + 1 == playervec.size()) {
            pos = 0;
            //around = true;
        }
        
        else {
            pos = (pos + 1) % playervec.size();
        }
        if (bet_leader != nullptr) {
            if ((*playervec[pos]).playerName == (*bet_leader).playerName) {
                around = true;
            }
        }
    }
    std::cout << "test" << endl;
    //MW std::cout << "pre bet" << endl;
    //MW std::cout << (*playervec[0]).playerHand << endl;
    //MW std::cout << (*playervec[1]).playerHand << endl;
    //MW std::cout << (*playervec[2]).playerHand << endl;
    //Player * betIter = &(*playervec[pos]);
    //for (size_t i = 0; i < playervec.size(); ++i) {
    //	//*betIter = *playervec[(pos + i) % playervec.size()];
    //	//bet(*betIter);
    //	bet((*playervec[(pos + i) % playervec.size()]));
    //}
    ////MW std::cout << "post bet" << endl;
    ////MW std::cout << (*playervec[0]).playerHand << endl;
    ////MW std::cout << (*playervec[1]).playerHand << endl;
    ////MW std::cout << (*playervec[2]).playerHand << endl;
    
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
    
    //std::vector<std::shared_ptr<Player>> tempplayervec(playervec);
    std::vector<std::shared_ptr<Player>> tempplayervec;
    for (size_t i = 0; i < playervec.size(); ++i) {
        //Player did not fold
        if (!((*playervec[i]).fold)) {
            tempplayervec.push_back(playervec[i]);
        }
        else {
            ++(playervec[i]->handLost);
            playervec[i]->fold = false;
            std::cout << (playervec[i])->playerName << " folded. They have " << (playervec[i])->handWon << " wins and " << (playervec[i])->handLost
            << " losses with a current chipCount " << (playervec[i])->chipCount << endl;
        }
    }
    //MW std::cout << "reached1" << endl;
    //MW std::cout << tempplayervec.size() << endl;
    sort(tempplayervec.begin(), tempplayervec.end(), FiveCardDraw::handCompare);
    //MW std::cout << "reached2" << endl;
    auto winner = tempplayervec.begin();
    int ties = 0; //number of Players that tied
    for (auto player = tempplayervec.begin(); player != tempplayervec.end(); ++player) {
        //if player has a better hand
        //is this if statement even ever hit? How can the 2nd highest player be strictly better than the 1st highest?
        if (poker_rank((**player).playerHand, (**winner).playerHand)) {
            (*(*player)).handWon++;
            (*player)->chipCount += pot;
            pot = 0;
            //			std::cout << " player " << (*player)->playerName << " won with hand " << (*player)->playerHand
            //				<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
            //				<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
            std::cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost
            << " losses" << " handInt = " << (**player).playerHand.handInt << "with a current chipCount " << (**player).chipCount << endl << endl;
            
            //HERE
            
        }
        //The hands are already sorted using poker rank
        //so here we're just testing for two hands that have the same type of hand
        else if ((**player).playerHand.handInt == (**winner).playerHand.handInt) {
            //and player and winner are both not better than each other
            if (!(poker_rank((**player).playerHand, (**winner).playerHand)) && !(poker_rank((**winner).playerHand, (**player).playerHand))) {
                (*(*player)).handWon++;
                //if we haven't already determined the number of players that tied
                if (ties == 0) {
                    while (!(poker_rank((**player).playerHand, (**winner).playerHand)) && !(poker_rank((**winner).playerHand, (**player).playerHand))) {
                        ++ties;
                        ++player;
                    }
                }
                (*player)->chipCount += pot / ties;
                //				std::cout << " player " << (*player)->playerName << " won with hand " << (*player)->playerHand
                //					<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
                //					<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
                std::cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            }
            //Player is worse
            else {
                (*(*player)).handLost++;
                //				std::cout << " player " << (*player)->playerName << " lost with hand " << (*player)->playerHand
                //					<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
                //					<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
                std::cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            }
            
            
            //HERE
            
            
        }
        //player is strictly worse (doesn't have the same type of hand
        else {
            
            (*(*player)).handLost++;
            //			std::cout << " player " << (*player)->playerName << " lost with hand " << (*player)->playerHand
            //				<< "won  " << (*player)->handWon << " times" << " and lost   " << (*player)->handLost
            //				<< " times" << " handInt = " << (**player).playerHand.handInt << endl;
            std::cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost
            << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            //HERE
            
            
        }
        
        // collect cards from discard deck
        
        std::copy(discard_deck.cardvec.begin(), discard_deck.cardvec.end(), std::back_inserter(main_deck.cardvec));
        discard_deck.cardvec.erase(discard_deck.cardvec.begin(), discard_deck.cardvec.end());
        
        
    }
    pot = 0;
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
            //trying to add a remove player method 
            
            std::shared_ptr<Player> leaveplayer = find_player(leavename);// leavename.c_str());
            if (leaveplayer != NULL) {
                
                std::ofstream ofs(leavename + ".txt", std::ofstream::out);
                ofs << "Name " << (*leaveplayer).playerName << endl;
                ofs << "Wins " << (*leaveplayer).handWon << endl;
                ofs << "Losses " << (*leaveplayer).handLost << endl;
                ofs << "chipCount " << (*leaveplayer).chipCount << endl;
                ofs.close();
                for (std::vector<std::shared_ptr<Player>>::iterator p = playervec.begin(); p != playervec.end(); ++p) {
                    if ((*p)->playerName == leavename) {
                        playervec.erase(p);
                        ifleave = "no";
                        break;
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

int FiveCardDraw::allFoldWinner() {
    //just initialize winner to something for now
    auto winner = playervec[0];
    for (auto x : playervec) {
        if ((*x).fold == false) {
            winner = x;
            (*x).chipCount += pot;
            pot = 0;
            ++(*x).handWon;
            std::cout << x->playerName << " wins, has " << x->handWon << " wins and " << x->handLost
            << " losses" << " with a current chipCount " << x->chipCount << endl << endl;
        }
        else {
            (*x).fold = false;
            ++(*x).handLost;
            std::cout << x->playerName << " folded. They have " << x->handWon << " wins and " << x->handLost
            << " losses with a current chipCount " << x->chipCount << endl;
        }
    }
    allFold = false;
    return 0;
    
}