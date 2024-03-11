#pragma once
#include "Player.h"
class Dealer :
    public Player {
public:

    Dealer();
    
    int MakeBlackjackDecision();
};

