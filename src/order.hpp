#ifndef ORDER_HPP
#define ORDER_HPP

#include "board.hpp"
#include "search.hpp"

#include <climits>

inline std::string bestMoveOrder(ll depth, ll alpha, ll beta)
{
    std::string best_move = "";
    ll mx1 = LLONG_MIN;

    const ll dx[4] = {-1, 1, 0, 0};
    const ll dy[4] = {0, 0, -1, 1};

    for (ll row = 1; row <= 7; ++row)
    {
        for (ll col = 1; col <= 7; ++col)
        {
            if (board[row][col] == 0)
            {
                continue;
            }

            std::string cur_cell = chipToString(row, col);

            // Initialized doing nothing as a move
            ll stand_score = ev(depth, alpha, beta);
            if (stand_score > mx1)
            {
                mx1 = stand_score;
                best_move = cur_cell + cur_cell;
            }
            alpha = std::max(mx1, alpha);

            // Check all the empty cells by going up, down, left, right
            for (ll k = 0; k < 4; ++k)
            {
                ll new_row = row + dx[k];
                ll new_col = col + dy[k];

                // If find a non-empty space -> break
                while (new_row >= 1 && new_row <= 7 && new_col >= 1 &&
                       new_col <= 7 && board[new_row][new_col] == 0)
                {
                    std::string move_dest =
                        cur_cell + chipToString(new_row, new_col);
                    moveChip(move_dest);

                    ll cur_score = ev(depth, alpha, beta);
                    if (cur_score > mx1)
                    {
                        mx1 = cur_score;
                        best_move = move_dest;
                    }
                    undoMoveChip(move_dest);

                    alpha = std::max(mx1, alpha);

                    // keep going in up, down, left or right pattern
                    new_row += dx[k];
                    new_col += dy[k];
                }
            }
        }
    }
    return best_move;
}

#endif
