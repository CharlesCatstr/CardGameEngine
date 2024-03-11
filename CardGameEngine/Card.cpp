#include "Card.h"
#include "Resources.h"
#include <iostream>;

using namespace std;

Card::Card() {};

Card::Card(Suit N_Suit, int N_Value) {
	CardSuit = N_Suit;
	CardValue = N_Value;
}

void Card::SetValue(int N_Value) {
	CardValue = N_Value;
};

void Card::SetSuit(Suit N_Suit) {
	CardSuit = N_Suit;
};

int Card::GetValue() {
	return CardValue;
};

Suit Card::GetSuit() {
	return CardSuit;
};

