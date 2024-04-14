#pragma once

#include "Card.h"
#include <iostream>
#include <string>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "CardGameDrawer.h"

class BlackjackHandler
{
private:

    bool lastDrawFromAdditional;
    CardGameDrawer drawer;

public:

    BlackjackHandler();
    BlackjackHandler(CardGameDrawer& drawer);

    int PlayBlackjack(Deck& standardDeck, Deck& additionalDeck, Player& realPlayer, Dealer& houseDealer);

    int GetBlackjackRoundResult(int playerTotal, int dealerTotal);
    int GetWinRoundResult(int playerTotal, int dealerTotal);
    
    Card BlackjackDrawCardHelper(Deck& standardDeck, Deck& additionalDeck); // Pass by reference so that the card actually gets removed from the deck instead of a copy of the deck
    void DisplayAllPlayersHands(Player realPlayer, Dealer houseDealer);

    int BlackjackPlayerChoice();

};

/*

Old code from before the blackjack game was its own class



int PlayBlackjack() {

    cout << "\n" << BlackjackAsciiTitle;

    cout << "\nWelcome to Blackjack!\n";

    // Start by shuffling the Decks.
    // Don't reset the decks unless there are fewer than 10 cards in total (allows for card counting)

    if (standardDeck.RemainingCards() + additionalDeck.RemainingCards() < 26) {

        cout << "\n" << GenericDivider << "\n";

        cout << DeckResetAnnouncement; // State that the cards have been reset

        cout << "\n" << GenericDivider << "\n\n";

        standardDeck.Reset();
        additionalDeck.Reset();
    }

    cout << "\n";

    standardDeck.Shuffle();
    additionalDeck.Shuffle();

    // Clear the player's hands

    realPlayer.EmptyPlayerHand();
    houseDealer.EmptyPlayerHand();

    // Give out cards to player and dealer

    for (int i = 0; i < 4; i++) {

        Card nextDrawnCard = BlackjackDrawCardHelper();

        if (i < 2) {
            realPlayer.AddCardToHand(nextDrawnCard);
        }
        else {
            houseDealer.AddCardToHand(nextDrawnCard);
        }

    }

    bool playerHasStood = false;
    bool dealerHasStood = false;

    bool playerHasBlackjack = false;
    bool dealerHasBlackjack = false;

    DisplayAllPlayersHands();

    // Give player choices if their total is not 21

    int playerTotal = GetHandTotal(realPlayer.GetPlayerHand());
    int dealerTotal = GetHandTotal(houseDealer.GetPlayerHand());

    cout << "\n: Your total: " << playerTotal << "\n";
    cout << ": Dealer total: " << dealerTotal << "\n\n";

    int blackjackRoundResult = GetBlackjackRoundResult(playerTotal, dealerTotal);
    int winRoundResult = -1;

    // Continue the game so long as neither the player nor dealer got blackjack off the bat

    if (blackjackRoundResult == -1) {
        // Keep asking the player for their choice until they choose to stand
        while (not playerHasStood) {
            int nextChoice = BlackjackPlayerChoice();

            if (nextChoice == 0)
            {
                // Hit
                cout << "Hit!\n";

                Card nextDrawnCard = BlackjackDrawCardHelper();

                realPlayer.AddCardToHand(nextDrawnCard);

                DisplayAllPlayersHands();

                // Give player choices if their total is not 21

                playerTotal = GetHandTotal(realPlayer.GetPlayerHand());
                dealerTotal = GetHandTotal(houseDealer.GetPlayerHand());

                playerHasBlackjack = playerTotal == 21;
                dealerHasBlackjack = dealerTotal == 21;

                cout << "\n: Your total: " << playerTotal << "\n";
                cout << ": Dealer total: " << dealerTotal << "\n\n";

                blackjackRoundResult = GetBlackjackRoundResult(playerTotal, dealerTotal);
                winRoundResult = GetWinRoundResult(playerTotal, dealerTotal);

                //cout << "\nBJRR: " << blackjackRoundResult << ", WRR: " << winRoundResult << "\n";

                if (not (blackjackRoundResult == -1 and winRoundResult == -1)) {
                    // End game, as blackjack was met or someone went bust
                    return 0;
                }

                //cout << "\nRoundResult: " << blackjackRoundResult << "\n";

            }
            else if (nextChoice == 1) {
                // Stand
                playerHasStood = true;
            }

        }


    }

    // The Dealer now plays, if the game has not yet ended.
    int dealerChoice;

    while (not dealerHasStood and blackjackRoundResult == -1 and winRoundResult == -1) {
        // 1 is hit, 2 is stand

        cout << "\nThe Dealer is playing...\n";

        dealerChoice = houseDealer.MakeBlackjackDecision();

        if (dealerChoice == 1) {
            Card nextDrawnCard = BlackjackDrawCardHelper();

            houseDealer.AddCardToHand(nextDrawnCard);

            playerTotal = GetHandTotal(realPlayer.GetPlayerHand());
            dealerTotal = GetHandTotal(houseDealer.GetPlayerHand());

            DisplayAllPlayersHands();
        }
        else {

            cout << "\nThe Dealer is standing.\n";

            dealerHasStood = true;
        }

        cout << "\n: Your total: " << playerTotal << "\n";
        cout << ": Dealer total: " << dealerTotal << "\n\n";

        blackjackRoundResult = GetBlackjackRoundResult(playerTotal, dealerTotal);
        winRoundResult = GetWinRoundResult(playerTotal, dealerTotal);

        //cout << "\nBJRR: " << blackjackRoundResult << ", WRR: " << winRoundResult << "\n";


        if (not (blackjackRoundResult == -1 and winRoundResult == -1)) {
            // End game, as blackjack was met or someone went bust

            return 0;
        }

    }



    /*
    // Dealer makes decision

    int DealerDecision = houseDealer.MakeDecision();

    if (DealerDecision == 1) {
        Card nextDrawnCard = BlackjackDrawCardHelper();
        houseDealer.AddCardToHand(nextDrawnCard);
        cout << "Dealer has hit and drawn another card";
    }

    DisplayAllPlayersHands();
    * /
    return 0;
}

void DisplayAllPlayersHands() {
    // Helper function to display the dealer and player's hands

    // Display dealer cards

    cout << DealerDivider << "\n";

    cout << Drawer.FormatMultipleCards(houseDealer.GetPlayerHand()) << "\n";

    cout << GenericDivider << "\n";

    // Display player cards

    cout << PlayerDivider << "\n";

    cout << Drawer.FormatMultipleCards(realPlayer.GetPlayerHand()) << "\n";

    cout << GenericDivider << "\n";
}

Card BlackjackDrawCardHelper() {
    // Helper function to draw from both decks evenly, 
    // by swapping which deck is in use each time

    Card drawnCard;

    if (LastCardDrawnFromAdditional) {
        drawnCard = standardDeck.DrawCard();
    }
    else {
        drawnCard = additionalDeck.DrawCard();
    }

    LastCardDrawnFromAdditional = not LastCardDrawnFromAdditional;
    // Flip this each time so that we draw from both decks and not just one

    return drawnCard;
}

int BlackjackPlayerChoice() {

    cout << BlackjackUserChoiceDescription;

    string PlayerChoiceInput;
    getline(cin, PlayerChoiceInput);

    if (PlayerChoiceInput == "1") {
        return 0;
    }
    else if (PlayerChoiceInput == "2") {
        return 1;
    }
    else {
        cout << "\n" << UserChoiceInvalidMessage << "\n";
        return BlackjackPlayerChoice(); // Call again and return the value of the recursive call once the player finally enters a valid value
    }
}

*/