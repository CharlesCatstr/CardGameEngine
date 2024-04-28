#include "CardGameDrawer.h";
#include "Resources.h";
#include "Card.h";
#include <string>;
#include <iostream>;

CardGameDrawer::CardGameDrawer() {
	vector<string> SplitTemplateAsLines = SplitString(Template, '\n');
	TemplateLineCount = SplitTemplateAsLines.size();
};

string CardGameDrawer::GetTemplate() {
	return Template;
}

string CardGameDrawer::GetStackTemplate() {
	return StackTemplate;
}

string CardGameDrawer::GetEmptyTemplate() {
	return EmptyTemplate;
}

string CardGameDrawer::GetDeckCounterTemplate() {
	return DeckCounterTemplate;
}

int CardGameDrawer::GetTemplateLineCount() {
	return TemplateLineCount;
}

string CardGameDrawer::GetCardWidthAsWhitespace() {
	return CardWidthAsWhitespace;
}

string CardGameDrawer::GetBaronessRowHeadingTemplate() {
	return BaronessRowHeadingTemplate;
}

string CardGameDrawer::FormatCard(Suit cardSuit, int cardValue) {
	return FormatCard(cardSuit, cardValue, false);
}

string CardGameDrawer::FormatCard(Card& GivenCard, bool isStacked) {
	// Returns a drawn card formatted to represent the given card suit and value

	return FormatCard(GivenCard.GetSuit(), GivenCard.GetValue(), isStacked);
}

