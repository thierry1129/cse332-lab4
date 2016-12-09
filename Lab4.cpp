//
//  Lab4.cpp

#include "stdafx.h"
#include "Cards.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include <string>
#include <iostream>

using namespace std;

const char* programName;
const char* gameName;
const int minimumArguments = 4;

int main(int argc, char* argv[]) {

	if (argc < minimumArguments) {
		return 23; //not enough arguments passed in
	}
	programName = argv[0];

	FiveCardDraw cardGame;
	shared_ptr<Game> gameInst;
	gameName = argv[1];
	string gName = gameName;
	bool play = true;
	while (play) {
		//cout << gameName << endl;
		string game1 = "FiveCardDraw";
		string game2 = "SevenCardStud";
		if (gName == game1) {
			//cout << "test1" << endl;
			try {
				//start_game method completed?
				//cout << "test2" << endl;
				cardGame.start_game(gameName);
				//cout << "test2" << endl;
				gameInst = cardGame.instance();
			}
			catch (int e) {
				return e;
			}
			catch (string e) {
				//cout << "error: " << e << endl;
			}
			try {
				//cout << "test3" << endl;
				for (int i = 2; i < argc; ++i) {
					gameInst->add_player(argv[i]);
				}
				//gameInst->add_player(argv[2]);
				//gameInst->add_player(argv[3]);

				//if (argc > 4) { //creating players when there are more than just two
				//	for (int i = 4; i < argc; ++i) {
				//		gameInst->add_player(argv[i]);
				//	}
				//}
			}
			catch (string e) {
				cout << "error: " << e << endl;
			}

			while (gameInst->size() >= 2) {
				try {
					cout << "test4" << endl;
					gameInst->before_round();
					if (gameInst->allFold != false) {
						gameInst->allFoldWinner();
					}
					else {
						gameInst->round();
						gameInst->after_round();
					}
					
					if (gameInst->size() < 2) {
						cout << "not enough players, game over" << endl;
					}
				}
				//catch (...) {
				catch (string s) {
					cardGame.stop_game();
					cout << s << endl;
					cout << "game failure 2" << endl;
					break;
				}
				catch (const std::exception &exc) {
					cardGame.stop_game();
					cout << exc.what() << endl;
					cout << "game failure" << endl; //another case of a true error exception?
					break;
				}
			}
		}
		else if (gName == game2) {
			cout << "Now Playing SevenCardStud" << endl;
			break;
		}
		else {
			cout << "Please enter a valid game name (FiveCardDraw, SevenCardStud) or 'quit' to exit the program." << endl;
			string input;
			cin >> input;
			if (input == "quit") {
				play = false;
				//INSERT THE EXIT GAME CODE. EG WRITE OUT THE PLAYER VALUES AND ETC
			}
			else {
				gName = input;
			}
		}
	}
	return 0;
}