#include "Pile.h"
#include <vector>
#include "Card.h"



Pile::Pile() {};

void Pile::AppendCard(Card inCard) {
	cardsInPile.push_back(inCard);
}

Card& Pile::PopCard() {

	Card cardAtBack = cardsInPile.back();
	cardsInPile.pop_back();
	return cardAtBack;
	
}

bool Pile::IsEmpty() {
	return cardsInPile.empty();
}

Card Pile::PeekCard() {
	return cardsInPile.back();
}

vector<Card> Pile::GetAllCards() {
	return cardsInPile;
}

int Pile::GetTotalCards() {
	return cardsInPile.size();
}

void Pile::Empty() {
	cardsInPile.clear();
}