/*
#############################################################################
# COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures          #
# Project 4: Game called 2048                                               #
# File: gameboard                                                           #
# Description: A class with datastructure and functions for the gameboard.  #
#   The gameboard consists of items from class "NumberTile".                #
#############################################################################
*/

#include "gameboard.hh"
#include <iostream>

GameBoard::GameBoard()
{
}

GameBoard::~GameBoard()
{
    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            delete board_.at(x).at(y);
            board_.at(x).at(y) = nullptr;
        }
    }
}

void GameBoard::init_empty()
{
    std::vector<NumberTile*> row;
    for( int i = 0; i < size_; ++i)
    {
        row.push_back(nullptr);
    }
    for( int i = 0; i < size_; ++i)
    {
        board_.push_back(row);
    }
}

void GameBoard::fill(int seed)
{
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, size_ - 1);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);

    if(board_.at(0).at(0) == nullptr)
    {
        for( auto y = 0; y < size_; ++y )
        {
            for( auto x = 0; x < size_; ++x )
            {
                board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x),
                                                    this);
            }
        }

    }
    else
    {
        for( auto y = 0; y < size_; ++y )
        {
            for( auto x = 0; x < size_; ++x )
            {
                board_.at(y).at(x)->set_value(0);
            }
        }
    }

    for( int i = 0 ; i < size_ ; ++i )
    {
        new_value();
    }
}

void GameBoard::new_value(bool check_if_empty)
{
    if( check_if_empty and is_full() ){
        // So that we will not be stuck in a forever loop
        return;
    }
    int random_x = 0;
    int random_y = 0;
    do
    {
        random_x = distribution_(randomEng_);
        random_y = distribution_(randomEng_);
    } while( not board_.at(random_y).at(random_x)->new_value(NEW_VALUE) );
}

void GameBoard::print() const
{
    for( auto& y : board_ )
    {
        std::cout << std::string(PRINT_WIDTH * size_ + 1, '-') << std::endl;
        for( auto x : y )
        {
            x->print(PRINT_WIDTH);
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(PRINT_WIDTH * size_ + 1, '-') << std::endl;
}

bool GameBoard::move(Coords dir, int goal)
{
    bool has_won = false;
    for( unsigned y = 0; y < board_.size(); ++y )
    {
        for( unsigned x = 0; x < board_.front().size(); ++x )
        {
            int directed_y = dir.first > 0 ? board_.size() - y - 1 : y;
            int directed_x = dir.second > 0 ? board_.back().size() - x - 1 : x;
            if( board_.at(directed_y).at(directed_x)->move(dir, goal) )
            {
                has_won = true;
            }
        }
    }
    for( auto &row : board_ )
    {
        for( auto &tile : row )
        {
            tile->reset_turn();
        }
    }
    return has_won;
}

NumberTile* GameBoard::get_item(Coords coords)
{
    return board_.at(coords.first).at(coords.second);
}

void GameBoard::set_size(bool size_4, bool size_6, bool size_8)
{
    if (size_4)
    {
        size_ = 4;
    }
    else if (size_6)
    {
        size_ = 6;
    }
    else if (size_8)
    {
        size_ = 8;
    }
}

int GameBoard::get_size()
{
    return size_;
}

bool GameBoard::is_full() const
{
    for( auto& y : board_ )
    {
        for( auto x : y )
        {
            if( x->is_empty() )
            {
                return false;
            }
        }
    }
    return true;
}
