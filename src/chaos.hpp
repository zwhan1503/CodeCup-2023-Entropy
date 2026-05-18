#ifndef CHAOS_HPP
#define CHAOS_HPP

#include "board.hpp"
#include "search.hpp"

#include <climits>

inline std::string bestMoveChaos(ll depth, ll colour, ll alpha, ll beta)
{
    std::string best_move = "";
    ll mn1 = LLONG_MAX;
    for (ll row = 1; row <= 7; ++row)
    {
        for (ll col = 1; col <= 7; ++col)
        {
            if (board[row][col] != 0)
            {
                continue;
            }

            std::string cur_cell = chipToString(row, col);

            setChip(cur_cell, colour);
            ll score = minimax_order(depth - 1, alpha, beta);
            if (score < mn1)
            {
                mn1 = score;
                best_move = cur_cell;
            }
            undoSetChip(cur_cell);

            beta = std::min(beta, mn1);
        }
    }
    return best_move;
}

#endif
