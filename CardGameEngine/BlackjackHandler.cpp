#include "BlackjackHandler.h"
#include <iostream>

using namespace std;


int BlackjackHandler::GetBlackjackRoundResult(int PlayerTotal, int DealerTotal) {
    if (PlayerTotal == 21 and DealerTotal == 21) {
        cout << "Push! (You and the dealer both have Blackjack)\nNo one wins";
        return 0;
    }
    else if (DealerTotal == 21) {
        cout << "Dealer has Blackjack!\nYou lose!\n";
        return 1; // 1 means dealer won
    }
    else if (PlayerTotal == 21) {
        cout << "You have Blackjack!\nYou win!\n";
        return 2; // 2 means player won
    }
    // No one got blackjack, return -1
    return -1;
}
