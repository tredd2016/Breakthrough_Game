#include "breakthrough_ai.h"

using namespace std;

Move::Move(){
    start_row = -1;
    start_col = -1;
    target_row = -1;
    target_col = -1;    
}

Move::Move(int s_row, int s_col, int t_row, int t_col){
    start_row = s_row;
    start_col = s_col;
    target_row = t_row;
    target_col = t_col;
}

Game_Node::Game_Node(Game_Node &g, Move m){
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            hypo_board[i][j] = g.hypo_board[i][j];
        }
    }

    hypo_board[m.start_row][m.start_col] = 0;
    hypo_board[m.target_row][m.target_col] = g.player_num;

    player_num = (g.player_num == 1) ? 2 : 1;   
    move_made = m;
}

Game_Node::Game_Node(){
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            hypo_board[i][j] = 0;
        }
    }
}

Game_Node::Game_Node(int b[8][8], int p){
    player_num = p;
    
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            hypo_board[i][j] = b[i][j];
        }
    }
}

Game_Node::Game_Node(int b[8][8]){
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            hypo_board[i][j] = b[i][j];
        }
    }
}


void Game_Node::print_board(){
    cout << "    0 1 2 3 4 5 6 7\n\n";
    for(int i=0; i<8; ++i){
        cout << i << "  |";
        for(int j=0; j<8; ++j){
            cout << hypo_board[i][j] << "|";
        }
        cout << "\n";
    }        
    cout << endl << endl;
}

bool Game_Node::is_game_over(){
    // reach home row
    for(int i=0; i<8; ++i){
        if(hypo_board[0][i] == 2 || hypo_board[7][i] == 1){
            return true;
        }
    }

    // eliminate all pieces
    int p1_count = 0;
    int p2_count = 0;
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            if(hypo_board[i][j] == 1) ++p1_count;
            if(hypo_board[i][j] == 2) ++p2_count;
        }
    }
    if(p1_count == 0 || p2_count == 0) return true;

    return false;
}

vector<Move> Game_Node::get_all_moves(){
    vector<Move> all_moves;
    int partity = (player_num == 1) ? 1 : -1;
    int opp_player_num = (player_num == 1) ? 2 : 1;
    //run through the game board looking for my pieces
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            if(hypo_board[i][j] == player_num){               
                int target_row = i + partity; // same for all moves, do once
                // Forward                
                if(target_row >= 0 && target_row < 8){
                    if(hypo_board[target_row][j] == 0){
                        all_moves.push_back(Move(i, j, target_row, j));
                    }
                }
                // Left
                int l_target_col = j + partity;
                if(target_row >= 0 && target_row < 8 && l_target_col >= 0 && l_target_col < 8){
                    if(hypo_board[target_row][l_target_col] == 0 || hypo_board[target_row][l_target_col] == opp_player_num){
                        all_moves.push_back(Move(i, j, target_row, l_target_col));
                    }
                }

                // Right
                int r_target_col = j - partity;
                if(target_row >= 0 && target_row < 8 && r_target_col >= 0 && r_target_col < 8){
                    if(hypo_board[target_row][r_target_col] == 0 || hypo_board[target_row][r_target_col] == opp_player_num){
                        all_moves.push_back(Move(i, j, target_row, r_target_col));
                    }
                }
            }
        }
    }
    return all_moves;
}


pair<int,int> Game_Node::translate_move(Move m){
    int orgin_square = ((m.start_row * 8) + m.start_col);
    int target_square = ((m.target_row * 8) + m.target_col);

    return make_pair(orgin_square, target_square);
}

Move Game_Node::pick_AI_move(vector<Move> moves){
    int N = moves.size();
    int r = rand() % N;
    //int test = grade_move(moves[r], true);
    return moves[r];
}

void Game_Node::grade_board(){
    // Player number in the Game_Node holds who's turn it currently is for that game state. 
    // Fliping those around to eval this game state from the perspective of who make the last move.    

    int eval_player_num = (player_num == 1) ? 2 : 1; 
    int opp_player_num = player_num;
    
    int my_home_row_index = (eval_player_num == 1) ? 0 : 7;
    int opp_home_row_index = (eval_player_num == 1) ? 7 : 0;
    
    int eval_counter = 0;   

    int my_home_pieces = 0;
    int opp_home_pieces = 0;

    for(int i=0; i<8; ++i){
        // winning/losing
        if((hypo_board[opp_home_row_index][i] == eval_player_num)){
            //cout << "Winner found" << endl;
            eval_counter += 99999;
        } 
        if((hypo_board[my_home_row_index][i] == opp_player_num)){
            //cout << "Loser found" << endl;
            eval_counter -= 99999;
        } 
        
        // Almost winning/losing (potentially check to see if unobstructed [bounds check])
        if(eval_player_num == 1){                        
            if(hypo_board[6][i] == eval_player_num) eval_counter += 500;
            if(hypo_board[1][i] == opp_player_num) eval_counter -= 500;            
        }
        if(eval_player_num == 2){      
            if(hypo_board[1][i] == eval_player_num) eval_counter += 500;            
            if(hypo_board[6][i] == opp_player_num) eval_counter -= 500;            
        }

        // Home row pieces
        if(hypo_board[my_home_row_index][i] == eval_player_num) my_home_pieces++;
        if(hypo_board[opp_home_row_index][i] == opp_player_num) opp_home_pieces++;
    }
    
    eval_counter += (my_home_pieces * 20);
    eval_counter -= (opp_home_pieces * 20);
    
    // vertical/horizontal pairs
    
    
    // column holes    
    for(int i=0; i<8; ++i){ 
        bool my_col_filled = false;
        bool opp_col_filled = false;
        
        for(int j=0; j<8; ++j){
            if(hypo_board[j][i] == eval_player_num){
                my_col_filled = true;
                break;
            }
        }

        for(int j=0; j<8; ++j){
            if((hypo_board[j][i]) == opp_player_num){
                opp_col_filled = true;
                break;
            }
        }
        if(!my_col_filled) eval_counter -= 20;
        if(!opp_col_filled) eval_counter += 20;
    }

    // Count pieces, the more I have the better, the less my opponent has the better.
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            if(hypo_board[i][j] == eval_player_num) eval_counter += 500;
            if(hypo_board[i][j] == opp_player_num) eval_counter -= 500;
        }
    }

    node_score = eval_counter;
    return;
}

