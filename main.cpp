#include "parser.h"

int main(){

    Shell shell;
    /*main input loop*/
    while (shell._running_()){

        shell.print_prompt();
        shell.read_raw_input();
        if (shell._raw_input_() == "exit"){
            shell._exit_();
        }
        else{
            shell._update_shell_state_("waiting");
        }

        while (shell._waiting_()){
            //parse our input
            Lexer lexer(shell._raw_input_());
            clear();
            if (lexer.is_command()){
                /*do something with valid input*/
                mvprintw(LINES - 1, 0, "\"%s\" is VALID command", shell._raw_input_().c_str());
            }
            else{
                mvprintw(LINES - 1, 0, "\"%s\" is NOT VALID command", shell._raw_input_().c_str());
            }
            refresh();
            getch();

            //lexer._TEST_tokenize_();
            shell._update_shell_state_("running");
        }
        
        
    }

    return 0;
}
