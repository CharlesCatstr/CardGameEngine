#include "Player.h"
#include "Card.h"
#include "Resources.h"
#include <vector>

Player::Player() {};

vector<Card> Player::GetPlayerHand() {
	return PlayerHand;
}

int Player::GetHandTotal() {
    int AceCount    = 0;
    int Total       = 0;

    for (Card card : PlayerHand) {

        int CardValue = card.GetValue();
        if (CardValue == 1) {
            // Ace logic
            AceCount++;
        }
        else {
            // Jack, king and queen worth 10
            if (CardValue >= 11) {
                Total += 10;
            }
            else {
                Total += CardValue;
            }
        }
    }

    // Add on Aces

    for (int n = 0; n < AceCount; n++) {
        if (Total + 11 > 21) {
            Total += 1;
        }
        else {
            Total += 11;
        }
    }

    return Total;
}

void Player::EmptyPlayerHand() {
	PlayerHand = {};
}

void Player::AddCardToHand(Card& NewCard) {
	PlayerHand.push_back(NewCard);
}