#include "stdafx.h"
#include "Cards.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Hand.h"

using namespace std;
const char* Card::suitArray[4] = { "C", "D", "H", "S" };
const char* Card::rankArray[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
const char* Hand::handrankArray[9] = { "straight flush", "four_of_a_kind", "full_house", "flush", "straight", "three_of_a_kind", "two_pair", "one_pair", "high_card" };





/*operator test */
bool operator == (const Card& a, const Card& b) {
    if (a.rank == b.rank) {
        if (a.suit == b.suit) {
            return true;
        }
        else return false;
        
    }
    else return false;
    
    
    
    
    
}

bool operator < (const Card& a, const Card& b) {
    if (a.rank < b.rank) {
        return true;
    }
    else if (a.rank == b.rank) {
        return (a.suit < b.suit);
    }
    else {
        return false;
    }
    
}
bool operator > (const Card& a, const Card& b) {
    if (a.rank > b.rank) {
        return true;
    }
    else if (a.rank == b.rank) {
        return (a.suit > b.suit);
    }
    else {
        return false;
    }
    
}
/*helper functions to convert to proper suit and rank format */
int getSuit(char s, Card::Suit &suit) {
    switch (s)
    {
        case 'c':
        case 'C':
            suit = Card::Suit::clubs;
            return 0;
            break;
            
        case 'd':
        case 'D':
            suit = Card::Suit::diamonds;
            return 0;
            break;
            
        case 'h':
        case 'H':
            suit = Card::Suit::hearts;
            return 0;
            break;
            
        case 's':
        case 'S':
            suit = Card::Suit::spades;
            return 0;
            break;
            
            
        default:
            return 1;
            break;
    }
    
    return 1;
}

int getRank(char r, Card::Rank &rank) {
    
    switch (r) {
        case '2':
            rank = Card::Rank::two;
            return 0;
            
            break;
            
        case '3':
            rank = Card::Rank::three;
            return 0;
            
            break;
            
        case '4':
            rank = Card::Rank::four;
            return 0;
            break;
            
        case '5':
            rank = Card::Rank::five;
            return 0;
            break;
            
        case '6':
            rank = Card::Rank::six;
            return 0;
            break;
            
        case '7':
            rank = Card::Rank::seven;
            return 0;
            break;
            
        case '8':
            rank = Card::Rank::eight;
            return 0;
            break;
            
        case '9':
            rank = Card::Rank::nine;
            return 0;
            break;
            
        case 't':
            rank = Card::Rank::ten;
            return 0;
            break;
            
        case 'j':
        case 'J':
            
            rank = Card::Rank::jack;
            return 0;
            break;
            
        case 'q':
        case 'Q':
            
            rank = Card::Rank::queen;
            return 0;
            break;
            
        case 'k':
        case 'K':
            rank = Card::Rank::king;
            return 0;
            break;
            
        case 'a':
        case 'A':
        case '1':
            
            rank = Card::Rank::ace;
            return 0;
            break;
            
        default:
            
            return 1;
            break;
            
    }
    
    
}




int printCards(vector<Card> &cards) {
    /* since we cannot directly print out c.rank and c.suit because rank and suit are enum types, we use an array with the enum as key and proper
     format as value, to print out the correct value. */
    
    for (Card c : cards) {
        cout << Card::rankArray[c.rank] << Card::suitArray[c.suit] << endl;
        
        
    }
    return 0;
    
}

int usageMessage(string arg) {
    
    cout << "error processing file" << arg << "the file should be " << endl;
    return 3;
    
    
}

string cardToString(const Card & card) {
    string rank = Card::rankArray[card.rank];
    string suit = Card::suitArray[card.suit];
    return rank + suit;
    
}

vector<int> people;
vector<int> combination;
vector<vector<int> > v2;

void go(int offset, int k) {
    if (k == 0) {
        v2.push_back(combination);
        return;
    }
    for (size_t i = offset; i <= people.size() - k; ++i) {
        combination.push_back(people[i]);
        go(i + 1, k - 1);
        combination.pop_back();
    }
}

Hand check7Hand(Hand &prehand) {
    //why do you need a sevencardvec? - MW
    std::vector<Card> sevencardvec = prehand.cardvec;
    std::vector<Card> fivecardvec;
    std::vector <Hand> handvec;
    int n = 7, k = 5;
    //std::vec
    for (int i = 0; i < n; ++i) { people.push_back(i); }
    go(0, k);
    //std::cout << "Check7Hand" << endl;
    // v2 is a vector of differnent vectors 12345, 12346----they are;
    //std::cout << "test1" << endl; - MW
    for (const std::vector<int> &v : v2)
    {
        //std::cout << "test2" << endl; - MW
        int m = 0;
        for (int x : v) {
            //std::cout << "test3" << endl; - MW
            //std::cout << x << endl;
            fivecardvec.push_back(sevencardvec[x]);
            
        }
        //Hand a =
        //std::cout << "test4" << endl; - MW
        handvec.push_back(checkHand(Hand(fivecardvec, 1)));
        fivecardvec.clear();
        
        //std::cout << x << ' ';
        //std::cout << std::endl;
    }
    //std::cout << "Check7Hand2" << endl;
    //std::cout << "test5" << endl; - MW
    //std::cout << handvec[0] << handvec[0].handInt << endl; - MW
    //std::cout << handvec[20] << handvec[0].handInt << endl; - MW
    //std::cout << "Hand vec size is " << handvec.size() << endl;
    
    //for (size_t i = 0; i < handvec.size(); ++i) {
    //	std::cout << "Check7Hand for loop print" << endl;
    //	std::cout << handvec[i].size() << endl;
    //	std::cout << handvec[i] << endl;
    //}
    
    
    // -MWstd:cout << "HANDVEC SIZE" << handvec.size() << endl;
    try {
        
        
        
        
        
        // -MWstd::cout << "working " << endl;
        std::sort(handvec.begin(), handvec.end(), poker_rank);
        // -MWstd::cout << "working " << endl;
    }
    catch (const std::exception &exc) {
        std::cout << exc.what();
        std::cout << "caught" << endl;
    }
    catch (...) {
        // -MW	std::cout << "caught" << endl;
        
    }
    // -MW std::cout << "test6" << endl; - MW
    //std::cout << handvec[20] << endl;
    people.clear();
    combination.clear();
    v2.clear();
    return handvec[0];
    
}


Hand checkHand(Hand &prehand) {
    int handRankInt = 1;
    //Hand::handRank hr = Hand::handRank::high_card;
    //Hand h = Hand(cards, handRankInt);
    prehand.handInt = 1;
    
    std::sort(prehand.cardvec.begin(), prehand.cardvec.end());
    int counts[13] = {};
    int suits[4] = {};
    
    
    
    for (size_t i = 0; i < prehand.cardvec.size(); ++i)
    {
        
        counts[prehand.cardvec[i].rank] += 1;
        suits[prehand.cardvec[i].suit] += 1;
    }
    
    //process the rank of card
    
    //string score = "high card";
    int pair_count = 0;
    int three_count = 0;
    int four_count = 0;
    for (int i = 0; i < 13; ++i)
    {
        if (counts[i] == 2)
        {
            ++pair_count;
            
        }
        else if (counts[i] == 3)
        {
            ++three_count;
        }
        else if (counts[i] == 4)
        {
            ++four_count;
        }
        
    }
    
    
    //flush detector
    
    bool flush = false;
    for (int i = 0; i < 4; i++)
    {
        if (suits[i] == 5)
        {
            
            flush = true;
            break;
        }
    }
    
    const int spread5 = prehand.cardvec[prehand.cardvec.size() - 1].rank - prehand.cardvec[0].rank;
    
    
    
    if (flush) {
        
        //now flush
        if (spread5 == 4 && pair_count == 0 && three_count != 1 && four_count != 1) {
            //h.hr = Hand::handRank::straight_flush;
            prehand.handInt = 9;
            //score = "striaight flush";
        }
        else {
            //h.hr = Hand::handRank::flush;
            //score = "flush";
            prehand.handInt = 6;
        }
        
    }
    else {
        //check for straight
        if (spread5 == 4 && pair_count == 0 && three_count != 1 && four_count != 1)
        {
            //h.hr = Hand::handRank::straight;
            prehand.handInt = 5;
            // now it is a straight
            //score = "straight";
        }
        
        else if (four_count == 1)
        {
            //h.hr = Hand::handRank::four_of_a_kind;
            prehand.handInt = 8;
            //now it is a four of a kind
            //score = "four of a kind";
        }
        
        else if (three_count == 1)
        {
            if (pair_count == 1)
            {
                //h.hr = Hand::handRank::full_house;
                prehand.handInt = 7;
                //score = "full house";
                // now it is a full house  (AAABB)
            }
            else
            {
                //h.hr = Hand::handRank::three_of_a_kind;
                prehand.handInt = 4;
                //score = "three of a kind";
                //  it is only three of a kind
                
            }
        }
        else if (pair_count == 2)
        {
            //h.hr = Hand::handRank::two_pair;
            prehand.handInt = 3;
            //score = "two pairs";
            // two pairs
        }
        else if (pair_count == 1)
        {
            
            //h.hr = Hand::handRank::one_pair;
            prehand.handInt = 2;
            //score = " one pair";
            //one pair
        }
        else {
            //score = "high card";
        }
        
        
        
    }
    //print out the score of the hand
    return prehand;
    //cout << score << endl;
    
}




bool poker_rank(Hand& a, Hand& b) {
    // -MWstd::cout << a << endl;
    // -MWstd::cout << b << endl;
    //bool equal = false;
    //if (a.cardvec.size() != b.cardvec.size()) {
    //	equal = false;
    //}
    //else {
    //	auto ca = a.cardvec.begin();
    //	auto cb = b.cardvec.begin();
    
    //	while (ca != a.cardvec.end()) {
    //		if (!((*ca).rank == (*cb).rank)) {
    //			equal = false;
    //		}
    //		ca++;
    //		cb++;
    //	}
    //	equal = true;
    //}
    //
    //if (equal) {
    //	//std::cout << "equal" << endl;
    //	return false;
    //}
    
    bool equal = true;
    if (a.cardvec.size() == b.cardvec.size()) {
        auto ca = a.cardvec.begin();
        auto cb = b.cardvec.begin();
        
        while (ca != a.cardvec.end()) {
            if ((*ca).rank != (*cb).rank) {
                equal = false;
            }
            ca++;
            cb++;
        }
        if (equal) {
            for (int i = 0; i < 5; ++i) {
                if (a.cardvec[i].suit != b.cardvec[i].suit) {
                    // -MW std::cout << (a.cardvec[i].suit > b.cardvec[i].suit) << endl;
                    return a.cardvec[i].suit > b.cardvec[i].suit;
                }
            }
            return false;
            //return a.cardvec[0].suit > b.cardvec[0].suit;
        }
    }
    
    if (a.handInt == b.handInt) {
        //cout << "the same is " << a.handInt << "and " << b.handInt << "and the result is " << (a.handInt > b.handInt) << endl;
        
        //if it is a straight flush or straight , sort by the highest card
        if (a.handInt == 9 || a.handInt == 5)
        {
            // -MWstd::cout << "9 or 5" << endl;
            
            return (a.cardvec[4].rank > b.cardvec[4].rank);
            
        }
        //four of a kind or three of a kind, just compare the middle card
        else if (a.handInt == 8 || a.handInt == 4) {
            // -MW std::cout << "8 or 4" << endl;
            
            Card middlea = a.cardvec[2];
            
            Card middleb = b.cardvec[2];
            
            
            
            return (middlea.rank > middleb.rank);
            
        }
        //full house, three cards first, then two cards.
        else if (a.handInt == 7) {
            // -MW std::cout << "7" << endl;
            
            Card middlea = a.cardvec[2];
            
            Card middleb = b.cardvec[2];
            if (middlea.rank > middleb.rank) {
                // good , we only need to compare the three same cards
                return true;
            }
            else if (a.cardvec[0].rank == b.cardvec[0].rank) {
                //this is the type AAABB and AAACC , so compare the fourth card
                return(a.cardvec[3].rank > b.cardvec[3].rank);
            }
            else {
                return (a.cardvec[0].rank > b.cardvec[4].rank);
                
            }
            
            
            
        }
        
        
        
        else if (a.handInt == 6 || a.handInt == 1) {
            // -MW std::cout << "6 or 1" << endl;
            //right now it is a flush,, or a high card check from the last position in the card array and if encounter any card from a smaller than b, return false, break
            
            //for(auto x = a.cards.end();x>0;x--){
            for (int i = (int)(a.cardvec.size() - 1); i >= 0; i--) {
                // -MW std::cout << "loop" << endl;
                // -MW std::cout << i << endl;
                
                if (a.cardvec[i].rank < b.cardvec[i].rank) {
                    // -MW std::cout << "high card false" << endl;
                    return false;
                }
                
                
            }
            // -MW std::cout << "high card true" << endl;
            
            return true;
            
        }
        else if (a.handInt == 3) {
            // -MW std::cout << "3" << endl;
            
            //cout << "gotyou" << endl;
            
            // We first want to process the two hands to see where the large pair 
            // and the small  pairs are locaeted 
            
            
            auto ca = a.cardvec.cend();
            auto cb = b.cardvec.cend();
            
            Card alarge;
            Card asmall;
            Card afive;
            Card blarge;
            Card bsmall;
            Card bfive;
            
            if (a.cardvec[4] == a.cardvec[3])
            {
                
                alarge = a.cardvec[4];
                
                //alarge.rank = a.cardvec[4].rank;
                
                if (a.cardvec[2].rank == a.cardvec[1].rank) {
                    
                    asmall = a.cardvec[2];
                    afive = a.cardvec[0];
                    
                }
                else {
                    asmall = a.cardvec[0];
                    
                    afive = a.cardvec[2];
                    
                    
                }
                
                
            }
            
            else {
                afive = a.cardvec[4];
                alarge = a.cardvec[3];
                asmall = a.cardvec[0];
                
                
            }
            
            
            if (b.cardvec[4].rank == b.cardvec[3].rank)
            {
                blarge = b.cardvec[4];
                
                if (b.cardvec[2].rank == b.cardvec[1].rank) {
                    
                    bsmall = b.cardvec[2];
                    bfive = b.cardvec[0];
                    
                }
                else {
                    bsmall = b.cardvec[0];
                    
                    bfive = b.cardvec[2];
                    
                    
                }
                
                
            }
            
            else {
                bfive = b.cardvec[4];
                blarge = b.cardvec[3];
                bsmall = b.cardvec[0];
                
                
            }
            
            if (alarge.rank == blarge.rank) {
                if (asmall.rank == bsmall.rank) {
                    return (afive.rank > bfive.rank);
                    
                }
                else return (asmall.rank > bsmall.rank);
                
            }
            
            
            else return (alarge.rank > blarge.rank);
            
            
            
        }
        
        else if (a.handInt == 2) {
            // now we are dealing with both hands have one pair, 
            // use  a while loop to determine where the pair is located
            auto ca = a.cardvec.cend();
            auto cb = b.cardvec.cend();
            Card apair;
            Card bpair;
            
            for (int x = 4; x >0; x--) {
                
                if (a.cardvec[x].rank == a.cardvec[x - 1].rank) {
                    apair = a.cardvec[x];
                    break;
                }
                
                
                
            }
            for (int y = 4; y >0; y--) {
                Card::Rank ranka = b.cardvec[y].rank;
                Card::Rank rankb = b.cardvec[y - 1].rank;
                bool rankeq = (b.cardvec[y].rank == b.cardvec[y - 1].rank);
                if (b.cardvec[y].rank == b.cardvec[y - 1].rank) {
                    bpair = b.cardvec[y];
                    break;
                }
                
                
                
            }
            
            
            
            if (apair.rank == bpair.rank) {
                
                for (int i = 4; i >= 0; --i) {
                    
                    if (a.cardvec[i].rank < b.cardvec[i].rank) {
                        // -MW std::cout << "false" << endl;
                        return false;
                    }
                    
                }
                // -MW std::cout << "true" << endl;
                return true;
            }
            
            
            else {
                // -MW std::cout << "great than or equal" << endl;
                return (apair > bpair);
            }
            
            
        }
        
        
        else {
            
            // -MW std::cout << "no catch" << endl;
            return false;
        }
        
        
        
    }
    
    
    
    
    else {
        
        
        
        // -MW std::cout << "no catch2" << endl;
        
        return (a.handInt > b.handInt);
    }
    
    
    
    
    
}

