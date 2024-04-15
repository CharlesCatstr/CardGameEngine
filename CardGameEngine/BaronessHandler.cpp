#include "BaronessHandler.h"

#include <iostream>
#include "Deck.h"
#include "Player.h"
#include "Pile.h"
#include "CardGameDrawer.h"

using namespace std;

BaronessHandler::BaronessHandler(CardGameDrawer& inDrawer, Deck& standardDeck) {
	//int numPiles = 5;
	//vector<Pile> allPiles;

	// Create the piles that will be used in the game

	for (int n = 1; n <= numPiles; n++) {
		Pile nextPile = Pile();
		allPiles.push_back(nextPile);
	}

	drawer = inDrawer;
	activeDeck = standardDeck;

}

void BaronessHandler::SetGameActive(bool isActive) {
	gameIsActive = isActive;
};

bool BaronessHandler::IsGameActive() {
	return gameIsActive;
}

void BaronessHandler::AppendAllPiles() {

	int remainingCardCount = activeDeck.RemainingCards();

	//cout << "remaining count:" << remainingCardCount << "\n";

	if (remainingCardCount > 0) {
		for (Pile& nextPile : allPiles) {
			Card nextCard = activeDeck.DrawCard();
			nextPile.AppendCard(nextCard);
			remainingCardCount = activeDeck.RemainingCards();

			//cout << "Added Card\n";

			if (remainingCardCount <= 0) {
				break;
			}
		}
	}
}



void BaronessHandler::DisplayActions() {

	cout << "\n";

	cout << "Your options:\n";

	cout << "> 0: Draw 5 more cards\n";
	cout << "> 1: Remove any 13s\n";
	cout << "> 2: Quit\n\n";

}

void BaronessHandler::UserSelectThirteenCards() {
	vector<string> validUserInputChoices = { "0" };

	// setup the valid options based on the number of piles in the game, with 0 as a predefined value for cancelling
	int count = 1;
	for (Pile nextPile : allPiles) {
		validUserInputChoices.push_back(to_string(count));
		count++;
	}

	cout << "Please select the column(s) that equal 13 that you want to discard\nEnter 0 to cancel\n";

	int firstColumnOrCancel = GetUserAction(validUserInputChoices);

	if (firstColumnOrCancel == 0) {
		return;
	}
	else {
		//todo remove
		cout << "You selected: " << firstColumnOrCancel;

		// Check if this first column's card is a king (equals 13) on its own. If not, ask for the second column

		// Get the pile

		Pile& firstSelectedPile = allPiles[firstColumnOrCancel - 1];

		// Get the card on top of the pile

		Card firstTopCard = firstSelectedPile.PeekCard();

		cout << "1st card value: " << firstTopCard.GetValue() << "\n";

		if (firstTopCard.GetValue() == 13) {
			// Remove the card immediately as it is a king and equals 13

			cout << "get out of here nerd!\n";

			firstSelectedPile.PopCard();

			return;
		}
		else {

			// get second value, make sure its not the same column as the first (even though it is impossible to get 13 this way)

			int secondColumnOrCancel = firstColumnOrCancel;

			do {
				secondColumnOrCancel = GetUserAction(validUserInputChoices);
				if (secondColumnOrCancel == firstColumnOrCancel) {
					cout << "You cannot choose the same column twice!\n";
				}
			} while (secondColumnOrCancel == firstColumnOrCancel);


			if (secondColumnOrCancel == 0) {
				return;
			}
			else {

				Pile& secondSelectedPile = allPiles[secondColumnOrCancel - 1];

				// Get the card on top of the pile

				Card secondTopCard = secondSelectedPile.PeekCard();

				cout << "2nd card value: " << secondTopCard.GetValue() << "\n";

				if (firstTopCard.GetValue() + secondTopCard.GetValue() == 13) {
					// both are valid and both add to 13 so we can remove both cards

					firstSelectedPile.PopCard();
					secondSelectedPile.PopCard();

					return;
				}
				else {

					cout << "Invalid selection, cards do not add to 13!\n";

				}
			}
		}



		
	}

}

int BaronessHandler::GetUserAction(vector<string> validOptions) {

	cout << "> ";

	// Get user input as a string and ensure it is valid

	string userInput;
	getline(cin, userInput);

	bool isValidInput = false;

	for (string nextValidOption : validOptions) {
		if (nextValidOption == userInput) {
			isValidInput = true;
			break;
		}
	}

	if (isValidInput) {
		return stoi(userInput); // Return the user's choice as an integer
	} else {
		// Display error message and then return another call of GetUserAction to ensure that the original call always gets a valid input
		cout << "\n" << UserChoiceInvalidMessage << "\n";
		return GetUserAction(validOptions);
	}

	cout << "\n";

	return 0;
}

int BaronessHandler::PlayBaroness(Player& realPlayer) {

	// Reset Deck

	activeDeck.Reset();
	
	// Reset all Piles

	for (Pile& nextPile : allPiles) {
		nextPile.Empty();
	}

	// Start the game loop

	SetGameActive(true);

	// Show the game title

	cout << BaronessAsciiTitle; // from resources.h

	vector<string> validUserActions = { "0", "1", "2" };

	while (IsGameActive()) {

		// Draw the board in its current empty state

		cout << drawer.FormatBaroness(allPiles, activeDeck.RemainingCards()) << "\n";

		// Show the player's choices

		DisplayActions();

		// Get the user's choice of action

		int playerChoice = GetUserAction(validUserActions);

		if (playerChoice == 0) {
			// Add a new card to each pile
			AppendAllPiles();
		} else if (playerChoice == 1) {
			// Call the pile selector to let the player choose the pile(s) they want to discard (if the card(s) on the bottom add to 13)
			UserSelectThirteenCards();
		} else if (playerChoice == 2) {
			SetGameActive(false);
		}

	}

	return 0;
}