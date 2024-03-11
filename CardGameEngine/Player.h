#pragma once

#include "Card.h"
#include "Resources.h"
#include <vector>

class Player
{

	vector<Card> PlayerHand = {};

public:

	Player();
	vector<Card> GetPlayerHand();
	void EmptyPlayerHand();
	void AddCardToHand(Card& NewCard);
	int GetHandTotal();

};

