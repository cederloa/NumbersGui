/*
#############################################################################
# COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures          #
# Project 4: Game called 2048                                               #
# File: gameboard                                                           #
# Description: A class with datastructure and functions for the gameboard.  #
#   The gameboard consists of items from class "NumberTile".                #
#############################################################################
*/

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "numbertile.hh"
#include <vector>
#include <random>

const int PRINT_WIDTH = 5;
const int NEW_VALUE = 2;

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    ~GameBoard();

    // Initializes the gameboard with nullptrs.
    void init_empty();

    // Initializes the random number generator and fills the gameboard
    // with random numbers.
    void fill(int seed);

    // Draws a new location (coordinates) from the random number generator and
    // puts the NEW_VALUE on that location, unless check_if_empty is true and
    // the gameboard is full.
    void new_value(bool check_if_empty = true);

    // Returns true, if all the tiles in the game board are occupied,
    // otherwise returns false.
    bool is_full() const;

    // Prints the game board.
    void print() const;

    // Moves the number tiles in the gameboard, if possible (by calling
    // move method for each number tile).
    // Finally, resets turn of all number tiles.
    bool move(Coords dir, int goal);

    // Returns the element (number tile) in the given coordinates.
    NumberTile* get_item(Coords coords);

    // Set the size of one side of the board.
    void set_size(bool size_4, bool size_6, bool size_8);

    // Access size from outside the class
    int get_size();

private:
    // Internal structure of the game board
    std::vector<std::vector<NumberTile*>> board_;

    int goal_ = 2048;
    int size_ = 4;

    // Random number generator and distribution,
    // they work better, if they are attributes of a class.
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;
};

#endif // GAMEBOARD_HH