void Game_Node::build_tree(tree<Game_Node> &tr, int depth){
    if(depth == 0) return;

    vector<tree<Game_Node>::iterator> leaf_nodes;
    for(tree<Game_Node>::leaf_iterator it = tr.begin_leaf(); it != tr.end_leaf(); ++it){
        leaf_nodes.push_back(it);
    }

    for(auto it : leaf_nodes){
        if(!it->is_game_over()){
            vector<Move> all_moves = it->get_all_moves();
            for(auto i : all_moves){
                Game_Node temp(*it, i);                
                tr.append_child(it, temp);
            }
        }
    }

    build_tree(tr, depth - 1);
}

// Post order iteration hits all leaf nodes first before working up to root
void Game_Node::calc_scores(tree<Game_Node> &tr){
    for(tree<Game_Node>::post_order_iterator it = tr.begin_post(); it != tr.end_post(); ++it){     
        // Terminal/Leaf node - run the scoring function
        if(tr.number_of_children(it) == 0){
            it->grade_board();
            if(tr.depth(it) % 2 == 0) it->node_score *= -1;
            //cout << "scoring player: " << it->player_num << "\t scoring is max: " << tr.depth(it) % 2 << endl;           
        }
        
        // Otherwise, bubble up score
        // The grade_board function evals the current game state through the perspective of the person who made the move last.
        // E.g., A good AI move will score high, and a good opponent move will also score high. 
        // Therefore, need to always take max scores at each level. Min/Max would assume every board is graded in the perspective of the AI.
        if(tr.number_of_children(it) > 0){
            int best_child_value = tr.child(it, 0)->node_score;
            for(int i = 1; i < tr.number_of_children(it); ++i){
                if(tr.depth(it) % 2 == 0) best_child_value = max(best_child_value, tr.child(it, i)->node_score);
                else best_child_value = min(best_child_value, tr.child(it, i)->node_score);
            }                        
            it->node_score = best_child_value;
        }
    }
}

// If there are multiple possible moves with the same score pick a semi-random one of them
Move Game_Node::find_winning_move(const tree<Game_Node> &tr){
    vector<Move> winners;
    int winning_score = tr.begin()->node_score; // begin() is pre-order iterator, starts with root    

    for(tree<Game_Node>::sibling_iterator it = tr.begin(tr.begin()); it != tr.end(tr.begin()); ++it){
        if(it->node_score == winning_score) winners.push_back(it->move_made);
        //cout << "children scores: " << it->node_score << endl;           
    }
    int N = winners.size();
    int r = rand() % N;
    return winners[r];
}

void Game_Node::new_minimax(tree<Game_Node> &tr, tree<Game_Node>::iterator parent, int8_t depth){
    // Generate all possible moves
    vector<Move> all_moves = parent->get_all_moves(); 

    if(depth == 1){
        //bool is_max_level = (tr.depth(parent) % 2 == 0) ? true : false;
        vector<int> move_scores;
        vector<int>::iterator itr_best_child_value;
        for(auto move : all_moves){
            Game_Node temp(*parent, move);
            temp.grade_board();
            if(tr.depth(parent) % 2 == 1) temp.node_score *= -1;
            //cout << "scoring player: " << temp.player_num << "\t scoring is max: " << tr.depth(parent) % 2 << endl;
            //tr.append_child(parent, temp);
            move_scores.push_back(temp.node_score);
        }
        if(tr.depth(parent) % 2 == 1){
            itr_best_child_value = min_element(begin(move_scores), end(move_scores));
        }
        else{
            itr_best_child_value = max_element(begin(move_scores), end(move_scores));
        }
        
        parent->node_score = *itr_best_child_value;
    }

    else{
        for(auto move : all_moves){            
            Game_Node temp(*parent, move);
            tree<Game_Node>::iterator itr = tr.append_child(parent, temp);
            new_minimax(tr, itr, depth-1);
        }

        if(tr.number_of_children(parent) > 0){
            int best_child_value = tr.child(parent, 0)->node_score;
            for(int i = 1; i < tr.number_of_children(parent); ++i){
                if(tr.depth(parent) % 2 == 0) best_child_value = max(best_child_value, tr.child(parent, i)->node_score);
                else best_child_value = min(best_child_value, tr.child(parent, i)->node_score);
            }
            parent->node_score = best_child_value;

            // generally delete children nodes once we have what we need out of them. But keep depth=1 (possible moves) to further eval them.
            if(tr.depth(parent) !=0) tr.erase_children(parent); 
        }
    }
}

// Only used for testing
void Game_Node::test_print(const tree<Game_Node>& tr){
    tree<Game_Node>::pre_order_iterator it = tr.begin();
    tree<Game_Node>::pre_order_iterator end = tr.end();
    if(!tr.is_valid(it)) return;
    int rootdepth=tr.depth(it);
    cout << "-----" << endl;
    while(it!=end) {
        for(int i=0; i<tr.depth(it)-rootdepth; ++i)
            cout << "  ";
        cout << it->node_score << endl << flush;
        ++it;
        }
    cout << "-----" << endl;
}