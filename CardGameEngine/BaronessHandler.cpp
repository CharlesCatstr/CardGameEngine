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

	// Display the user's actions to them

	cout << "\n";
	cout << "Your options:\n";

	cout << "> 0: Draw 5 more cards\n";
	cout << "> 1: Remove any 13s\n";
	cout << "> 2: Quit\n\n";

}

bool BaronessHandler::DoThirteensExist() {
	// Returns true if there are any 13s in any columns (kings or cards totaling to 13)

	bool foundThirteen = false;

	for (int i = 0; i < numPiles; i++) {
		// Skip if the pile is empty
		if (allPiles[i].IsEmpty()) { continue; };

		Card topCard = allPiles[i].PeekCard();

		if (topCard.GetValue() == 13) {
			foundThirteen = true;
			break;
		}

		// try next pile, if there is a next pile and it isn't empty

		for (int j = 0; j < numPiles; j++) {
			// Skip if the pile is empty
			if (allPiles[j].IsEmpty()) { continue; };

			Card nextCard = allPiles[j].PeekCard();

			//cout << "the total of the piles " << i << " and " << j << " is " << topCard.GetValue() + nextCard.GetValue() << "\n";

			if (topCard.GetValue() + nextCard.GetValue() == 13) {
				// There is a thirteen available from the total of 2 cards, return true
				foundThirteen = true;
				break;
			}

		}

	}

	return foundThirteen;
}

void BaronessHandler::UserSelectThirteenCards() {
	vector<string> validUserInputChoices = { "0" };

	// setup the valid options based on the number of piles in the game, with 0 as a predefined value for cancelling
	// allows for us to add more piles if we wanted (though the game only needs 5)
	int count = 1;
	for (Pile nextPile : allPiles) {
		validUserInputChoices.push_back(to_string(count));
		count++;
	}
	// ask user for their choice
	cout << "Please select the column(s) that equal 13 that you want to discard\nEnter 0 to cancel\n";

	int firstColumnOrCancel = GetUserAction(validUserInputChoices);

	if (firstColumnOrCancel == 0) {
		// cancelled, do nothing
		return;
	}
	else {

		// Check if this first column's card is a king (equals 13) on its own. If not, ask for the second column

		// Get the pile

		Pile& firstSelectedPile = allPiles[firstColumnOrCancel - 1];

		// Get the card on top of the pile (so long as it is not empty)

		if (firstSelectedPile.IsEmpty()) {
			cout << "That column is empty!\n\n";
			return;
		}

		// take a look at what the first selected pile's top card is
		Card firstTopCard = firstSelectedPile.PeekCard();

		cout << "\n-> First card's value: " << firstTopCard.GetValue() << "\n\n";

		if (firstTopCard.GetValue() == 13) {
			// Remove the card immediately as it is a king and equals 13


			firstSelectedPile.PopCard();

			return;
		}
		else {

			// get second value, make sure its not the same column as the first (even though it is impossible to get 13 this way)

			int secondColumnOrCancel = firstColumnOrCancel;

			do {
				secondColumnOrCancel = GetUserAction(validUserInputChoices);
				if (secondColumnOrCancel == firstColumnOrCancel) {
					cout << "\nYou cannot choose the same column twice!\n\n";
				}
			} while (secondColumnOrCancel == firstColumnOrCancel);


			if (secondColumnOrCancel == 0) {
				return;
			}
			else {

				Pile& secondSelectedPile = allPiles[secondColumnOrCancel - 1];

				// Get the card on top of the pile (if not empty)

				if (secondSelectedPile.IsEmpty()) {
					cout << "That column is empty!\n\n";
					return;
				}
				// take a look at the top card of the second selectedpile
				Card secondTopCard = secondSelectedPile.PeekCard();

				cout << "\n-> Second card's value: " << secondTopCard.GetValue() << "\n\n";

				if (firstTopCard.GetValue() + secondTopCard.GetValue() == 13) {
					// both are valid and both add to 13 so we can remove both cards

					firstSelectedPile.PopCard();
					secondSelectedPile.PopCard();

					return;
				}
				else {

					cout << "\nInvalid selection, cards do not add to 13!\n\n";

				}
			}
		}
	}
}
	// TODO: win condition
	// TODO 1 in 3 chnce david williams easter egg (he won before you did)

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

		cout << "\n";

		cout << drawer.FormatBaroness(allPiles, activeDeck.RemainingCards()) << "\n";

		// Show the player's choices

		DisplayActions();

		// Get the user's choice of action

		cout << "Does 13 exist? " << DoThirteensExist() << "\n\n";

		bool allPilesEmpty = true;

		for (int x = 0; x < numPiles; x++) {
			if (not allPiles[x].IsEmpty()) {
				allPilesEmpty = false;
				break;
			}
		}

		if (allPilesEmpty and activeDeck.RemainingCards() <= 0) {
			// Player has won, let them know
			cout << "\nNo cards remain. You win!\n\n";
			cout << "david is the coolest (he won the baroness game before you).\n"; // thanks david williams for playing my game and forcing me to add this cout
			SetGameActive(false);
			// End this iteration early so the loop can end without showing user options
			continue;
		} else if (DoThirteensExist()) {
			cout << "\nThere is a possible play this round\n\n";
		} else if (activeDeck.RemainingCards() <= 0) {
			// The game is impossible and the player loses as there are no plays available and no cards left in the deck
			cout << "\nThere are no more possible plays left. You lose.\n\n";
			SetGameActive(false);
			// End this iteration early so the loop can end without showing user options
			continue;
		};

		int playerChoice = GetUserAction(validUserActions);

		if (playerChoice == 0) {
			// Add a new card to each pile, or show error if there are no cards left

			// clear the output so its easier to read
			system("CLS");
			cout << flush; // Flushes the output stream

			if (activeDeck.RemainingCards() > 0) {
				AppendAllPiles();
			}

			else {
				cout << "There are no cards left in the deck to draw.\n";
			}

		} else if (playerChoice == 1) {
			// Call the pile selector to let the player choose the pile(s) they want to discard (if the card(s) on the bottom add to 13)
			UserSelectThirteenCards();

			// clear the output so its easier to read
			system("CLS");
			cout << flush; // Flushes the output stream

		} else if (playerChoice == 2) {
			SetGameActive(false);
		}

	}

	return 0;
}