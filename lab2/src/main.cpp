//
// Created by ivan on 5/7/15.
//

#include "main.h"
#include "game.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;

    Game game;
    game.load(argv[1]);
    printMe("loaded");
    game.printBoard();
    printMe("printed");

    game.run();
}