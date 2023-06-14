#include <cstdlib>

#include "../state/state.hpp"
#include "./alphabeta.hpp"
#include <iostream>


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

int alphabeta(State* node, int depth, int alpha, int beta, int player, int game_player) {
    std::cout << "in alphabeta " << node->player << std::endl;
    node->get_legal_actions();
    int eval = node->evaluate(game_player);
    if (depth == 0 || node->game_state == WIN) {
      if (depth == 0) std::cout << "depth = 0\n";
      else std::cout << "reach end " << eval << std::endl;
      return eval;
    }
    int alphabeta_val = 0;
    auto actions = node->legal_actions;

    if (player == game_player) {
        alphabeta_val = -INF;
        for (auto i : actions) {
            alphabeta_val = std::max(alphabeta_val, alphabeta(node->next_state(i), depth-1, alpha, beta, 1-player, game_player));
            alpha = std::max(alpha, alphabeta_val);
            if (alpha >= beta) break;
        }
    }
    else {
        alphabeta_val = INF;
        for (auto i : actions) {
            alphabeta_val = std::min(alphabeta_val, alphabeta(node->next_state(i), depth-1, alpha, beta, 1-player, game_player));
            beta = std::min(beta, alphabeta_val);
            if (beta <= alpha) break;
        }
    }
    return alphabeta_val;
}
Move AlphaBeta::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  int fnl_val = -INF;

  Move best_move;
  for (auto i : actions) {
    std::cout << "start alphabeta\n";
    int tmp = alphabeta(state->next_state(i), depth, -INF, INF, 1-state->player, state->player);
    std::cout << "end alphabeta\ntmp " << tmp << "\n";
    if (tmp >= fnl_val) {
      std::cout << "change best move " << tmp << std::endl;
      fnl_val = tmp;
      best_move = i; 
    }
  }
  return best_move;
}