#include "stdafx.h"
#include "Game.h"
#include "sevenCardStud.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>

using namespace std;

SevenCardStud::SevenCardStud() : dealer(0), game_bet(0) {
    
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            Card c = Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit));
            main_deck.add_card(c);
        }
    }
    pot = 0;
    main_deck.shuffle();
}

int SevenCardStud::after_turn(Player &p) {
    cout << p.playerName << " " << p.playerHand << endl;
    return 0;
}

void FiveCardDraw::bet(Player &p) {



	if (folded_players == playervec.size() - 1) {
		// now all but one players have folded
		std::cout << p.playerName << ": everyone else has folded, you won!" << std::endl;
		return;
	}


	std::cout << "your name is " << p.playerName << std::endl;
	std::cout << "your hand is " << p.playerHand << std::endl;
	std::cout << "currently , you have " << p.chipCount << "  chips " << std::endl;

	if (!p.fold) {


		std::cout << "the current pot is " << pot << std::endl;
	     std::cout << "current bet on table is " << game_bet << std::endl;
		std::cout << "you already bet " << p.bet_put_in << std::endl;
		std::cout << "you need   " << game_bet - p.bet_put_in << "  chips to meet the call limit " << std::endl;


		// a bool indicator whether current round has bet or not 
		//bool ifgamebet = false; //Made ifgamebet a class variable of Game -MW

		// a bool indicator whether this player has finished or not 

		bool playerroundfinished = false;

		if (ifgamebet) {



			//  there is bet on table, but i already asked all of the players. 
			if (p.playerName == (*bet_leader).playerName) {
				ifgamebet = false;
				bet_leader = nullptr;
			}



			if (playerroundfinished) {
				return;
			}



			// now there is someone betting, you can fold, call, or reraise, or megareraise ;
			// reraise, extra one bet, 
			// mega reraise, extra two bet 


			std::cout << p.playerName << ": there is a bet out there , do you want to fold, call , or reraise, or megareraise " << std::endl;
			std::string action;
			std::cin >> action;

			if (action == "fold") {

				std::cout << p.playerName << " has folded" << std::endl;
				playerroundfinished = true;
				p.fold = true;
				++folded_players;
				return;

			}
			if (action == "call") {
				//if not enough money to call, going all in 
				if (p.chipCount <= game_bet - p.bet_put_in) {
					add_pot(p, p.chipCount);
					playerroundfinished = true;
					std::cout << p.playerName << ": don't have enough money to call, going all in for you " << std::endl;

				}

				else {
					// now just call regularly 
					add_pot(p, game_bet - p.bet_put_in);

					std::cout << p.playerName << ", you chose to call, putting in extra   " << game_bet - p.bet_put_in << "chips for you" << endl;

					std::cout << " now your bet in the game is " << p.bet_put_in << std::endl;
				}

			}
			if (action == "reraise") {
				// player trying to reraise another one chip, checking their pot 
				if (p.chipCount < (game_bet - p.bet_put_in) + 1) {

					add_pot(p, p.chipCount);
					playerroundfinished = true;
					
					std::cout << p.playerName << ": going all in for you " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;
				}


				else if (p.chipCount == (game_bet - p.bet_put_in) + 1) {
					// reraise success, but already all in since no more chips left
					add_pot(p, p.chipCount);
					playerroundfinished = true;
					++game_bet;
					bet_leader = &p;

					std::cout << p.playerName << ": you reraise the pot by one chip, but gone all in " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;

					ifgamebet = true;
				}
				else {
					// reraise success, 
					add_pot(p, (game_bet - p.bet_put_in) + 1);
					playerroundfinished = true;
					++game_bet;
					bet_leader = &p;

					std::cout << p.playerName << ": you reraise the pot by 1 chip " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;

					ifgamebet = true;
				}


			}

			if (action == "megareraise") {
				if (p.chipCount <= (game_bet - p.bet_put_in)) {



					add_pot(p, p.chipCount);
					playerroundfinished = true;
					std::cout << " you are trying to megareraise, but not enough chips, going all in, " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;


				}

				// player trying to reraise by two chips, checking their pot 
				else if (p.chipCount == (game_bet - p.bet_put_in) + 1) {

					add_pot(p, p.chipCount);
					playerroundfinished = true;
					bet_leader = &p;
					game_bet += 1;
					std::cout << p.playerName << ": going all in for you " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;

				}


				else if (p.chipCount == (game_bet - p.bet_put_in) + 2) {
					// reraise success, but already all in since no more chips left
					add_pot(p, p.chipCount);
					playerroundfinished = true;
					game_bet += 2;
					bet_leader = &p;

					std::cout << p.playerName << ": you reraise the pot, but gone all in " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;
					ifgamebet = true;
				}
				else {
					// reraise success, 
					add_pot(p, (game_bet - p.bet_put_in) + 2);
					playerroundfinished = true;
					game_bet += 2;
					bet_leader = &p;

					std::cout << p.playerName << ": you reraise the pot by 2 chip " << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;
					ifgamebet = true;
				}


			}
			else {
				throw "please enter in valid strings, if you want to fold, call, or reraise";
			}
		}




		else {// now there is no bet on the table, you can either bet,check or fold
			  // bet will bet one chip, megabet bets two chips


			if (playerroundfinished) {

				return;
			}

			if (p.chipCount == 0) {
				std::cout << p.playerName << ": no more chips, must check " << std::endl;
				return;
			}

			std::cout << p.playerName << ": now there is no bet on table, either fold, check or bet or megabet" << std::endl;
			std::string action;
			std::cin >> action;

			if (action == "fold") {
				folded_players += 1;
				std::cout << p.playerName << ": has folded" << std::endl;
				playerroundfinished = true;
				p.fold = true;
				return;

			}
			if (action == "check") {
				std::cout << p.playerName << ": has checked" << endl;
				std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
				std::cout << "The current pot is: " << pot << endl;
				playerroundfinished = true;
				return;

			}
			if (action == "bet") {
				game_bet += 1;
				add_pot(p, 1);
				bet_leader = &p;

				ifgamebet = true;

				playerroundfinished = true;
				
				std::cout << p.playerName << ": bet one chip" << std::endl;
				std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
				std::cout << "The current pot is: " << pot << std::endl;
				return;

			}
			if (action == "megabet") {
				if (p.chipCount == 1) {
					game_bet += 1;
					add_pot(p, 1);
					bet_leader = &p;
					std::cout << p.playerName << ": chip not enough , betting one now" << std::endl;
					
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << std::endl;

					ifgamebet = true;

					playerroundfinished = true;
					return;

				}
				else {
					std::cout << p.playerName << ": bet two chip" << std::endl;
					std::cout << p.playerName << ": now the current bet is  " << game_bet << std::endl;
					std::cout << "The current pot is: " << pot << endl;
					game_bet += 2;
					add_pot(p, 2);
					bet_leader = &p;

					ifgamebet = true;

					playerroundfinished = true;
					return;
				}

			}
		}
	}
}

