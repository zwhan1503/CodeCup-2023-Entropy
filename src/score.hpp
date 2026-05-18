#ifndef SCORE_HPP
#define SCORE_HPP

#include "board.hpp"
#include "common.hpp"

inline ll scoreRow(ll row)
{
    ll total = 0;
    for (ll i = 1; i <= 7; ++i)
    {
        if (board[row][i] == 0)
        {
            continue;
        }
        for (ll j = i + 1; j <= 7; ++j)
        {
            // If the substring contains 0 -> invalid
            if (board[row][j] == 0)
            {
                break;
            }

            bool is_palindrome = 1;
            ll len = j - i + 1;

            // Check palindrome 
            for (ll k = 0; k < len / 2; ++k)
            {
                if (board[row][i + k] != board[row][j - k])
                {
                    is_palindrome = 0;
                    break;
                }
            }
            if (is_palindrome)
            {
                total += len;
            }
        }
    }
    return total;
}

inline ll scoreCol(ll col)
{
    ll total = 0;
    for (ll i = 1; i <= 7; ++i)
    {
        if (board[i][col] == 0)
        {
            continue;
        }
        for (ll j = i + 1; j <= 7; ++j)
        {
            // If the substring contains 0 -> invalid
            if (board[j][col] == 0)
            {
                break;
            }

            bool is_palindrome = 1;
            ll len = (j - i + 1);

            // Check palindrome
            for (ll k = 0; k < len / 2; ++k)
            {
                if (board[i + k][col] != board[j - k][col])
                {
                    is_palindrome = 0;
                    break;
                }
            }
            if (is_palindrome)
            {
                total += len;
            }
        }
    }
    return total;
}

inline ll scoreCrossPattern(ll row, ll col)
{
    return scoreRow(row) + scoreCol(col);
}

inline ll evalBoard()
{
    ll total = 0;
    for (ll row = 1; row <= 7; ++row)
    {
        total += scoreRow(row);
    }
    for (ll col = 1; col <= 7; ++col)
    {
        total += scoreCol(col);
    }
    return total;
}

#endif
