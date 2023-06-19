#pragma once
#include "../state/state.hpp"
#include <queue>

/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */

struct Node {
    State* state;
    std::vector<Node*> childs;
    std::vector<Move> edge_to_childs;
    int minimax_val;
    bool vis;
    Node* parent;
    
    Node() :minimax_val(0), vis(0) {}
    Node(State* _state): state(_state), minimax_val(0), vis(0) {}
    
};

class Mixed{
public:
  static Move get_move(Node* root_node, int depth, int game_player);
};