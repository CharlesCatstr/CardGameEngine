#pragma once
#pragma execution_character_set( "utf-8" )

#include <string>;
#include "Resources.h";
#include "Card.h";
#include <vector>;
#include "Pile.h";

using namespace std;

class CardGameDrawer
{

	string Template = 
	R"(A_____A
|X   |
|     |
|  Y  |
|     |
|___Z|
)";	

	string StackTemplate =
		R"(A_____A
)";

	string EmptyTemplate =
		R"( _ _ _ 
|     |
  ###  
| # # |
  ###  
|_ _ _|
)";

	string CardWidthAsWhitespace = "       ";

	int TemplateLineCount = -1; // Gets recalculated on construct

public:

	CardGameDrawer();

	string GetTemplate();
	string GetStackTemplate();
	string GetEmptyTemplate();
	int GetTemplateLineCount();
	string GetCardWidthAsWhitespace();

	string FormatCard(Card& GivenCard, bool isStacked);
	string FormatCard(Suit CardSuit, int CardValue, bool IsStacked);
	string FormatCard(Suit CardSuit, int CardValue);
	string FormatCard(Card& GivenCard);
	string FormatMultipleCards(vector<Card> CardsToFormat);
	string FormatPile(Pile);
	string FormatMultiplePiles(vector<Pile>);

};
