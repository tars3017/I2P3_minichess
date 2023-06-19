#include <cstdlib>

#include "../state/state.hpp"
#include "./mcts.hpp"
#include <iostream>
#include <math.h>


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

// Random Function
static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

static uint64_t s[2] = {1234, 4321};

uint32_t RandomBigInt() {
  uint64_t s0 = s[0], s1 = s[1], result = s0 + s1;
  s1 ^= s0;
  s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
  s[1] = rotl(s1, 37);
  return (uint32_t)(result % INF);
}


Move MCTS::get_move(Node *now_node, int search_times){
//   Node* now_node = new Node(state);
  while (search_times > 0) {
    search_times--;
    monte_carlo_tree_search(now_node, now_node->state->player);
    std::cout << "=============\n";
    std::cout << "root " << now_node->wins << ' ' << now_node->times << std::endl;
    for (auto child : now_node->childs) {
    std::cout << "child " << child->wins << ' ' << child->times << std::endl;
    }
    std::cout << "=============\n";
  }

  double win_rate = -1;
  Move best_move;
  std::cout << "seek biggest ucb child\n";
  for (int i = 0; i < (int)now_node->childs.size(); ++i) {
    std::cout << "wins/times " << now_node->childs[i]->wins << ' ' << now_node->childs[i]->times << '\n';
    if (now_node->childs[i]->times == 0) {
      continue;
      // if (0 > win_rate) {
      //     win_rate = 0;
      //     best_move = node->edge_to_childs[i];
      //     continue;
      // }
      } 
      double now_rate = now_node->childs[i]->wins / now_node->childs[i]->times;
      if (now_rate > win_rate) {
        win_rate = now_rate;
        best_move = now_node->edge_to_childs[i];
      } 
    }

  return best_move; 
}


void monte_carlo_tree_search(Node* node, int game_player) {
    Node* selected = sel_node(node, game_player);
    rollout(selected, game_player);
    backpropagate(selected);
}

Node* sel_node(Node* node, int game_player) {
    std::cout << "===" << std::endl;
    if (node->childs.empty()) {
        // root
        return expand(node);
    }
    else {
        while (!node->childs.empty()) {
            Node* best_UCB_child = nullptr;
            double UCB_val = -INF;
            for (auto child : node->childs) {
                double tmp = cal_UCB(child, game_player);
                std::cout << "child UCD " << tmp << std::endl;
                if (tmp > UCB_val) {
                    UCB_val = tmp;
                    best_UCB_child = child;
                }
            }
            node = best_UCB_child;
        }
    }
    
    std::cout << "===" << std::endl;
    if (node->times == 0) {
        return node;
    }
    else {
        return expand(node);
    }

}

double cal_UCB(Node* node, int game_player) {
    if (node->times == 0 || node->parent->times == 0) {
        return INF;
    }
    if (node->parent != nullptr) {
        double UCB;
        if (node->state->player == game_player) {
            UCB = (node->wins / node->times) + 1.41421 * sqrt(log(node->parent->times) / node->times);
        }
        else {
            double oppo_wins = node->times - node->wins;
            UCB = (oppo_wins / node->times) + 1.41421 * sqrt(log(node->parent->times) / node->times);
        }
        return UCB;
    }
    
}

Node* expand(Node* node) {
    node->state->get_legal_actions();
    auto actions = node->state->legal_actions;
    Node* ret_child = nullptr;
    for (auto i : actions) {
        Node* new_child = new Node(node->state->next_state(i));
        node->childs.push_back(new_child);
        node->edge_to_childs.push_back(i);
        new_child->parent = node;
        if (ret_child == nullptr) ret_child = new_child;
    }
    return (ret_child == nullptr ? node : ret_child);
}

void rollout(Node* node, int game_player) {
    // if (node->state->game_state == WIN) {
    //     if (node->state->player == game_player) {
    //         node->wins++;
    //     }
    //     node->times++;
    //     return ;
    // }
    // int eval = 0;
    // for (int i = 0; i < BOARD_H; ++i) {
    //     for (int j = 0; j < BOARD_W; ++j) {
    //         eval += node->state->board.board[game_player][i][j];
    //         eval -= node->state->board.board[1-game_player][i][j];
    //     }
    // }
    // if (eval > 0) {
    //     node->wins++;
    // }
    // node->times++;
    // return ;

    // int origin_player = node->state->player;

    // node->state->get_legal_actions();

    // node->state->player = game_player;
    // int my_eval = node->state->evaluate(game_player);

    // // evaluate by opponent's aspect
    // node->state->player = 1 - game_player;
    // int oppo_eval = node->state->evaluate(game_player);
    // node->state->player = origin_player;

    // std::cout << "evaluate " << my_eval << ' ' << oppo_eval << std::endl;
    // if (abs(my_eval) > abs(oppo_eval)) {
    //     node->wins++;
    // }
    // node->times++;


    int simu_depth = 50;
    State* now_state = node->state;
    while (now_state->game_state != WIN && simu_depth > 0) {
        --simu_depth;
        now_state->get_legal_actions();
        auto actions = now_state->legal_actions;
        if (actions.size() == 0) {
            std::cout << "White\n";
            for (int i = 0; i < BOARD_H; ++i) {
                for (int j = 0; j < BOARD_W; ++j) {
                    std::cout << (int)now_state->board.board[0][i][j] << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << "Black\n";
            for (int i = 0; i < BOARD_H; ++i) {
                for (int j = 0; j < BOARD_W; ++j) {
                    std::cout << (int)now_state->board.board[1][i][j] << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << now_state->game_state << std::endl;
            
            break;
        }
        
        Move next_move = actions[(RandomBigInt()+simu_depth) % actions.size()];
        now_state = now_state->next_state(next_move);
    }
    if (now_state->game_state == WIN) {
        std::cout << "simulation some win\n" << std::endl;
        if (now_state->player == game_player) {
            node->wins++;
            std::cout << "white win\n";
        }
        node->times++;
    }
    else {
        int win_or_lost = (RandomBigInt()) % 2;
        if (win_or_lost) node->wins++;
        node->times++;
    }
}
void backpropagate(Node* selected) {
    while (selected->parent != nullptr) {
        selected = selected->parent;
        int wins_total = 0;
        int times_total = 0;
        for (auto child : selected->childs) {
            wins_total += child->wins;
            times_total += child->times;
            std::cout << "child has " << child->wins << ' ' << child->times << '\n';
        }
        selected->wins = wins_total;
        selected->times = times_total;
        std::cout << "backpropagate " << selected->wins << ' ' << selected->times << '\n';
    }
}