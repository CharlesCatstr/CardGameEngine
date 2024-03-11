// Change console encoding to support UTF-8 (so that suit symbols can be printed)
#pragma execution_character_set( "utf-8" )

#include <iostream>
#include <Windows.h>;

#include "Resources.h";
#include "Card.h";
#include "Deck.h";
#include "CardGameDrawer.h"
#include "Player.h"
#include "Dealer.h"

using namespace std;

// Func declarations

int PlayBlackjack();
int BlackjackPlayerChoice();
Card BlackjackDrawCardHelper();
void DisplayAllPlayersHands();

Deck StandardDeck       = Deck();   // Standard deck for use in all games
Deck AdditionalDeck     = Deck();   // Additional deck for Blackjack

CardGameDrawer Drawer   = CardGameDrawer(); // Create drawer class for rendering
Dealer HouseDealer      = Dealer();         // Create Dealer instance of Player class to play against
Player RealPlayer       = Player();

bool LastCardDrawnFromAdditional = false;

/* Old and obsolete, replaced by method inside player class

int GetHandTotal(vector<Card> InHand) {
    // Keep count of how many aces there are so we can account for some aces being worth 1 and some being 11
    int AceCount    = 0;
    int Total       = 0;

    for (Card NextCard : InHand) {
        int Value = NextCard.GetValue();

        if (Value == 1) {
            AceCount++;
        }
        else {

            if (Value > 10) {
                Total += 10;

            }
            else {
                Total += Value;

            }
        }
    }
    if (AceCount > 0) {
        for (int i = 0; i < AceCount; i++) {
            if ((Total + 11) > 21) {
                Total += 1;
            }
            else {
                Total += 11;
            }
        }
    }
    return Total;
}
*/

int GetBlackjackRoundResult(int PlayerTotal, int DealerTotal) {
    if (PlayerTotal == 21 and DealerTotal == 21) {
        cout << "Push! (You and the dealer both have Blackjack)\nNo one wins";
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

int GetWinRoundResult(int PlayerTotal, int DealerTotal) {

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

int main()
{

    SetConsoleOutputCP(65001);

    // Testing that SetConsoleOutputCP(65001) and the UTF-8 pragma work to change the console character set

    /*
    
    for (string Value : SuitNames) {
        cout << Value << ",\n";
    }
    cout << "\n";
    
    */
    


    // Other debug testing stuff that isn't needed anymore
    /*
    Card NewCard5 = Card(Suit::Clubs, 11);

    Deck NewDeck = Deck();

    // Cout all cards

    /*
    for (Card NextCard : NewDeck.GetAllCards()) {
        NextCard.PrintData();
    }
    * /

    Card SelectedCard = NewDeck.DrawCard();
    cout << Drawer.FormatCard(SelectedCard);

    cout << "Shuffling!\n";

    NewDeck.Shuffle();
    /*
    for (Card NextCard : NewDeck.GetAllCards()) {
        cout << Drawer.FormatCard(NextCard);
    }
    * /
    vector<Card> AllCardsInDeck = { NewCard5, NewDeck.GetAllCards()[1], NewDeck.GetAllCards()[6] };

    cout << Divider << "\n";

    Drawer.FormatMultipleCards(AllCardsInDeck);

    cout << "\n" << Divider << "\n";

    int Total = 0;

    vector<Card> PlayerHand = {SelectedCard};

    int AceCount = 0;

    for (Card card : PlayerHand) {

        int CardValue = card.GetValue();
        if (CardValue == 1) {
            // Ace logic
            if (Total > 11) {
                Total += 1;
            }
            else {
                Total += 11;
            }
        }
        else {
            if (CardValue >= 11) {
                Total += 10;
            }
            else {
                Total += CardValue;
            }
        }
    }

    cout << "\n\n\n\n\n\n\n\n\n\n";
    */

    cout << "Welcome!!!!!!!!!\n\n";

    bool IsPlaying = true;

    do {

        cout << "What game do you want to play?\n";

        cout << GenericDivider;

        cout << "\n> 1) Blackjack";
        cout << "\n> 2) Quit";

        string PlayerInput;
        bool PlayerInputValid = false;

        while (not PlayerInputValid) {
            cout << "\n> ";
            getline(cin, PlayerInput);
            PlayerInputValid = (PlayerInput == "1" or PlayerInput == "2");

            if (not PlayerInputValid) {
                cout << "\nIncorrect selection! Please enter a value from the options provided.\n";
            }
        }

        if (PlayerInput == "1") {
            PlayBlackjack();
        }
        else if (PlayerInput == "2") {
            IsPlaying = false;
        }

    } while (IsPlaying);

    cout << "\n\nGoodbye!";
    return 0;
}

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

    int PlayerTotal = RealPlayer.GetHandTotal();
    int DealerTotal = HouseDealer.GetHandTotal();

    cout << "\n: Your total: " << PlayerTotal << "\n";
    cout << ": Dealer total: " << DealerTotal << "\n\n";

    int BlackjackRoundResult = GetBlackjackRoundResult(PlayerTotal, DealerTotal);
    int WinRoundResult       = -1;

    // Continue the game so long as neither the player nor dealer got blackjack off the bat

    if (BlackjackRoundResult == -1) {
        // Keep asking the player for their choice until they choose to stand
        while (not PlayerHasStood) {
            int NextChoice  = BlackjackPlayerChoice();

            if (NextChoice  == 0) 
            {
                // Hit
                cout << "Hit!\n";

                Card NextDrawnCard = BlackjackDrawCardHelper();

                RealPlayer.AddCardToHand(NextDrawnCard);

                DisplayAllPlayersHands();

                // Give player choices if their total is not 21

                PlayerTotal = RealPlayer.GetHandTotal();
                DealerTotal = HouseDealer.GetHandTotal();

                PlayerHasBlackjack = PlayerTotal == 21;
                DealerHasBlackjack = DealerTotal == 21;

                cout << "\n: Your total: " << PlayerTotal << "\n";
                cout << ": Dealer total: " << DealerTotal << "\n\n";

                BlackjackRoundResult = GetBlackjackRoundResult(PlayerTotal, DealerTotal);
                WinRoundResult       = GetWinRoundResult(PlayerTotal, DealerTotal);

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

        // Dealer makes their choice (basically stands (returns 2) if 17 or above)
        DealerChoice = HouseDealer.MakeBlackjackDecision();

        if (DealerChoice == 1) {
            // Draw next card and add to dealer's hand
            Card NextDrawnCard = BlackjackDrawCardHelper();

            HouseDealer.AddCardToHand(NextDrawnCard);

            PlayerTotal = RealPlayer.GetHandTotal();
            DealerTotal = HouseDealer.GetHandTotal();
            
            // Display the dealer and player cards
            DisplayAllPlayersHands();
        }
        else {

            cout << "\nThe Dealer is standing.\n";
            // make dealer stand
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
    */
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
    // Display player choices
    cout << BlackjackUserChoiceDescription;
    // Get player's choice
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