﻿#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>


using namespace std;

enum Suit : int
{
	Spades = 0,
	Clubs = 1,
	Hearts = 2,
	Diamonds = 3
};

static string SuitNames[4] = {
	"♠",
	"♣",
	"♥",
	"♦"
};

static string CardNames[13] = {
	"A",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"J",
	"Q",
	"K",
};

static string DeckResetAnnouncement = "All cards are back in play and have been reshuffled.";

static string CardDisplaySpacing	= "   ";
static string PlayerDivider			= "== [ Player Hand ] =======================";
static string DealerDivider			= "== [ Dealer Hand ] =======================";
static string GenericDivider		= "==========================================";

static string BlackjackUserChoiceDescription = R"(Would you like to:
1) Hit
2) Stand
> )";

static string UserChoiceInvalidMessage = "Please choose from the available options!\n";

static string BlackjackAsciiTitle = R"(
 ________  ___       ________  ________  ___  __          ___  ________  ________  ___  __       
|\   __  \|\  \     |\   __  \|\   ____\|\  \|\  \       |\  \|\   __  \|\   ____\|\  \|\  \     
\ \  \|\ /\ \  \    \ \  \|\  \ \  \___|\ \  \/  /|_     \ \  \ \  \|\  \ \  \___|\ \  \/  /|_   
 \ \   __  \ \  \    \ \   __  \ \  \    \ \   ___  \  __ \ \  \ \   __  \ \  \    \ \   ___  \  
  \ \  \|\  \ \  \____\ \  \ \  \ \  \____\ \  \\ \  \|\  \\_\  \ \  \ \  \ \  \____\ \  \\ \  \ 
   \ \_______\ \_______\ \__\ \__\ \_______\ \__\\ \__\ \________\ \__\ \__\ \_______\ \__\\ \__\
    \|_______|\|_______|\|__|\|__|\|_______|\|__| \|__|\|________|\|__|\|__|\|_______|\|__| \|__|
                                                                                                 )";




// The following function is lifted from stackoverflow: https://stackoverflow.com/questions/5878775/how-to-find-and-replace-string/5878802#5878802
// Answered by Czarek Tomczak
// Allows for a pattern in a string to be swapped out for another given pattern
// Will be used to replace instances of ${PlayerName} in dialog with the player's entered name

static string ReplaceString(string Format, const string& ToReplace,
	const string& ReplaceWith) {
	size_t pos = 0;
	while ((pos = Format.find(ToReplace, pos)) != string::npos) {
		Format.replace(pos, ToReplace.length(), ReplaceWith);
		pos += ReplaceWith.length();
	}
	return Format;
};


// The following function is lifted from stackoverflow: https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
// Answered by thelazydeveloper

static vector<string> SplitString(string& InString, char ToSplit) {

	std::stringstream test(InString);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, ToSplit))
	{
		seglist.push_back(segment);
	}

	return seglist;
};