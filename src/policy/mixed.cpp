#include <cstdlib>

#include "../state/state.hpp"
#include "./mixed.hpp"
#include <iostream>


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
struct my_cmp {
    bool operator()(Node* lhs, Node* rhs) const {
        if (lhs->vis == 0) return 1; // l < r is false
        else if (rhs->vis == 0) return 0; // l < r is true
        else return lhs->minimax_val > rhs->minimax_val; 
    }
};


std::priority_queue<Node*, std::vector<Node*>, my_cmp> pq;

void mixed_rollout(Node* now, int game_player) {
    now->vis = 1;
    now->state->get_legal_actions();
    now->minimax_val = now->state->evaluate(game_player);

    // int simu_depth = 50;
    // State* now_state = now->state;
    // while (now_state->game_state != WIN && simu_depth > 0) {
    //     --simu_depth;
    //     now_state->get_legal_actions();
    //     auto actions = now_state->legal_actions;
    //     Move next_move = actions[(rand()*simu_depth + rand()) % actions.size()];
    //     now_state = now_state->next_state(next_move);
    // }
    // if (now_state->game_state == WIN) {
    //     std::cout << "player " << now_state->player << ' ' << game_player << std::endl;
    //     if (now_state->player == game_player) {
    //         now->minimax_val++;
    //     }
    //     else {
    //         now->minimax_val--;
    //     }
    // }
    // else {
    //     std::cout << "minus 2" << std::endl;
    //     int win_or_lost = (rand()+1) % 3;
    //     if (win_or_lost > 0) now->minimax_val++;
    //     else now->minimax_val--;
    // }
    // std::cout << "minimax " << now->minimax_val << std::endl;
}

void mixed_backpropagate(Node* now, int game_player) {
    while (now->parent != nullptr) {
        now = now->parent;
        int fnl_val;
        if (now->state->player == game_player) {
            fnl_val = -INF;
            for (auto i : now->childs) {
                fnl_val = (i->minimax_val > fnl_val ? i->minimax_val : fnl_val);
            }
        }
        else {
            fnl_val = INF;
            for (auto i : now->childs) {
                fnl_val = (i->minimax_val < fnl_val ? i->minimax_val : fnl_val);
            }
        }
        now->minimax_val = fnl_val;
    }
}
Move Mixed::get_move(Node* root_node, int depth, int game_player) {
  std::cout << "depthhhh = " << depth << std::endl;
  if (depth == 0) {
    root_node->vis = 1;
    auto actions = root_node->state->legal_actions;
    Node* first_child = nullptr;
    for (auto i : actions) {
        Node* new_child = new Node(root_node->state->next_state(i));
        new_child->parent = root_node;
        root_node->childs.push_back(new_child);
        root_node->edge_to_childs.push_back(i);
        if (first_child == nullptr) {
            first_child = new_child;
            mixed_rollout(first_child, game_player);
        }
        pq.push(new_child);
    }
    std::cout << "Child size " << root_node->childs.size() << std::endl;
  }
  else {
    std::cout << "not root" << std::endl;
    Node* leaf = pq.top();
    pq.pop();

    if (leaf->vis == 0) {
        mixed_rollout(leaf, game_player);
        mixed_backpropagate(leaf, game_player);
    }
    else {
        auto actions = leaf->state->legal_actions;
        Node* first_child = nullptr;
        for (auto i : actions) {
            Node* new_child = new Node(leaf->state->next_state(i));
            new_child->parent = leaf;
            leaf->childs.push_back(new_child);
            leaf->edge_to_childs.push_back(i);
            if (first_child == nullptr) {
                first_child = new_child;
                mixed_rollout(first_child, game_player);
            }
            pq.push(new_child);
        }
        mixed_backpropagate(first_child, game_player);
    }
  }


  Move best_move;
  int max_val = -INF;
  for (int i = 0; i < (int)root_node->childs.size(); ++i) { 
    if (root_node->childs[i]->vis && root_node->childs[i]->minimax_val > max_val) {
        max_val = root_node->childs[i]->minimax_val;
        best_move = root_node->edge_to_childs[i];
    }
  }
  return best_move;
}