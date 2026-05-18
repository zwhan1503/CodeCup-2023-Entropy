#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <string>

#include "common.hpp"

// Initilize the empty board
inline ll board[8][8];
inline ll cnt[8]; // counting the occurance of each chip colour

inline void printBoard()
{
    for (ll i = 1; i <= 7; ++i)
    {
        for (ll j = 1; j <= 7; ++j)
        {
            // Print for readability
            if (board[i][j] == 0)
            {
                std::cout << "0" << " ";
                continue;
            }
            std::cout << board[i][j] << " ";
        }
        std::cout << '\n';
    }
}

inline std::string chipToString(ll row, ll col)
{
    std::string s = "";
    s += char(row - 1 + 'A');
    s += char(col - 1 + 'a');
    return s;
}

inline void setChip(std::string s, ll colour)
{
    ll row, col;
    row = s[0] - 'A' + 1;
    col = s[1] - 'a' + 1;
    board[row][col] = colour;
    cnt[colour]++;
}

// Method for undoing
inline void undoSetChip(std::string s)
{
    ll row, col;
    row = s[0] - 'A' + 1;
    col = s[1] - 'a' + 1;
    cnt[board[row][col]]--;
    board[row][col] = 0;
}

inline void moveChip(std::string s)
{
    ll cur_row, cur_col, new_row, new_col;
    cur_row = s[0] - 'A' + 1;
    new_row = s[2] - 'A' + 1;

    cur_col = s[1] - 'a' + 1;
    new_col = s[3] - 'a' + 1;

    // If doesn't move, return, else it will set itself to 0
    if (cur_row == new_row && cur_col == new_col)
    {
        return;
    }

    board[new_row][new_col] = board[cur_row][cur_col];
    board[cur_row][cur_col] = 0;
}

// Method for undoing moving
inline void undoMoveChip(std::string s)
{
    ll cur_row, cur_col, new_row, new_col;
    cur_row = s[0] - 'A' + 1;
    new_row = s[2] - 'A' + 1;

    cur_col = s[1] - 'a' + 1;
    new_col = s[3] - 'a' + 1;

    // If doesn't move, return, else it will set itself to 0
    if (cur_row == new_row && cur_col == new_col)
    {
        return;
    }

    board[cur_row][cur_col] = board[new_row][new_col];
    board[new_row][new_col] = 0;
}

#endif
