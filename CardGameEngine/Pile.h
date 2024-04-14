#pragma once
#include <vector>
#include "Card.h"

// Pile is similar to a deck but instead of having methods related to drawing and resetting cards, it accepts cards and adds them to an internal stack

class Pile
{

	vector<Card> cardsInPile;		// The cards currently in the pile stack

public:

	Pile();

	void AppendCard(Card inCard);	// Add new card to the stack
	Card& PopCard();				// Remove the top card from the stack and return it
	Card PeekCard();				// Returns the card at the top of the stack
	bool IsEmpty();					
	vector<Card> GetAllCards();		// Get all cards in the pile
	int GetTotalCards();			// Get the number of cards in the pile
	void Empty();					// Remove all cards from the pile 

};

