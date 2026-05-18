#include "board.hpp"
#include "chaos.hpp"
#include "common.hpp"
#include "order.hpp"
#include "score.hpp"
#include "search.hpp"

std::string role = "Order";
const ll DEPTH = 3;
const ll MAX_MS = 500;

const ll alpha = LLONG_MIN; // alpha is the best possible move for order in a
                            // depth, MAX score
const ll beta =
    LLONG_MAX; // beta is the best possible move for chaos in a depth, MIN score

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    while (true)
    {
        std::string input;

        // EOF safety
        if (!getline(std::cin, input))
        {
            break;
        }

        // Set the roles of the player
        if (input == "Start")
        {
            role = "Chaos";
            continue;
        }
        if (input == "Quit")
        {
            break;
        }

        if (role == "Chaos")
        {
            // Not the start turn
            if (sz(input) == 4)
            {
                moveChip(input);
                continue;
            }
            ll colour = std::stoi(input);

            // If the code runs over 500ms, exit
            allowed_time = std::chrono::steady_clock::now() +
                       std::chrono::milliseconds(MAX_MS);
            std::string chosen_cell;
            for (ll depth = 1; depth <= 5; ++depth)
            {
                exceed_time = false;
                std::string cell = bestMoveChaos(depth, colour, alpha, beta);
                if (exceed_time)
                {
                    break;
                }
                chosen_cell = cell;
            }

            setChip(chosen_cell, colour);
            std::cout << chosen_cell << "\n";
            std::cout.flush();
        }
        if (role == "Order")
        {
            std::string cell = input.substr(1, 2);
            ll colour = input[0] - '0';
            setChip(cell, colour);

            // If the code runs over 500ms, exit
            allowed_time = std::chrono::steady_clock::now() +
                       std::chrono::milliseconds(MAX_MS);
            std::string chosen_move;
            for (ll depth = 1; depth <= 5; ++depth)
            {
                exceed_time = false;
                std::string move = bestMoveOrder(depth, alpha, beta);
                if (exceed_time)
                {
                    break;
                }
                chosen_move = move;
            }

            moveChip(chosen_move);
            std::cout << chosen_move << "\n";
            std::cout.flush();
        }
    }

    return 0;
}
