#include "BaronessHandler.h"

#include <iostream>
#include "Deck.h"
#include "Player.h"
#include "Pile.h"
#include "CardGameDrawer.h"

using namespace std;

BaronessHandler::BaronessHandler(CardGameDrawer& inDrawer, Deck& standardDeck) {
	//int numPiles = 5;
	//vector<Pile> allPiles;

	// Create the piles that will be used in the game

	for (int n = 1; n <= numPiles; n++) {
		Pile nextPile = Pile();
		allPiles.push_back(nextPile);
	}

	drawer = inDrawer;
	activeDeck = standardDeck;

}

void BaronessHandler::AppendAllPiles() {

	int remainingCardCount = activeDeck.RemainingCards();

	//cout << "remaining count:" << remainingCardCount << "\n";

	if (remainingCardCount > 0) {
		for (Pile& nextPile : allPiles) {
			Card nextCard = activeDeck.DrawCard();
			nextPile.AppendCard(nextCard);
			remainingCardCount = activeDeck.RemainingCards();

			//cout << "Added Card\n";

			if (remainingCardCount <= 0) {
				break;
			}
		}
	}
}

int BaronessHandler::PlayBaroness(Player& realPlayer) {
	cout << "> Baroness :: PASS\n";

	// Reset Deck

	activeDeck.Reset();
	
	// Reset all Piles

	for (Pile& nextPile : allPiles) {
		nextPile.Empty();
	}

	// Deal out 5 cards 

	cout << drawer.FormatMultiplePiles(allPiles) << "\n";

	AppendAllPiles();

	cout << drawer.FormatMultiplePiles(allPiles) << "\n";

	AppendAllPiles();

	Card nextCard = activeDeck.DrawCard();
	allPiles[2].AppendCard(nextCard);

	AppendAllPiles();
	AppendAllPiles();

	allPiles[1].PopCard();
	allPiles[1].PopCard();

	nextCard = activeDeck.DrawCard();
	allPiles[4].AppendCard(nextCard);
	nextCard = activeDeck.DrawCard();
	allPiles[4].AppendCard(nextCard);

	cout << drawer.FormatMultiplePiles(allPiles) << "\n";


	return 0;
}