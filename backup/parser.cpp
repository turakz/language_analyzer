#include "parser.h"

/*BEGIN CLASS SHELL*/
void Shell::_update_shell_state_(std::string s){
    if (s == "running"){
        running = true;
        waiting = false;
    }
    else if (s == "waiting"){
        running = false;
        waiting = true;
    }
}

void Shell::_init_ncurses_(){
    //std::cout << "TEST: shell initialized...";
    //std::cin.clear();
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    initscr();
    nocbreak();
    echo();
}
void Shell::_destroy_ncurses_(){
    clear();
    endwin();
    //std::cout << "TEST: shell destroyed...";
    //std::cin.clear();
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Shell::_start_(){
    running = true;
    waiting = false;
}

void Shell::_exit_(){
    running = false;
    waiting = false;
}

bool Shell::_running_(){
    return (running && !waiting);
}

bool Shell::_waiting_(){
    return (!running && waiting);
}


std::string Shell::_raw_input_(){
    return raw_input;
}

void Shell::print_prompt(){
    if (running){
        clear();
        unsigned int x = 0;
        unsigned int y = LINES - 1;
        mvprintw(y, x, prompt.c_str());
        refresh();
    }
}

void Shell::read_raw_input(){
    if (running){
        raw_input.clear();
        unsigned int x = prompt.size();
        unsigned int y = LINES - 1;
        while ( char c = (mvgetch(y, x++)) ){
            if (c != '\n'){
                raw_input += c;
            }
            else{
                break;
            }
        }
    }
}

/*END CLASS Shell*/

/*BEGIN CLASS Lexer*/

/*BEGIN STRUCT Token_stream*/

void Lexer::Token_stream::_init_token_table_(const std::string filename){
    std::ifstream ifs(filename);
    if (ifs.is_open()){
        std::string line = "";
        while (std::getline(ifs, line)){
            //lines beginning with # are interpreted comments
            if (line.at(0) != '#'){
                std::stringstream ss(line);
                std::string name = "";
                std::getline(ss, name, ',');
                //skip whitespace
                if (ss.peek() == ' '){
                    ss.ignore();
                }
                std::string type = "";
                std::getline(ss, type);
                token_table.insert(std::pair<std::string, std::string>(name, type));
            }
            //ignore everything else
        }
        if (ifs.bad()){
            //hand corrupt input
        }
    }
    //handle bad file
}

std::string Lexer::Token_stream::tokenize(std::string& token){
    std::unordered_map<std::string, std::string>::iterator tt = token_table.find(token);
    if (tt != token_table.end()){
        return tt->second;
    }
    return "bad_token";
}

Lexer::Token Lexer::Token_stream::peek(std::istream& input){
    std::string token   = "";
    std::string token_t = "empty";
    if (std::getline(input, token, ' ')){
        if (token.empty()){
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        token_t = tokenize(token);
    }
    if (input.bad()){
        /*error: something went wrong*/
        token_t = "error";
    }
    //go back to our original position before read
    input.seekg(current_position);
    return Token(token, token_t);
}

Lexer::Token Lexer::Token_stream::get(std::istream& input){

    /******************************
     *
     *
     * this logic only parses sentences comprised of simple token types (one word)
     * it does not parse sentences comprised of compound token types (types made of more than one word)
     *
     * knife vs a well-sharpened knife
     *
     * remark:
     *  if we want this kind of logic, we are going to have to modify the grammars
     *  to account for expressions that are more than just
     *  VP | VP PP
     *  so something mayb like:
     *          VP | VP PP | VP MP PP where MP is a (M)odifying (P)hrase
     *   
     *
     *
     * *****************************/

    //do we have a token in buffer already?
    if (buffer_full){
        return buffer;
    }
    /*return empty token naturally, language does not include sequences with leading whitespace*/
    std::string token   = "";
    std::string token_t = "empty";
    current_position    = input.tellg();
    if (std::getline(input, token, ' ')){
        if (token.empty()){
            //we've likely read whitespace so lets dump input entirely
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        token_t = tokenize(token);
    }
    //the currrent token is the last one in stream, mark it
    if (input.eof()){
        end_of_stream = true;
    }
    else if (input.bad()){
        /*error: something went wrong*/
        token_t       = "error: input_stream";
        end_of_stream = true;
    }
    buffer_full = true;
    buffer      = Token(token, token_t);
    return buffer;
    
}

void Lexer::Token_stream::discard_token(){
    buffer_full = false;
}

void Lexer::Token_stream::putback(Token t){
    if (buffer_full){
        //we already have a token in buffer, do nothing
        return;
    }
    else{
        buffer_full = true;
        buffer      = t;
    }
}

/*END STRUCT Token_stream*/

//if we've identified a valid token, consider it 'read'
bool Lexer::prep(){
    Token t = ts.get(input_stream);
    if (t.type == "preposition"){
        tokens.push_back(t);
        ts.discard_token();
        return true;
    }
    return false;
}

bool Lexer::noun(){
    Token t = ts.get(input_stream);
    if (t.type == "noun"){
        tokens.push_back(t);
        ts.discard_token();
        return true;
    }
    return false;
}

bool Lexer::object(){
    Token t = ts.get(input_stream);
    if (t.type == "object"){
        tokens.push_back(t);
        ts.discard_token();
        return true;
    }
    return false;
}

bool Lexer::verb(){
    Token t = ts.get(input_stream);
    if (t.type == "verb"){
        tokens.push_back(t);
        ts.discard_token();
        return true;
    }
    return false;
}

bool Lexer::prep_phrase(){
    //if we have a prep, only an object or noun can follow
    if (prep()){
        if (object() || noun()){
            return true;
        }
    }
    return false;

}

bool Lexer::verb_phrase(){
    //if we have a verb, only the empty string, object, noun, or PP can follow
    if (verb()){
        return (ts.end_of_stream || object() || noun() || prep_phrase());
    }
    return false;
}   


bool Lexer::is_command(){

    /*language grammars:
     *
     * COMMAND -> VP
     *          | VP PP
     *
     * VP      -> verb
     *          | verb object
     *          | verb noun
     *          | verb PP
     *
     * PP      -> preposition object
     *          | preposition noun
     *
     *
     * */

    //everything must start with a verb 
    return verb_phrase();
}

//test functions
void Lexer::_TEST_tokenize_(){
    //pseudo-parse()
    Token t;
    while (!ts.end_of_stream){
        clear();
        t = ts.get(input_stream);
        if (ts.end_of_stream){
            mvprintw(LINES/2, (COLS/2)-30, "PROCESSED token -> (name: %s, type: %s, position: %d || ts.end_of_stream)", t.name.c_str(), t.type.c_str(), ts.current_position);
        }
        else{
            mvprintw(LINES/2, (COLS/2)-30, "PROCESSED token -> (name: %s, type: %s, position: %d)", t.name.c_str(), t.type.c_str(), ts.current_position);
        }
        refresh();
        tokens.push_back(t);
        ts.discard_token();
        getch();
    }
}
/*END CLASS Lexer*/
