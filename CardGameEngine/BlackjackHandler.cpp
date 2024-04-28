#include "BlackjackHandler.h"
#include <iostream>
#include "CardGameDrawer.h"
#include "Player.h"
#include "Dealer.h"

using namespace std;

BlackjackHandler::BlackjackHandler(CardGameDrawer& inDrawer) {
    drawer = inDrawer;
}

int BlackjackHandler::GetBlackjackRoundResult(int playerTotal, int dealerTotal) {
    if (playerTotal == 21 and dealerTotal == 21) {
        cout << "Push! (You and the dealer both have Blackjack)\nNo one wins\n";
        return 0;
    }
    else if (dealerTotal == 21) {
        cout << "Dealer has Blackjack!\nYou lose!\n";
        return 1; // 1 means dealer won
    }
    else if (playerTotal == 21) {
        cout << "You have Blackjack!\nYou win!\n";
        return 2; // 2 means player won
    }
    // No one got blackjack, return -1
    return -1;
}

int BlackjackHandler::BlackjackPlayerChoice() {

    cout << BlackjackUserChoiceDescription;

    string playerChoiceInput;
    getline(cin, playerChoiceInput);

    if (playerChoiceInput == "1") {
        return 0;
    }
    else if (playerChoiceInput == "2") {
        return 1;
    }
    else {
        cout << "\n" << UserChoiceInvalidMessage << "\n";
        return BlackjackPlayerChoice(); // Call again and return the value of the recursive call once the player finally enters a valid value
    }
}

int BlackjackHandler::PlayBlackjack(Deck& standardDeck, Deck& additionalDeck, Player& realPlayer, Dealer& houseDealer) {

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

        Card nextDrawnCard = BlackjackDrawCardHelper(standardDeck, additionalDeck);

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

    DisplayAllPlayersHands(realPlayer, houseDealer);

    // Give player choices if their total is not 21

    int playerTotal = realPlayer.GetHandTotal();
    int dealerTotal = houseDealer.GetHandTotal();

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

                // clear the output so its easier to read
                system("CLS");
                cout << flush; // Flushes the output stream

                cout << "Hit!\n";

                Card nextDrawnCard = BlackjackDrawCardHelper(standardDeck, additionalDeck);

                realPlayer.AddCardToHand(nextDrawnCard);

                DisplayAllPlayersHands(realPlayer, houseDealer);

                // Give player choices if their total is not 21

                playerTotal = realPlayer.GetHandTotal();
                dealerTotal = houseDealer.GetHandTotal();

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

        // clear the output so its easier to read
        system("CLS");
        cout << flush; // Flushes the output stream

        cout << "\nThe Dealer is playing...\n";

        dealerChoice = houseDealer.MakeBlackjackDecision();

        if (dealerChoice == 1) {

            // dealer chooses to draw card, add it to their hand and then display the new hands

            Card nextDrawnCard = BlackjackDrawCardHelper(standardDeck, additionalDeck);

            houseDealer.AddCardToHand(nextDrawnCard);

            playerTotal = realPlayer.GetHandTotal();
            dealerTotal = houseDealer.GetHandTotal();

            DisplayAllPlayersHands(realPlayer, houseDealer);
        }
        else {

            // dealer chose to stand

            cout << "\nThe Dealer is standing.\n";

            dealerHasStood = true;
        }

        cout << "\n: Your total: " << playerTotal << "\n";
        cout << ": Dealer total: " << dealerTotal << "\n\n";

        // get round results

        blackjackRoundResult    = GetBlackjackRoundResult(playerTotal, dealerTotal);
        winRoundResult          = GetWinRoundResult(playerTotal, dealerTotal);

        //cout << "\nBJRR: " << blackjackRoundResult << ", WRR: " << winRoundResult << "\n";

        if (not (blackjackRoundResult == -1 and winRoundResult == -1)) {
            // End game, as blackjack was met or someone went bust

            return 0;
        }
        else {
            // Also output who won if there wasn't a bust or a blackjack (whoever scored higher) (as long as the dealer has stood, otherwise the dealer plays once and then gets cut off)

            if (dealerHasStood and blackjackRoundResult == -1 and winRoundResult == -1) {
                
                if (playerTotal > dealerTotal) {
                    cout << "\nPlayer has a higher total, Player wins!\n\n";
                }
                else if (playerTotal < dealerTotal) {
                    cout << "\Dealer has a higher total, Dealer wins!\n\n";
                }
                else {
                    cout << "\nPush! Both players have the same total!\n\n";
                }

                return 0;
            }

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
    */
    return 0;
}

int BlackjackHandler::GetWinRoundResult(int playerTotal, int dealerTotal) {

    if (playerTotal > 21) {
        cout << "\nBust!\nThe house wins, as always!\n\n";
        return 0; // Player lost
    }
    else if (dealerTotal > 21) {
        cout << "\nDealer Bust!\nYou win!\n\n";
        return 1; // Player won
    }
    else {
        // No one won, return -1
        return -1;
    }
}

Card BlackjackHandler::BlackjackDrawCardHelper(Deck& standardDeck, Deck& additionalDeck) {
    // Helper function to draw from both decks evenly, 
    // by swapping which deck is in use each time

    Card drawnCard;

    if (lastDrawFromAdditional) {
        drawnCard = standardDeck.DrawCard();
    }
    else {
        drawnCard = additionalDeck.DrawCard();
    }

    lastDrawFromAdditional = not lastDrawFromAdditional;
    // Flip this each time so that we draw from both decks and not just one

    return drawnCard;
}

void BlackjackHandler::DisplayAllPlayersHands(Player realPlayer, Dealer houseDealer) {
    // Helper function to display the dealer and player's hands

    // Display dealer cards

    cout << DealerDivider << "\n";

    cout << drawer.FormatMultipleCards(houseDealer.GetPlayerHand()) << "\n";

    cout << GenericDivider << "\n";

    // Display player cards

    cout << PlayerDivider << "\n";

    cout << drawer.FormatMultipleCards(realPlayer.GetPlayerHand()) << "\n";

    cout << GenericDivider << "\n";
}
