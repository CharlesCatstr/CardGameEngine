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

    bool LastDrawFromAdditional;
    CardGameDrawer Drawer;

public:

    BlackjackHandler(CardGameDrawer Drawer);

    int PlayBlackjack(Deck StandardDeck, Deck AdditionalDeck, Player RealPlayer, Dealer HouseDealer);

    int GetBlackjackRoundResult(int PlayerTotal, int DealerTotal);
    int GetWinRoundResult(int PlayerTotal, int DealerTotal);
    
    Card BlackjackDrawCardHelper(Deck StandardDeck, Deck AdditionalDeck);
    void DisplayAllPlayersHands(Player RealPlayer, Dealer HouseDealer);

    int BlackjackPlayerChoice();

};

/*

int PlayBlackjack() {

    cout << "\n" << BlackjackAsciiTitle;

    cout << "\nWelcome to Blackjack!\n";

    // Start by shuffling the Decks.
    // Don't reset the decks unless there are fewer than 10 cards in total (allows for card counting)

    if (StandardDeck.RemainingCards() + AdditionalDeck.RemainingCards() < 26) {

        cout << "\n" << GenericDivider << "\n";

        cout << DeckResetAnnouncement; // State that the cards have been reset

        cout << "\n" << GenericDivider << "\n\n";

        StandardDeck.Reset();
        AdditionalDeck.Reset();
    }

    cout << "\n";

    StandardDeck.Shuffle();
    AdditionalDeck.Shuffle();

    // Clear the player's hands

    RealPlayer.EmptyPlayerHand();
    HouseDealer.EmptyPlayerHand();

    // Give out cards to player and dealer

    for (int i = 0; i < 4; i++) {

        Card NextDrawnCard = BlackjackDrawCardHelper();

        if (i < 2) {
            RealPlayer.AddCardToHand(NextDrawnCard);
        }
        else {
            HouseDealer.AddCardToHand(NextDrawnCard);
        }

    }

    bool PlayerHasStood = false;
    bool DealerHasStood = false;

    bool PlayerHasBlackjack = false;
    bool DealerHasBlackjack = false;

    DisplayAllPlayersHands();

    // Give player choices if their total is not 21

    int PlayerTotal = GetHandTotal(RealPlayer.GetPlayerHand());
    int DealerTotal = GetHandTotal(HouseDealer.GetPlayerHand());

    cout << "\n: Your total: " << PlayerTotal << "\n";
    cout << ": Dealer total: " << DealerTotal << "\n\n";

    int BlackjackRoundResult = GetBlackjackRoundResult(PlayerTotal, DealerTotal);
    int WinRoundResult = -1;

    // Continue the game so long as neither the player nor dealer got blackjack off the bat

    if (BlackjackRoundResult == -1) {
        // Keep asking the player for their choice until they choose to stand
        while (not PlayerHasStood) {
            int NextChoice = BlackjackPlayerChoice();

            if (NextChoice == 0)
            {
                // Hit
                cout << "Hit!\n";

                Card NextDrawnCard = BlackjackDrawCardHelper();

                RealPlayer.AddCardToHand(NextDrawnCard);

                DisplayAllPlayersHands();

                // Give player choices if their total is not 21

                PlayerTotal = GetHandTotal(RealPlayer.GetPlayerHand());
                DealerTotal = GetHandTotal(HouseDealer.GetPlayerHand());

                PlayerHasBlackjack = PlayerTotal == 21;
                DealerHasBlackjack = DealerTotal == 21;

                cout << "\n: Your total: " << PlayerTotal << "\n";
                cout << ": Dealer total: " << DealerTotal << "\n\n";

                BlackjackRoundResult = GetBlackjackRoundResult(PlayerTotal, DealerTotal);
                WinRoundResult = GetWinRoundResult(PlayerTotal, DealerTotal);

                //cout << "\nBJRR: " << BlackjackRoundResult << ", WRR: " << WinRoundResult << "\n";

                if (not (BlackjackRoundResult == -1 and WinRoundResult == -1)) {
                    // End game, as blackjack was met or someone went bust
                    return 0;
                }

                //cout << "\nRoundResult: " << BlackjackRoundResult << "\n";

            }
            else if (NextChoice == 1) {
                // Stand
                PlayerHasStood = true;
            }

        }


    }

    // The Dealer now plays, if the game has not yet ended.
    int DealerChoice;

    while (not DealerHasStood and BlackjackRoundResult == -1 and WinRoundResult == -1) {
        // 1 is hit, 2 is stand

        cout << "\nThe Dealer is playing...\n";

        DealerChoice = HouseDealer.MakeBlackjackDecision();

        if (DealerChoice == 1) {
            Card NextDrawnCard = BlackjackDrawCardHelper();

            HouseDealer.AddCardToHand(NextDrawnCard);

            PlayerTotal = GetHandTotal(RealPlayer.GetPlayerHand());
            DealerTotal = GetHandTotal(HouseDealer.GetPlayerHand());

            DisplayAllPlayersHands();
        }
        else {

            cout << "\nThe Dealer is standing.\n";

            DealerHasStood = true;
        }

        cout << "\n: Your total: " << PlayerTotal << "\n";
        cout << ": Dealer total: " << DealerTotal << "\n\n";

        BlackjackRoundResult = GetBlackjackRoundResult(PlayerTotal, DealerTotal);
        WinRoundResult = GetWinRoundResult(PlayerTotal, DealerTotal);

        //cout << "\nBJRR: " << BlackjackRoundResult << ", WRR: " << WinRoundResult << "\n";


        if (not (BlackjackRoundResult == -1 and WinRoundResult == -1)) {
            // End game, as blackjack was met or someone went bust

            return 0;
        }

    }



    /*
    // Dealer makes decision

    int DealerDecision = HouseDealer.MakeDecision();

    if (DealerDecision == 1) {
        Card NextDrawnCard = BlackjackDrawCardHelper();
        HouseDealer.AddCardToHand(NextDrawnCard);
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

    cout << Drawer.FormatMultipleCards(HouseDealer.GetPlayerHand()) << "\n";

    cout << GenericDivider << "\n";

    // Display player cards

    cout << PlayerDivider << "\n";

    cout << Drawer.FormatMultipleCards(RealPlayer.GetPlayerHand()) << "\n";

    cout << GenericDivider << "\n";
}

Card BlackjackDrawCardHelper() {
    // Helper function to draw from both decks evenly, 
    // by swapping which deck is in use each time

    Card DrawnCard;

    if (LastCardDrawnFromAdditional) {
        DrawnCard = StandardDeck.DrawCard();
    }
    else {
        DrawnCard = AdditionalDeck.DrawCard();
    }

    LastCardDrawnFromAdditional = not LastCardDrawnFromAdditional;
    // Flip this each time so that we draw from both decks and not just one

    return DrawnCard;
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