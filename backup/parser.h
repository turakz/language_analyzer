#ifndef _PARSER
#define _PARSER

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <ncurses.h>

class Shell{

    private: 
        std::string prompt, raw_input;

    public:
        //internals
        bool running, waiting;
        Shell()
            :prompt("{ ~ } >> "), raw_input(""), running(false), waiting(false){
                //initialize ncurses session
                _init_ncurses_();
                //initialize shell session
                _start_();
            }
        ~Shell(){
                //end ncurses session
                _destroy_ncurses_();
                //end shell session
                _exit_();
            }
        void _update_shell_state_(std::string s);
        void _init_ncurses_();
        void _destroy_ncurses_();
        //state
        void _start_();
        void _exit_();
        bool _running_();
        bool _waiting_();
        bool _is_exiting_();
        //input
        std::string _raw_input_();
        void print_prompt();
        void read_raw_input();

};

class Lexer{
    
    private:
        std::stringstream input_stream;
        struct Token{
            std::string name;
            std::string type;

            Token()
                :name(""), type(""){
                    /**/
                }

            Token(std::string n, std::string k)
                :name(n), type(k){
                    /**/
                }
        };
        struct Token_stream{
            int current_position;
            bool end_of_stream;
            std::unordered_map<std::string, std::string> token_table;
            bool buffer_full;
            Token buffer;

            Token_stream()
                :current_position(0), end_of_stream(false), buffer_full(false){
                    _init_token_table_("token_table.txt");
                }
            void _init_token_table_(const std::string filename);
            std::string tokenize(std::string& token);
            Token peek(std::istream& is);
            Token get(std::istream& is);
            void discard_token();
            void putback(Token t);

        };
        Token_stream ts;
        std::vector<Token> tokens;
    
    public:
        //internals
        Lexer(std::string raw_input){        
            input_stream.str(raw_input);
        }
        bool prep();
        bool noun();
        bool object();
        bool verb();
        bool prep_phrase();
        bool verb_phrase();
        bool is_command();

        void _TEST_tokenize_();

};

/**OTHER CLASSES
 *
 *
 *
 *if we are going to implement other classes which require interaction with a window
 *then we should probably create an abstract window class that has a bunch of wrapped
 *ncurses functions to manage windows (draw, delete, print, refresh, destroy)
 *
 *
 *
 *
 * */
#endif

