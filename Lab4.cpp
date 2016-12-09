//
//  Lab4.cpp

#include "stdafx.h"
#include "Cards.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include <string>
#include <iostream>
#include <set>

using namespace std;

const char* programName;
const char* gameName;
const int minimumArguments = 4;

int main(int argc, char* argv[]) {
    
    if (argc < minimumArguments) {
        return 23; //not enough arguments passed in
    }
    
    int playerCount = argc - 2;
    std::set<string> uniq{ argv + 2, argv + argc };
    /*
     cout << "set size: " << uniq.size() << endl;
     cout << "player count: " << playerCount << endl;
     set<string>::iterator iter;
     for (iter = uniq.begin(); iter != uniq.end(); ++iter) {
     cout << *iter << " ";
     }
     cout << endl;
     */
    if (uniq.size() != playerCount) {
        //throw "cannot have players with the same name in game, please restart with new names.";
        cout << "players cannot have same name, please restart with new names." << endl;
        return 95;
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
                    //MW cout << "test4" << endl;
                    gameInst->before_round();
                    if (gameInst->allFold != false) {
                        gameInst->allFoldWinner();
                    }
                    else {
                        gameInst->around = false;
                        gameInst->checks = 0;
                        gameInst->round();
                        gameInst->after_round();
                        gameInst->around = false;
                        gameInst->checks = 0;
                        //gameInst->dealer = (gameInst->dealer + 1) % gameInst->size();
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
            //cout << "test1" << endl;
            try {
                //start_game method completed?
                //cout << "test2" << endl;
                cardGame.start_game7(gameName);
                //cout << "test2" << endl;
                gameInst = cardGame.instance();
            }
            catch (int e) {
                return e;
            }
            catch (string e) {
                cout << "error: " << e << endl;
            }
            try {
                cout << "test3" << endl;
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
            catch (int e) {
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