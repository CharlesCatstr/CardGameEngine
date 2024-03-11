#pragma once

#include <String>;
#include <vector>;
#include <map>;
#include <random>;

#include "Resources.h";
#include "Card.h";

using namespace std;

class Deck
{

	vector<Card> CardsInPlay;
	vector<Card> RemovedCards;

public:

	Deck();

	Card DrawCard();
	vector<Card> GetAllCards();

	void Shuffle();
	void Refill();
	void Reset();

	int RemainingCards();

};

