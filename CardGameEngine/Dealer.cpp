
#include "Dealer.h"
#include "Player.h"

Dealer::Dealer() {}

int Dealer::MakeBlackjackDecision() {

	// Decisions:
	// 1: Hit
	// 2: Stand

	int DealerTotal = GetHandTotal();

	if (DealerTotal >= 17) { 
		// The dealer has to stand if their total is 17 or higher
		return 2;
	}
	else {
		return 1;
	}
}
