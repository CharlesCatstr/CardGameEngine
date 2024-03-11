#pragma once

#include <string>
#include "Resources.h"

class Card
{
private:

	Suit CardSuit;
	int CardValue;

public:
	// Constructor for the card to accept a suit type and value
	Card();
	Card(Suit N_Suit, int N_Value);

	void SetValue(int N_Value);
	void SetSuit(Suit N_Suit);
	int GetValue();
	Suit GetSuit();
};

