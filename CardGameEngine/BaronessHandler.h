#pragma once

#include "Deck.h"
#include "Player.h"
#include "Pile.h"
#include "CardGameDrawer.h"

class BaronessHandler
{
	int numPiles = 5;
	vector<Pile> allPiles;
	CardGameDrawer drawer;

	bool gameIsActive;
	bool debugMode;

	Deck activeDeck;

public:

	BaronessHandler();
	BaronessHandler(CardGameDrawer& inDrawer, Deck& standardDeck);

	int PlayBaroness(Player& realPlayer);
	bool IsGameActive();
	void SetGameActive(bool isActive);
	void SetDebugMode(bool isDebug);
	bool DoThirteensExist();
	void AppendAllPiles();

	void UserSelectThirteenCards();
	void DisplayActions();
	int GetUserAction(vector<string> validOptions);
};

