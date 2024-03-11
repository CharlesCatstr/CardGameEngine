#include "CardGameDrawer.h";
#include "Resources.h";
#include "Card.h";
#include <iostream>;

CardGameDrawer::CardGameDrawer() {
	vector<string> SplitTemplateAsLines = SplitString(Template, '\n');
	TemplateLineCount = SplitTemplateAsLines.size();
};

string CardGameDrawer::GetTemplate() {
	return Template;
}

int CardGameDrawer::GetTemplateLineCount() {
	return TemplateLineCount;
}

string CardGameDrawer::FormatCard(Suit CardSuit, int CardValue) {
	// Returns a drawn card formatted to represent the given card suit and value

	int LookupCardValue = CardValue - 1;

	string SuitSymbol			= SuitNames[CardSuit];
	string StringValueTop		= CardNames[LookupCardValue];
	string StringValueBottom	= "";

	//cout << "Value is " << CardValue << ", StringValue " << StringValueTop << "\n";

	// Minor adjustments to formatting to display 1 and 2 digit numbers correctly (basically just for the number 10)
	
	if (CardValue != 10) {
		StringValueTop		+= " ";
		StringValueBottom	= "_" + CardNames[LookupCardValue];
	}
	else {
		StringValueBottom = CardNames[LookupCardValue];
	}

	return ReplaceString(
		ReplaceString(
			ReplaceString(
				GetTemplate(),
				"Y",
				SuitSymbol
			),
			"X",
			StringValueTop
		),
		"Z",
		StringValueBottom
	);
}

string CardGameDrawer::FormatCard(Card& GivenCard) {
	// Returns a drawn card formatted to represent the given card suit and value

	return FormatCard(GivenCard.GetSuit(), GivenCard.GetValue());
}

string CardGameDrawer::FormatMultipleCards(vector<Card> CardsToFormat) {
	
	// We will go through all cards and format them as normal, then split by newline character to get each line of the card
	// These will then be appended together to render the cards side by side

	/*
	Old attempt, did not work (caused assertion error for some reason so I rewrote the logic afterwards)
	vector<string> LinesOfCards = {}; // each line to draw will be stored in here

	int AmountCards = CardsToFormat.size();
	cout << AmountCards << " amt cards";
	for (int i = 0; i < AmountCards; i++) {
		LinesOfCards.push_back("test");
	}

	cout << LinesOfCards.size() << "\n";

	// Iterate 6 times as there are 6 lines in the card template

	for (int index = 0; index < 6; index++) {
		for (Card NextCard : CardsToFormat) {
			string NextCardAsString = FormatCard(NextCard);
			vector<string> NextCardAsLines = SplitString(NextCardAsString, '\n'); // ' instead of " denotes char over string literal

			cout << NextCardAsLines.at(index) << "     ";

			LinesOfCards[index] = LinesOfCards[index].append(NextCardAsLines.at(index)).append("     ");

			
		};

		cout << "\n";
	}
	*/

	// Store all split cards so we don't have to split them repeatedly

	vector<vector<string>> StoredSplitCards = {};

	// Go through all cards, split, and store

	for (Card NextCard : CardsToFormat) {
		string NextCardAsString = FormatCard(NextCard);
		vector<string> NextCardAsLines = SplitString(NextCardAsString, '\n'); 
		// ' instead of " denotes char over string literal
		StoredSplitCards.push_back(NextCardAsLines);
	};

	// Now, go through each row of each card and append them next to one another

	vector<string> CardHorizontalRows = {}; // each line to draw will be stored in here

	for (int index = 0; index < GetTemplateLineCount(); index++) { 
		// Iterate by however many lines there are in the template. 
		// Was originally just a constant 6 but if the template was to ever change 
		// for whatever reason, we may as well account for that possibility.

		//cout << index << "\n";

		string Row = "";

		for (vector<string> NextCardLines : StoredSplitCards) {
			Row.append(NextCardLines.at(index));
			Row.append(CardDisplaySpacing); // CardDisplaySpacing is from resources.h
		}

		cout << Row << "\n";

		CardHorizontalRows.push_back(Row);
	}

	return "";
}