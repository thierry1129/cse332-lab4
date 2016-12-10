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
int SevenCardStud::before_turn(Player &p) {
	return 0;

}
int SevenCardStud::turn(Player &p) {
	return 0;

}
int SevenCardStud::allFoldWinner() {
	return 0;

}


int SevenCardStud::after_turn(Player &p) {
	cout << p.playerName << " " << p.playerHand << endl;
	return 0;
}

void SevenCardStud::bet(Player &p) {

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

					std::cout << p.playerName << ": going all in for you " << std::endl;
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
			std::cout << p.playerName << ": no more chips, must check" << std::endl;
			return;
		}

		std::cout << p.playerName << ": now there is no bet on table, either fold, check or bet or megabet" << std::endl;
		std::cout << p.playerName << ": your current chip count is: " << p.chipCount << endl;
		std::string action;
		std::cin >> action;
		while (!playerroundfinished) {
			if (action == "fold") {
				++checks;
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
					std::cout << p.playerName << ": bet two chip" << endl;
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
int SevenCardStud::firstTurnCard() {

	size_t pos = 0;
	if (dealer != (playervec.size() - 1)) {
		pos = dealer + 1;
	}
	int totalCards = playervec.size(); //dealing the face up cards
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
	totalCards = 2 * playervec.size(); //dealing cards face down
	while (totalCards > 0) {
		// now deal the card when there are still cards to deal.
		(playervec[pos])->playerHand << main_deck;
		int cardsInHand = (playervec[pos])->playerHand.size() - 1;
		(playervec[pos])->playerHand.make_faceDown(cardsInHand); //after dealing the card to that position, make it face down

		pos = (pos + 1) % playervec.size();
		--totalCards;
	}

	pos = 0;
	if (dealer != (playervec.size() - 1)) {
		pos = dealer + 1;
	}

	int numplayers = playervec.size();

	while (numplayers > 0) {
		// print delat cards





		cout << (*playervec[pos]).playerHand << endl;
		cout << "player " << (*playervec[pos]).playerName << "has a " << (*playervec[pos]).playerHand.cardvec.size();
		pos = (pos + 1) % playervec.size();

		std::cout << "firstturnexecuted" << endl;
		numplayers--;
	}

	size_t postDealer = 0;
	if (dealer != (playervec.size() - 1)) {
		postDealer = dealer + 1;
	}

	// need to ask player for bet before dealing them cards
	// because this is bet phase number one 
	pos = postDealer;

	while ((!around || bet_leader != nullptr) && allFold == false) {
		if (checks == playervec.size()) {
			break;
		}
		bet(*playervec[pos]);
		(*playervec[pos]).playerHand.asString();
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


	return 0;
}

int SevenCardStud::firstTurn(Player &p) {


	size_t pos = 0;
	if (dealer != (playervec.size() - 1)) {
		pos = dealer + 1;
	}

	int numplayers = playervec.size();

	while (numplayers > 0) {
		// print delat cards





		cout << (*playervec[pos]).playerHand << endl;
		cout << "player " << (*playervec[pos]).playerName << "has a " << (*playervec[pos]).playerHand.cardvec.size();
		pos = (pos + 1) % playervec.size();

		std::cout << "firstturnexecuted" << endl;
		numplayers--;
	}

	size_t postDealer = 0;
	if (dealer != (playervec.size() - 1)) {
		postDealer = dealer + 1;
	}

	// need to ask player for bet before dealing them cards
	// because this is bet phase number one 
	pos = postDealer;

	while ((!around || bet_leader != nullptr) && allFold == false) {
		if (checks == playervec.size()) {
			break;
		}
		bet(*playervec[pos]);
		(*playervec[pos]).playerHand.asString();
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


	return 0;
}
int SevenCardStud::twoThrFourTurnCard() {
	checks = 0;
	int posa = 0;
	if (dealer != (playervec.size() - 1)) {
		posa = dealer + 1;
	}
	int totalCards = playervec.size(); //dealing cards face down
	while (totalCards > 0) {
		// now deal the card when there are still cards to deal.
		(playervec[posa])->playerHand << main_deck;
		int cardsInHand = (playervec[posa])->playerHand.size() - 1;
		//(playervec[posa])->playerHand.make_faceDown(cardsInHand);
		posa = (posa + 1) % playervec.size();
		--totalCards;
	}

	//cout << "second executed" << round << endl;
	size_t		pos = 0;
	if (dealer != (playervec.size() - 1)) {
		pos = dealer + 1;
	}

	int numplayers = playervec.size();

	while (numplayers > 0) {






		cout << (*playervec[pos]).playerHand << endl;
		cout << "player " << (*playervec[pos]).playerName << "has a " << (*playervec[pos]).playerHand.cardvec.size();
		pos = (pos + 1) % playervec.size();

		//	cout << "second executed" << round << endl;
		numplayers--;
	}

	size_t postDealer = 0;
	if (dealer != (playervec.size() - 1)) {
		postDealer = dealer + 1;
	}




	around = false;

	bet_leader = &(*playervec[postDealer]);
	// need to ask player for bet before dealing them cards
	// because this is bet phase number one 
	size_t posc = postDealer;

	while ((!around || bet_leader != nullptr) && allFold == false) {
		if (checks == playervec.size()) {
			break;
		}
		bet(*playervec[posc]);
		if (posc + 1 == playervec.size()) {
			posc = 0;
			//around = true;
		}

		else {
			posc = (posc + 1) % playervec.size();
		}
		if (bet_leader != nullptr) {
			if ((*playervec[posc]).playerName == (*bet_leader).playerName) {
				around = true;
			}
		}
	}
	return 0;
}



int SevenCardStud::twoThrFourTurn(Player &p) { //current while loop structure runs the 2nd, 3rd, and 4th rounds in one method call
	int round = 3;  //could also be changed to run once, and call it thrice
	while (round <= 5) {


		//print card

		cout << "second executed" << round << endl;
		size_t		pos = 0;
		if (dealer != (playervec.size() - 1)) {
			pos = dealer + 1;
		}

		int numplayers = playervec.size();

		while (numplayers > 0) {






			cout << (*playervec[pos]).playerHand << endl;
			cout << "player " << (*playervec[pos]).playerName << "has a " << (*playervec[pos]).playerHand.cardvec.size();
			pos = (pos + 1) % playervec.size();

			cout << "second executed" << round << endl;
			numplayers--;
		}

		size_t postDealer = 0;
		if (dealer != (playervec.size() - 1)) {
			postDealer = dealer + 1;
		}







		// need to ask player for bet before dealing them cards
		// because this is bet phase number one 
		size_t posc = postDealer;

		while ((!around || bet_leader != nullptr) && allFold == false) {
			if (checks == playervec.size()) {
				break;
			}
			bet(*playervec[posc]);
			if (posc + 1 == playervec.size()) {
				posc = 0;
				//around = true;
			}

			else {
				posc = (posc + 1) % playervec.size();
			}
			if (bet_leader != nullptr) {
				if ((*playervec[posc]).playerName == (*bet_leader).playerName) {
					around = true;
				}
			}
		}
		round = round + 1;
	}


	return 0;
}
int SevenCardStud::finalTurnCard() {
	checks = 0;
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
	pos = 0;
	if (dealer != (playervec.size() - 1)) {
		pos = dealer + 1;
	}

	int numplayers = playervec.size();

	while (numplayers > 0) {
		// print delat cards





		cout << (*playervec[pos]).playerHand << endl;
		cout << "player " << (*playervec[pos]).playerName << "has a " << (*playervec[pos]).playerHand.cardvec.size();
		pos = (pos + 1) % playervec.size();

		std::cout << "finalexecuted" << endl;
		numplayers--;
	}

	size_t postDealer = 0;
	if (dealer != (playervec.size() - 1)) {
		postDealer = dealer + 1;
	}


	around = false;

	allFold = false;
	bet_leader = &(*playervec[postDealer]);
	// need to ask player for bet before dealing them cards
	// because this is bet phase number one 
	pos = postDealer;

	while ((!around || bet_leader != nullptr) && allFold == false) {
		if (checks == playervec.size()) {
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
	return 0;

}
int SevenCardStud::finalTurn(Player &p) { //one more final card, face down, and a round of betting

										  // print delat cards
	size_t	pos = 0;
	if (dealer != (playervec.size() - 1)) {
		pos = dealer + 1;
	}

	int numplayers = playervec.size();

	while (numplayers > 0) {
		// print delat cards





		cout << (*playervec[pos]).playerHand << endl;
		cout << "player " << (*playervec[pos]).playerName << "has a " << (*playervec[pos]).playerHand.cardvec.size();
		pos = (pos + 1) % playervec.size();

		std::cout << "finalexecuted" << endl;
		numplayers--;
	}

	size_t postDealer = 0;
	if (dealer != (playervec.size() - 1)) {
		postDealer = dealer + 1;
	}





	// need to ask player for bet before dealing them cards
	// because this is bet phase number one 
	pos = postDealer;

	while ((!around || bet_leader != nullptr) && allFold == false) {
		if (checks == playervec.size()) {
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
	return 0;
}

int SevenCardStud::before_round() {
	// before each round, we need to clear up everything, so we
	// need to set pot equal to zero
	allFold = false;
	checks = 0;
	pot = 0;
	folded_players = 0;
	int ante = 1;
	for (auto a : playervec) {
		if (a->chipCount >= ante) {
			a->bet_put_in = 0;
			// revert all folded player to unfold position
			a->fold = false;
			anteUp(*a);
			//add_pot(*a, ante);
			std::cout << a->playerName << " ante'd" << endl;
		}


		else {
			bankrupt(*a);
		}
	}
	std::cout << "The current pot is: " << pot << endl;
	main_deck.shuffle();


	return 0;
}

int SevenCardStud::round() {


	firstTurnCard();

	for (int a = 0; a < 3; ++a) {
		twoThrFourTurnCard();
	}
	finalTurnCard();






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
		// -MW std::cout << a->playerHand << endl;
		// -MW std::cout << b->playerHand << endl;

		Hand ah = check7Hand(a->playerHand);
		Hand bh = check7Hand(b->playerHand);


		return poker_rank(ah, bh);
	}
}

int SevenCardStud::after_round() {



	std::vector<std::shared_ptr<Player>> tempplayervec;
	for (size_t i = 0; i < playervec.size(); ++i) {
		//Player did not fold
		if (!(*playervec[i]).fold) {
			tempplayervec.push_back(playervec[i]);
		}
		else {
			std::cout << (playervec[i])->playerName << " folded. They have " << (playervec[i])->handWon << " wins and " << (playervec[i])->handLost
				<< " losses with a current chipCount" << (playervec[i])->chipCount << std::endl;
		}
	}
	/*if (tempplayervec.size() <= 1 && ifgameend) {
	std::cout << "everyone folded in the before round and you are the only one left so you won" << std::endl;
	(*(*tempplayervec.begin())).handWon++;
	(*(*tempplayervec.begin())).chipCount += pot;
	pot = 0;
	//	(*tempplayervec.begin())->
	std::cout << (*(*tempplayervec.begin())).playerName << " has won and his winning time is " << (*(*tempplayervec.begin())).handWon;


	}


	else {*/
	//std::cout << "tempplayervec size" << tempplayervec.size() << endl;
	sort(tempplayervec.begin(), tempplayervec.end(), SevenCardStud::handCompare);
	//std::cout << "aftersort" << endl;
	auto winner = tempplayervec.begin();
	//std::cout << (*winner)->playerHand << " " << (*winner)->playerName << endl;
	auto hand1 = check7Hand((*winner)->playerHand);
	auto hand2 = check7Hand((*(winner + 1))->playerHand);
	std::cout << hand1 << endl;
	std::cout << hand2 << endl;
	int winnernum = 0; //number of Players that tied
	int ties = 0;
	std::vector<std::shared_ptr<Player>> winnerplayervec;
	//std::cout << "aftersort" << endl;
	for (auto player = tempplayervec.begin(); player != tempplayervec.end(); ++player) {
		std::cout << (**player).playerHand << endl;
		//VERSION 3
		//std::cout << (**player).playerHand << (**winner).playerHand << poker_rank((**player).playerHand, (**winner).playerHand) << endl;
		if (poker_rank((**player).playerHand, (**winner).playerHand)) {
			++((*player)->handWon);
			(*player)->chipCount += pot;
			pot = 0;
			std::cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost
				<< " losses" << " handInt = " << (**player).playerHand.handInt << "with a current chipCount" << (**player).chipCount << endl;
		}
		else if (!(poker_rank((**player).playerHand, (**winner).playerHand)) && !(poker_rank((**winner).playerHand, (**player).playerHand))) {
			++((*player)->handWon);
			//if we haven't already determined the number of players that tied
			if (ties == 0) {
				auto iterplayer = player;
				while (!(poker_rank((**iterplayer).playerHand, (**winner).playerHand)) && !(poker_rank((**winner).playerHand, (**iterplayer).playerHand))) {
					++ties;
					if (iterplayer == tempplayervec.end() -1 ) {
						break;
					}
					++iterplayer;
				}
			}
			(*player)->chipCount += pot / ties;
			std::cout << (*player)->playerName << " wins, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl;
		}
		else {
			(*(*player)).handLost++;
			std::cout << (*player)->playerName << " lost, has " << (*player)->handWon << " wins and " << (*player)->handLost << " losses" << " handInt = " << (**player).playerHand.handInt << endl;
		}


		//for (auto winp : winnerplayervec) {
		//	(*winp).chipCount += pot / winnernum;

		//}

		// collect cards from discard deck
	}
	//}
	std::copy(discard_deck.cardvec.begin(), discard_deck.cardvec.end(), std::back_inserter(main_deck.cardvec));
	discard_deck.cardvec.erase(discard_deck.cardvec.begin(), discard_deck.cardvec.end());



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