string CardGameDrawer::FormatCard(Suit cardSuit, int cardValue, bool isStacked) {
	// Returns a drawn card formatted to represent the given card suit and value

	int LookupCardValue = cardValue - 1;

	string SuitSymbol			= SuitNames[cardSuit];
	string StringValueTop		= CardNames[LookupCardValue];
	string StringValueBottom	= "";

	string StackedSymbol;
	if (isStacked) {
		StackedSymbol = "|";
	}
	else {
		StackedSymbol = " ";
	}

	//cout << "Value is " << CardValue << ", StringValue " << StringValueTop << "\n";

	// Minor adjustments to formatting to display 1 and 2 digit numbers correctly (basically just for the number 10)
	
	if (cardValue != 10) {
		StringValueTop		+= " ";
		StringValueBottom	= "_" + CardNames[LookupCardValue];
	}
	else {
		StringValueBottom = CardNames[LookupCardValue];
	}

	string stackedTopFormatString = "__";

	if (isStacked) {
		stackedTopFormatString = "B";
	}

	return 
		ReplaceString(
			ReplaceString(
				ReplaceString(
					ReplaceString(
						ReplaceString(
							GetTemplate(),
							"B",
							stackedTopFormatString
						),
						"A",
						StackedSymbol
					),
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

/*

string FormatPile(Pile);
string FormatMultiplePiles(vector<Pile>);

*/

string CardGameDrawer::FormatPile(Pile givenPile) {

	// Get the card from the top of the pile
	int numCardsTotal = givenPile.GetTotalCards();
	string formattedTopCard;
	Card topCard;
	// Check if the pile is empty and, if it is, use the EmptyTemplate instead
	if (givenPile.IsEmpty()) {
		formattedTopCard = GetEmptyTemplate();
	} else {
		topCard = givenPile.PeekCard();
		formattedTopCard = FormatCard(topCard, numCardsTotal > 1);
	}
	
	string formattedPile = "";

	// Iterate n times where n is the number of cards - 1 (as we don't want to include the topCard)
	// Then append StackTemplate each iteration

	string stackedSymbol;

	vector<Card> cardsInPile = givenPile.GetAllCards();

	int count = 1;
	int lastCardValue = -1;
	string stringCardValue;

	for (Card nextCard: cardsInPile) {
		// Skip the top card as its already formatted
		if (nextCard == topCard) {
			continue;
		}

		if (count == 1) {
			stackedSymbol = " ";
		} else {
			stackedSymbol = "|";
		}

		count++;

		if (lastCardValue == -1) {
			stringCardValue = "__";
		} else if (lastCardValue != 10) {
			stringCardValue = CardNames[lastCardValue - 1] + "_";
		}
		else {
			stringCardValue = CardNames[lastCardValue - 1];
		}

		string formattedStackTemplate = ReplaceString(
			ReplaceString(
				GetStackTemplate(),
				"A",
				stackedSymbol
			),
			"B",
			stringCardValue
		);

		lastCardValue = nextCard.GetValue();

		formattedPile.append(formattedStackTemplate);
	}

	// lastCardValue will now equal the second topmost card (that isnt the one shown) so format that now

	if (lastCardValue == -1) {
		stringCardValue = "__";
	}
	else if (lastCardValue != 10) {
		stringCardValue = CardNames[lastCardValue - 1] + "_";
	}
	else {
		stringCardValue = CardNames[lastCardValue - 1];
	}

	formattedTopCard = ReplaceString(
		formattedTopCard,
		"B",
		stringCardValue
	);

	// Append formatted topCard
	formattedPile.append(formattedTopCard);

	return formattedPile;
}

string CardGameDrawer::FormatBaroness(vector<Pile> pilesToFormat, int deckRemainingCards) {

	// Firstly, cout the row headings so that the player knows which pile is which, as well as additional formatting with breaks using #'s and ='s

	int currentColumnIndex = 0; // We have the retrieve more cards option be "0" so that it is further away from the other number inputs 1-n to prevent accidentally retrieving 5 more cards
	string formattedHeader = ReplaceString(
		GetBaronessRowHeadingTemplate(),
		"X",
		to_string(currentColumnIndex) // https://stackoverflow.com/questions/5590381/how-to-convert-int-to-string-in-c
	);
	// Needs an extra bit of whitespace
	formattedHeader.append("  ");

	for (Pile nextPile : pilesToFormat) {
		currentColumnIndex++;
		formattedHeader.append(ReplaceString(
			GetBaronessRowHeadingTemplate(),
			"X",
			to_string(currentColumnIndex) // https://stackoverflow.com/questions/5590381/how-to-convert-int-to-string-in-c
		));
	}

	// cout the divider based on the number of piles, the deck counter, and how many characters are required for the width of each

	cout << formattedHeader << "\n";

	for (int i = 0; i < pilesToFormat.size() + 1; i++) {
		for (int j = 0; j < GetCardWidthAsWhitespace().length() + CardDisplaySpacing.length(); j++) {
			cout << "=";
		}
	}
	// Account for the extra whitespace we added earlier (plus one extra because it looks more symmetrical this way)
	cout << "===\n";

	// Essentially identical to the FormatMultipleCards method, but with checks for non-uniform formatted pile height

	vector<vector<string>> storedSplitPiles = {};

	// Go through all piles, split, and store

	// Keep track of the highest number of rows from a formatted pile
	int highestRowCount;


	// First, format the deck counter and separator

	string deckCountAsString = " ";

	// Add whitespace beforehand if single digit, so that the formatting is consistent and centered

	if (deckRemainingCards < 10) {
		deckCountAsString.append(to_string(deckRemainingCards));
	}
	else {
		deckCountAsString = to_string(deckRemainingCards);
	}

	string formattedDeckCounter = ReplaceString(
		GetDeckCounterTemplate(),
		"X",
		deckCountAsString
	);

	vector<string> deckCountAsLines = SplitString(formattedDeckCounter, '\n');
	// ' instead of " denotes char over string literal
	storedSplitPiles.push_back(deckCountAsLines);

	// Set highestRowCount to the number of rows in the deckCounter for now
	highestRowCount = deckCountAsLines.size();

	for (Pile nextPile : pilesToFormat) {
		string nextPileAsString = FormatPile(nextPile);
		vector<string> nextPileAsLines = SplitString(nextPileAsString, '\n');
		// ' instead of " denotes char over string literal
		storedSplitPiles.push_back(nextPileAsLines);

		int rowCount = nextPileAsLines.size();
		if (rowCount > highestRowCount) {
			highestRowCount = rowCount;
		}
	};

	// Now, go through each row of each pile and append them next to one another

	vector<string> pileHorizontalRows = {}; // each line to draw will be stored in here

	for (int index = 0; index < highestRowCount; index++) {
		// Iterate by however many lines there are in the template. 
		// Was originally just a constant 6 but if the template was to ever change 
		// for whatever reason, we may as well account for that possibility.

		//cout << index << "\n";

		string row = "";

		for (vector<string> nextPileLines : storedSplitPiles) {
			if (nextPileLines.size()-1 < index) {
				row.append(GetCardWidthAsWhitespace());
			} else {
				row.append(nextPileLines.at(index));
			}
			row.append(CardDisplaySpacing); // CardDisplaySpacing is from resources.h
		}

		cout << row << "\n";

		pileHorizontalRows.push_back(row);
	}

	return "";
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