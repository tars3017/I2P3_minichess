#pragma once
#include "../state/state.hpp"


/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */

struct Node{
    State* state;
    std::vector<Node*> childs;
    std::vector<Move> edge_to_childs;
    double wins, times;
    Node *parent;
    
    Node() :wins(0), times(0) {}
    Node(State* _s)
      :state(_s), wins(0), times(0) {}
};

class MCTS{
public:
  static Move get_move(Node *now_node, int depth);
  
};

double cal_UCB(Node* node, int game_player);
Node* sel_node(Node* node, int game_player);
void rollout(Node* node, int game_player);
void backpropagate(Node* node);
void monte_carlo_tree_search(Node* node, int game_player);
Node* expand(Node* node);


