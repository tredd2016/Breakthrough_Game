#include "crow.h"
#include "crow/middlewares/cors.h"
#include "breakthrough_ai.h"
#include "tree.hh"

int main() {
    crow::App<crow::CORSHandler> app;
	auto& cors = app.get_middleware<crow::CORSHandler>();

	cors
        .global()
		.origin("*")
		//.prefix("/makeMove")
		.headers("origin, x-requested-with, accept, Access-Control-Allow-Origin, authorization, content-type")
		.methods("POST"_method, "GET"_method, "PUT"_method, "DELETE"_method, "PATCH"_method, "OPTIONS"_method)
		.allow_credentials();

    
    CROW_ROUTE(app, "/makeMove")
    .methods("POST"_method)
    ([](const crow::request& req){
        // Check if the request has a JSON body
        if (req.body.size() > 0) {
            // Parse the JSON payload
            crow::json::rvalue json_data = crow::json::load(req.body);

            // Check if JSON parsing is successful
            if (!json_data) {
                return crow::response(400, "Bad Request: Invalid JSON");
            }

            // Check the client sent the correct number of items:
            int orgin_square, target_square;            
            int board[8][8];                
            for(int i=0; i<64; ++i){
                int row = i/8;
                int col = i%8;
                board[row][col] = json_data[i].i();
            }

            // initiate root node
            Game_Node root_node(board, 1);
            //root_node.print_board();

            // build out the game tree, N nodes deep.            
            tree<Game_Node> game_tree;
            tree<Game_Node>::iterator top = game_tree.set_head(root_node);
            
            root_node.new_minimax(game_tree, top, 4);
            //root_node.build_tree(game_tree, 4);      
            std::cout << "Num Nodes: " << game_tree.size() << std::endl;
            //root_node.calc_scores(game_tree);
            

            Move m = root_node.find_winning_move(game_tree);
            std::pair<int,int> to_make = root_node.translate_move(m);
           
            orgin_square = to_make.first;
            target_square = to_make.second;     
            

			crow::json::wvalue r({
				{"origin", orgin_square}, {"destination", target_square}
			});			
			
			return crow::response(200, r);				 
			
        }
        
        else{
            return crow::response(400, "Bad Request: Empty Body");
        } 
        
    });

    app.port(9999).multithreaded().run();

    return 0;
}