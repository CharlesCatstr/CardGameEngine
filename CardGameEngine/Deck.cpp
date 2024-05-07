#pragma once

#include <iostream>;
#include "Deck.h"

#include <String>;
#include <vector>;
#include <map>;
#include <algorithm>;
#include <random>;
#include "Resources.h";
#include "Card.h";

using namespace std;

Deck::Deck() {
	// Create all cards for this deck
	for (int CardValue = 1; CardValue < 14; CardValue++) {
		for (int SuitInt = 0; SuitInt < 4; SuitInt++) {
			Suit CardSuit;

			switch (SuitInt) {
			case 1:
				CardSuit = Suit::Clubs;
				break;
			case 2:
				CardSuit = Suit::Hearts;
				break;
			case 3:
				CardSuit = Suit::Spades;
				break;
			default:
				CardSuit = Suit::Diamonds;
				break;
			}

			Card NextCard = Card(CardSuit, CardValue);

			CardsInPlay.push_back(NextCard);
		}
	}
	// Shuffle immediately
	Shuffle();
}

Card Deck::GetFirstCardOfValue(int inValue, vector<Card> cardsToConsider) {
	// Returns the first card in the provided cardsToConsider vector with the given value inValue
	// We don't use the cards in play because these are unaffected by this method and we don't want to choose the same card twice

	for (Card nextCard : cardsToConsider) {
		if (nextCard.GetValue() == inValue) {
			return nextCard;
		}
	}

}

void Deck::SetDebugOrder() {
	// Orders all cards into an order that guarantees a possible win for debugging purposes (to ensure that the win conditions can be met and are handled appropriately)

	// To ensure a win, cards must be ordered so that cards next to one another add to 13 e.g. the order of 12, 1, 11, 2, 10

	int cardsChecked = 0;
	int currentRiggedIndex = 0;

	vector<Card> cardPool = CardsInPlay;
	vector<Card> newOrder;

	do {

		cardsChecked++;

		// Get the next card and set in the new order

		Card nextCard = GetFirstCardOfValue(riggedCardOrder.at(currentRiggedIndex), cardPool);

		// Add to newOrder

		newOrder.push_back(nextCard);

		// Remove this card from the card pool using the erase-remove idiom (https://en.wikipedia.org/wiki/Erase–remove_idiom)
		// Solution found on StackOverflow at https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position

		// This essentially moves all elements in the given vector that do not match the input element (in this case nextCard) to the front of the vector.
		// Then, the erase method is called on the vector, where the tail of the vector now contains all elements that match the input that need to be erased, which then get removed.

		cardPool.erase(remove(cardPool.begin(), cardPool.end(), nextCard), cardPool.end());

		// increment rigged index

		currentRiggedIndex++;
		
		// Reset currentRiggedIndex if its greater than length of riggedCardOrder

		if (currentRiggedIndex > (riggedCardOrder.size() - 1)) {
			currentRiggedIndex = 0;
		}

	} while (cardsChecked < CardsInPlay.size());

	// Override CardsInPlay with the new ordered vector

	CardsInPlay = newOrder;

}

Card Deck::DrawCard() {
	// Removes
	Card SelectedCard = CardsInPlay[0];

	RemovedCards.push_back(SelectedCard);
	// Remove from CardsInPlay
	CardsInPlay.erase(CardsInPlay.begin());

	return SelectedCard;
}

vector<Card> Deck::GetAllCards() {
	return CardsInPlay;
};

void Deck::Shuffle() {
	shuffle(
		CardsInPlay.begin(),	// where to start shuffling from
		CardsInPlay.end(),		// where the end of the array being shuffled is
		std::mt19937{std::random_device{}()} // The random function, in this case mt19937 (Mersenne Twister),
		// which is provided with a random device (uniformly distributed random integer generator)
		// Page about random_device: https://en.cppreference.com/w/cpp/numeric/random/random_device
		// Solution from https://stackoverflow.com/questions/26320612/c-random-shuffle-how-does-it-work

	);
};

void Deck::Refill() {
	// Move all cards from RemovedCards back to CardsInPlay

	for (int i = 0; i < RemovedCards.size(); i++) {
		CardsInPlay.push_back(RemovedCards[i]);
	}

	// clear RemovedCards

	RemovedCards.clear();

}

void Deck::Reset() {
	// Move all cards from RemovedCards back to CardsInPlay
	Refill();
	Shuffle();
}

int Deck::RemainingCards() {
	// Returns the number of cards that are still undrawn
	return CardsInPlay.size();
}