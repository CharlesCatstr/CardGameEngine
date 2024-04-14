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
#include "BlackjackHandler.h"
#include "BaronessHandler.h"

#include <stdlib.h> // for using system("CLS")

using namespace std;

// Func declarations

Deck standardDeck       = Deck();   // Standard deck for use in all games
Deck additionalDeck     = Deck();   // Additional deck for Blackjack

CardGameDrawer Drawer   = CardGameDrawer(); // Create drawer class for rendering
Dealer houseDealer      = Dealer();         // Create Dealer instance of Player class to play against
Player realPlayer       = Player();

BlackjackHandler Blackjack = BlackjackHandler(Drawer);
BaronessHandler Baroness = BaronessHandler(Drawer, standardDeck);

//bool LastCardDrawnFromAdditional = false;

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
        cout << "\n> 2) Solitaire (Baroness)";
        cout << "\n> 3) Quit";

        string PlayerInput;
        bool PlayerInputValid = false;

        while (not PlayerInputValid) {
            cout << "\n> ";
            getline(cin, PlayerInput);
            PlayerInputValid = (PlayerInput == "1" or PlayerInput == "2" or PlayerInput == "3");

            if (not PlayerInputValid) {
                cout << "\nIncorrect selection! Please enter a value from the options provided.\n";
            }
        }

        if (PlayerInput == "1") {

            // use system("CLS") to clear the console before the next game starts
            // I used https://mathbits.com/MathBits/CompSci/Introduction/clear.htm#:~:text=To%20clear%20the%20screen%20in,h>%20is%20needed. to figure out how to do this

            system("CLS");
            cout << flush; // Flushes the output stream

            Blackjack.PlayBlackjack(standardDeck, additionalDeck, realPlayer, houseDealer);

        }
        else if (PlayerInput == "2") {

            // See above in PlayerInput == "1"

            system("CLS");
            cout << flush; // Flushes the output stream

            Baroness.PlayBaroness(realPlayer);
        }
        else if (PlayerInput == "3") {
            IsPlaying = false;
        }

    } while (IsPlaying);

    cout << "\n\nGoodbye!";
    return 0;
}