int SevenCardStud::firstTurn(Player &p) {
    size_t pos = 0;
    if (dealer != (playervec.size() - 1)) {
        pos = dealer + 1;
    }
    int totalCards = 2 * playervec.size(); //dealing the face up cards
    while (totalCards > 0) {
        // now deal the card when there are still cards to deal.
        (playervec[pos])->playerHand << main_deck;
        pos = (pos + 1) % playervec.size();
        --totalCards;
    }
    
    pos = 0;
    if (dealer != (playervec.size() - 1)) {
        pos = dealer + 1;
    }
    totalCards = playervec.size(); //dealing cards face down
    while (totalCards > 0) {
        // now deal the card when there are still cards to deal.
        (playervec[pos])->playerHand << main_deck;
        int cardsInHand = (playervec[pos])-> playerHand.size()-1;
        (playervec[pos])->playerHand.make_faceDown(cardsInHand); //after dealing the card to that position, make it face down
        pos = (pos + 1) % playervec.size();
        --totalCards;
    }
    //not sure if this next block is necessary -- had in lab3 method in which cards were dealt
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
    
    int n = 0;
    bool everyoneBet = false;
    
    while (!everyoneBet && bet_leader == nullptr) {
        
        bet(*playervec[n]);
        if (n + 1 == playervec.size()) {
            std::cout << "gone around once" << std::endl;
            everyoneBet = true;
        }
        n = (n + 1) % playervec.size();
    }
    
    return 0;
}

