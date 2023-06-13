#include <cstdlib>

#include "../state/state.hpp"
#include "./minimax.hpp"
#include <iostream>


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

int minimax(State* node, int depth, int player, int game_player) {
    node->get_legal_actions();
    int eval = node->evaluate(game_player);
    if (depth == 0 || node->game_state != NONE) {
      std::cout << "reach end " << eval << std::endl;
      return eval;
    }
    int minimax_val = 0;
    
    auto actions = node->legal_actions;

    if (player == game_player) {
      minimax_val = -INF;
      for (auto i : actions) {
        minimax_val = std::max(minimax_val, minimax(node->next_state(i), depth-1, 1 - player, game_player));
      }
    }
    else {
      minimax_val = INF;
      for (auto i : actions) {
        minimax_val = std::min(minimax_val, minimax(node->next_state(i), depth-1, 1 - player, game_player));
      }
    }
    return minimax_val;
}
Move Minimax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  int fnl_val = -INF;

  Move best_move;
  for (auto i : actions) {
    int tmp = minimax(state->next_state(i), depth, state->player, state->player);
    std::cout << "tmp " << tmp << "\n";
    if (tmp >= fnl_val) {
      std::cout << "change best move " << tmp << std::endl;
      fnl_val = tmp;
      best_move = i; 
    }
  }
  return best_move;
}