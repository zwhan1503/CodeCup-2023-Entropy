#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "board.hpp"
#include "score.hpp"

#include <algorithm>
#include <chrono>
#include <climits>
#include <utility>
#include <vector>

inline std::chrono::steady_clock::time_point allowed_time;
inline bool exceed_time;

inline ll minimax_chaos(ll depth, ll colour, ll alpha, ll beta);
inline ll minimax_order(ll depth, ll alpha, ll beta);

/*
Calculate EV to optimize minimax
- f(colour) = optimal score that Chaos can score with colour

- EV
- = sum (P(colour) * f(colour)) = sum((amount) / (remaining count) * f(colour))
- = sum(amount * f(colour)) / remaining count
*/

// ev calls chaos -> order -> chaos -> ...
inline ll ev(ll depth, ll alpha, ll beta)
{
    ll total = 0, remaining_colours = 0;
    for (ll colour = 1; colour <= 7; ++colour)
    {
        // All colours picked
        if (cnt[colour] >= 7)
        {
            continue;
        }
        remaining_colours += (7 - cnt[colour]);
        total +=
            (7 - cnt[colour]) * minimax_chaos(depth - 1, colour, alpha, beta);
    }
    if (remaining_colours == 0)
    {
        return evalBoard();
    }
    return total / remaining_colours;
}

inline bool cmp(std::pair<std::string, ll> &a, std::pair<std::string, ll> &b)
{
    return a.second < b.second;
}

inline bool cmp_desc(std::pair<std::string, ll> &a,
                     std::pair<std::string, ll> &b)
{
    return a.second > b.second;
}

// Precompute the Base Scores of each row, col before moving
inline void precompute(ll val_r[8], ll val_c[8])
{
    for (ll row = 1; row <= 7; ++row)
    {
        val_r[row] = scoreRow(row);
    }
    for (ll col = 1; col <= 7; ++col)
    {
        val_c[col] = scoreCol(col);
    }
}

const ll dx[4] = {-1, 1, 0, 0};
const ll dy[4] = {0, 0, -1, 1};

inline ll minimax_order(ll depth, ll alpha, ll beta)
{
    if (exceed_time)
    {
        return 0;
    }
    // Exceeds 500ms, flag exceed true and return
    if (std::chrono::steady_clock::now() >= allowed_time)
    {
        exceed_time = true;
        return 0;
    }

    if (depth == 0)
    {
        return evalBoard();
    }

    // Generate all possible moves -> sort the by descending value
    std::vector<std::pair<std::string, ll>> moves;

    ll original_r[8], original_c[8];
    precompute(original_r, original_c);

    // Not moving the board
    moves.push_back({"", 0});

    for (ll row = 1; row <= 7; ++row)
    {
        for (ll col = 1; col <= 7; ++col)
        {
            if (board[row][col] == 0)
            {
                continue;
            }
            std::string cur_cell = chipToString(row, col);

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

                    /*
                     * New score_row change = ScoreRow(new_row) + ScoreRow(row)
                     *           = - original_row - original_new_row
                     * New score_col change = ScoreCol(new_col) + ScoreCol(col)
                     *           = - original_col - original_new_col
                     */
                    ll score = 0;
                    // Row compute
                    score += scoreRow(new_row) + scoreRow(row) -
                             original_r[new_row] - original_r[row];

                    // Column compute
                    score += scoreCol(new_col) + scoreCol(col) -
                             original_c[new_col] - original_c[col];

                    if (row == new_row)
                        score -= (scoreRow(row) - original_r[row]);
                    if (col == new_col)
                        score -= (scoreCol(col) - original_c[col]);

                    std::pair<std::string, ll> element =
                        std::make_pair(move_dest, score);
                    moves.push_back(element);

                    undoMoveChip(move_dest);

                    // Keep going in up, down, left or right pattern
                    new_row += dx[k];
                    new_col += dy[k];
                }
            }
        }
    }

    // Sort for depth >= 2 for optimization
    if (depth >= 2)
    {
        // sort by descending value
        std::sort(moves.begin(), moves.end(), cmp_desc);
    }

    ll mx1 = LLONG_MIN;
    for (auto &[move, score] : moves)
    {
        // If not an empty string (not move string)
        if (!move.empty())
        {
            moveChip(move);
        }
        ll cur_score = ev(depth, alpha, beta);
        if (!move.empty())
        {
            undoMoveChip(move);
        }

        mx1 = std::max(mx1, cur_score);
        alpha = std::max(alpha, mx1);
        if (alpha >= beta)
        {
            return mx1;
        }
    }
    return mx1;
}

inline ll minimax_chaos(ll depth, ll colour, ll alpha, ll beta)
{
    if (exceed_time)
    {
        return 0;
    }
    // Exceeds 500ms, flag exceed true and return
    if (std::chrono::steady_clock::now() >= allowed_time)
    {
        exceed_time = true;
        return 0;
    }

    if (depth == 0)
    {
        return evalBoard();
    }

    ll original_r[8], original_c[8];
    precompute(original_r, original_c);

    // Generate all possible moves -> sort the by ascending value
    std::vector<std::pair<std::string, ll>> moves;
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

            /*
             * New score change = ScoreRow(new_row) + ScoreRow(row)
             *           = - original_row - original_new_row
             */
            ll score = scoreRow(row) + scoreCol(col) - original_r[row] -
                       original_c[col];

            std::pair<std::string, ll> element =
                std::make_pair(cur_cell, score);
            moves.push_back(element);

            undoSetChip(cur_cell);
        }
    }

    // Sort for depth >= 2 for optimization
    if (depth >= 2)
    {
        // sort by ascending value
        std::sort(moves.begin(), moves.end(), cmp);
    }

    ll mn1 = LLONG_MAX;
    for (auto &[cell, score] : moves)
    {
        setChip(cell, colour);
        ll cur_score = minimax_order(depth - 1, alpha, beta);
        mn1 = std::min(mn1, cur_score);
        undoSetChip(cell);

        beta = std::min(beta, mn1);
        if (alpha >= beta)
        {
            return mn1;
        }
    }
    return mn1;
}

#endif
