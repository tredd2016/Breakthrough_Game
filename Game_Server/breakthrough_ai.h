#ifndef BREAKTHROUGH_AI_H
#define BREAKTHROUGH_AI_H

#include "tree.hh"
#include<iostream>
#include<vector>
#include<cstdlib> //rand()
#include<climits> // INT MAX/MIN

struct Move{
    int start_row; 
    int start_col;

    int target_row;
    int target_col;

    Move(int s_row, int s_col, int t_row, int t_col);
    Move();
};


class Game_Node{
    int hypo_board[8][8];
    int node_score;
    int player_num;
    Move move_made;      

  public:  
    Game_Node();
    Game_Node(int b[8][8]);
    Game_Node(int b[8][8], int p);
    Game_Node(Game_Node &g, Move m);
    void print_board();
    bool is_game_over();
    std::vector<Move> get_all_moves();
    std::pair<int,int> translate_move(Move m);
    Move pick_AI_move(std::vector<Move> moves);
    void grade_board();

    void build_tree(tree<Game_Node> &tr, int depth);
    void calc_scores(tree<Game_Node> &tr);
    Move find_winning_move(const tree<Game_Node> &tr);
    
    void new_minimax(tree<Game_Node> &tr, tree<Game_Node>::iterator parent, int8_t depth);

    void test_print(const tree<Game_Node>& tr);
};

#endif
