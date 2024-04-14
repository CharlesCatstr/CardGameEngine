#include "BlackjackHandler.h"
#include <iostream>
#include "CardGameDrawer.h"
#include "Player.h"
#include "Dealer.h"

using namespace std;


BlackjackHandler::BlackjackHandler(CardGameDrawer& InDrawer) {
    Drawer = InDrawer;
}

int BlackjackHandler::GetBlackjackRoundResult(int PlayerTotal, int DealerTotal) {
    if (PlayerTotal == 21 and DealerTotal == 21) {
        cout << "Push! (You and the dealer both have Blackjack)\nNo one wins\n";
        return 0;
    }
    else if (DealerTotal == 21) {
        cout << "Dealer has Blackjack!\nYou lose!\n";
        return 1; // 1 means dealer won
    }
    else if (PlayerTotal == 21) {
        cout << "You have Blackjack!\nYou win!\n";
        return 2; // 2 means player won
    }
    // No one got blackjack, return -1
    return -1;
}

int BlackjackHandler::BlackjackPlayerChoice() {

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

int BlackjackHandler::PlayBlackjack(Deck& StandardDeck, Deck& AdditionalDeck, Player& RealPlayer, Dealer& HouseDealer) {

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

        Card NextDrawnCard = BlackjackDrawCardHelper(StandardDeck, AdditionalDeck);

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

    DisplayAllPlayersHands(RealPlayer, HouseDealer);

    // Give player choices if their total is not 21

    int PlayerTotal = RealPlayer.GetHandTotal();
    int DealerTotal = HouseDealer.GetHandTotal();

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

                Card NextDrawnCard = BlackjackDrawCardHelper(StandardDeck, AdditionalDeck);

                RealPlayer.AddCardToHand(NextDrawnCard);

                DisplayAllPlayersHands(RealPlayer, HouseDealer);

                // Give player choices if their total is not 21

                PlayerTotal = RealPlayer.GetHandTotal();
                DealerTotal = HouseDealer.GetHandTotal();

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
            Card NextDrawnCard = BlackjackDrawCardHelper(StandardDeck, AdditionalDeck);

            HouseDealer.AddCardToHand(NextDrawnCard);

            PlayerTotal = RealPlayer.GetHandTotal();
            DealerTotal = HouseDealer.GetHandTotal();

            DisplayAllPlayersHands(RealPlayer, HouseDealer);
        }
        else {

            cout << "\nThe Dealer is standing.\n";

            DealerHasStood = true;
        }

        cout << "\n: Your total: " << PlayerTotal << "\n";
        cout << ": Dealer total: " << DealerTotal << "\n\n";

        BlackjackRoundResult    = GetBlackjackRoundResult(PlayerTotal, DealerTotal);
        WinRoundResult          = GetWinRoundResult(PlayerTotal, DealerTotal);

        //cout << "\nBJRR: " << BlackjackRoundResult << ", WRR: " << WinRoundResult << "\n";

        if (not (BlackjackRoundResult == -1 and WinRoundResult == -1)) {
            // End game, as blackjack was met or someone went bust

            return 0;
        }
        else {
            // Also output who won if there wasn't a bust or a blackjack (whoever scored higher)

            if (BlackjackRoundResult == -1 and WinRoundResult == -1) {
                
                if (PlayerTotal > DealerTotal) {
                    cout << "\nPlayer has a higher total, Player wins!\n\n";
                }
                else if (PlayerTotal < DealerTotal) {
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

    int DealerDecision = HouseDealer.MakeDecision();

    if (DealerDecision == 1) {
        Card NextDrawnCard = BlackjackDrawCardHelper();
        HouseDealer.AddCardToHand(NextDrawnCard);
        cout << "Dealer has hit and drawn another card";
    }

    DisplayAllPlayersHands();
    */
    return 0;
}

int BlackjackHandler::GetWinRoundResult(int PlayerTotal, int DealerTotal) {

    if (PlayerTotal > 21) {
        cout << "\nBust!\nThe house wins, as always!\n\n";
        return 0; // Player lost
    }
    else if (DealerTotal > 21) {
        cout << "\nDealer Bust!\nYou win!\n\n";
        return 1; // Player won
    }
    else {
        // No one won, return -1
        return -1;
    }
}

Card BlackjackHandler::BlackjackDrawCardHelper(Deck& StandardDeck, Deck& AdditionalDeck) {
    // Helper function to draw from both decks evenly, 
    // by swapping which deck is in use each time

    Card DrawnCard;

    if (LastDrawFromAdditional) {
        DrawnCard = StandardDeck.DrawCard();
    }
    else {
        DrawnCard = AdditionalDeck.DrawCard();
    }

    LastDrawFromAdditional = not LastDrawFromAdditional;
    // Flip this each time so that we draw from both decks and not just one

    return DrawnCard;
}

void BlackjackHandler::DisplayAllPlayersHands(Player RealPlayer, Dealer HouseDealer) {
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
