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