#pragma once
#pragma execution_character_set( "utf-8" )

#include <string>;
#include "Resources.h";
#include "Card.h";
#include <vector>;

using namespace std;

class CardGameDrawer
{

	string Template = 
	R"( _____ 
|X   |
|     |
|  Y  |
|     |
|___Z|
)";	

	int TemplateLineCount = -1; // Gets recalculated on construct

public:

	CardGameDrawer();

	string GetTemplate();
	int GetTemplateLineCount();
	string FormatCard(Suit CardSuit, int CardValue);
	string FormatCard(Card& GivenCard);
	string FormatMultipleCards(vector<Card> CardsToFormat);


};
