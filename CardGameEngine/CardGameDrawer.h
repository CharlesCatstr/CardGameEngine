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

	string BaronessRowHeadingTemplate = "   [X]   #";

	string DeckCounterTemplate =
		R"( _____   #
|     |  #
|     |  #
| X  |  #
|     |  #
|_____|  #
         #
 CARDS   #
 LEFT    #
         #
         #
         #
         #
         #
         #
         #
         #
)";

	int TemplateLineCount = -1; // Gets recalculated on construct

public:

	CardGameDrawer();

	string GetTemplate();
	string GetStackTemplate();
	string GetEmptyTemplate();
	string GetDeckCounterTemplate();
	string GetBaronessRowHeadingTemplate();
	string GetCardWidthAsWhitespace();
	int GetTemplateLineCount();

	string FormatCard(Card& GivenCard, bool isStacked);
	string FormatCard(Suit CardSuit, int CardValue, bool IsStacked);
	string FormatCard(Suit CardSuit, int CardValue);
	string FormatCard(Card& GivenCard);
	string FormatMultipleCards(vector<Card> CardsToFormat);
	string FormatPile(Pile);
	string FormatBaroness(vector<Pile> pilesToFormat, int deckRemainingCards);

};