int SevenCardStud::twoThrFourTurn(Player &p) { //current while loop structure runs the 2nd, 3rd, and 4th rounds in one method call
    int round = 3;  //could also be changed to run once, and call it thrice
    while (round <= 5) {
        int pos = 0;
        if (dealer != (playervec.size() - 1)) {
            pos = dealer + 1;
        }
        int totalCards = playervec.size(); //dealing cards face down
        while (totalCards > 0) {
            // now deal the card when there are still cards to deal.
            (playervec[pos])->playerHand << main_deck;
            pos = (pos + 1) % playervec.size();
            --totalCards;
        }
        int n = 0;
        bool everyoneBet = false;
        while (!everyoneBet && bet_leader == nullptr) {
            bet(*playervec[n]);
            if (n + 1 == playervec.size()) {
                std::cout << "gone around once" << std::endl;
                everyoneBet = true;
            }
            n = (n + 1) % playervec.size();
        }
        round = round + 1;
    }
    //not sure if this next block is necessary -- had in lab3 method in which cards were dealt
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

int SevenCardStud::finalTurn(Player &p) { //one more final card, face down, and a round of betting
    int pos = 0;
    if (dealer != (playervec.size() - 1)) {
        pos = dealer + 1;
    }
    int totalCards = playervec.size(); //dealing cards face down
    while (totalCards > 0) {
        // now deal the card when there are still cards to deal.
        (playervec[pos])->playerHand << main_deck;
        int cardsInHand = (playervec[pos])->playerHand.size() - 1;
        (playervec[pos])->playerHand.make_faceDown(cardsInHand); //after dealing the card to that position, make it face down
        pos = (pos + 1) % playervec.size();
        --totalCards;
    }
    //not sure if this next block is necessary -- had in lab3 method in which cards were dealt
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
    
    int n = 0;
    bool everyoneBet = false;
    
    while (!everyoneBet && bet_leader == nullptr) {
        
        bet(*playervec[n]);
        if (n + 1 == playervec.size()) {
            std::cout << "gone around once" << std::endl;
            everyoneBet = true;
        }
        n = (n + 1) % playervec.size();
    }
    
    return 0;
}

int SevenCardStud::before_round() {
    // before each round, we need to clear up everything, so we
    // need to set pot equal to zero
    pot = 0;
    folded_players = 0;
    int ante = 1;
    for (auto a : playervec) {
        if (a->chipCount >= ante) {
            // revert all folded player to unfold position
            a->fold = false;
            
            add_pot(*a, ante);
        }
        else {
            bankrupt(*a);
        }
    }
    main_deck.shuffle();
    
    return 0;
    
}

int SevenCardStud::round() {
    for (auto p : playervec) {
        
        if (firstTurn(*p) != 0) {
            return firstTurn(*p);
        }
        if (twoThrFourTurn(*p) != 0) {
            return twoThrFourTurn(*p);
        }
        if (finalTurn(*p) != 0) {
            return finalTurn(*p);
        }
        after_turn(*p);
    }
    
    return 0;
}

bool SevenCardStud::handCompare(std::shared_ptr<Player> a, std::shared_ptr<Player> b) {
	if (a == NULL) {
		return false;

	}
	if (b == NULL) {
		return true;
	}
	else {


		return poker_rank(check7Hand(a->playerHand),check7Hand( b->playerHand));
	}
}
int SevenCardStud::after_round() {
    
    //std::vector<std::shared_ptr<Player>> tempplayervec(playervec);
    std::vector<std::shared_ptr<Player>> tempplayervec;
    for (size_t i = 0; i < playervec.size(); ++i) {
        //Player did not fold
        if ((*playervec[i]).fold) {
            tempplayervec.push_back(playervec[i]);
        }
        else {
            cout << (playervec[i])->playerName << " folded. They have " << (playervec[i])->handWon << " wins and " << (playervec[i])->handLost
            << " losses with a current chipCount" << (playervec[i])->chipCount << endl;
        }
    }
    
    sort(tempplayervec.begin(), tempplayervec.end(), SevenCardStud::handCompare); //note, handcompare has not yet been implemented (relies on poker rank)
    auto winner = tempplayervec.begin();
    int ties = 0; //number of Players that tied
    for (auto player = tempplayervec.begin(); player != tempplayervec.end(); ++player) {
        //if player has a better hand
        //is this if statement even ever hit? How can the 2nd highest player be strictly better than the 1st highest?
        if (poker_rank((**player).playerHand, (**winner).playerHand)) {
            (*(*player)).handWon++;
            (*player)->chipCount += pot;
            pot = 0;
            
            cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost
            << " losses" << " handInt = " << (**player).playerHand.handInt << "with a current chipCount" << (**player).chipCount << endl << endl;
            
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
                
                cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            }
            //Player is worse
            else {
                (*(*player)).handLost++;
                
                cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl << endl;
            }
            
            
            //HERE
            
            
        }
        //player is strictly worse (doesn't have the same type of hand
        else {
            
            (*(*player)).handLost++;
            
            cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost
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