Shell and Language Analyzer (prototype)

-naive implementation of a Shell, Lexer/Tokenizer, and Lexical Parser
-production rules are as follows (my knowledge/understanding is very much so introductory. corrections
and feedback of course always welcome):

    language grammars:
     
      COMMAND -> VP
               | VP PP
     
      VP      -> verb
               | verb object
               | verb noun
               | verb PP
     
      PP      -> preposition object
               | preposition noun

    where VP = verb phrase, PP = prepositional phrase
     

-this is just a prototype so everything is very simple. i attempted to develop
the language based on MUDs/Text-based games i have played because i am in the
process of developing a very small text-based game which will require command
input from the user that is very similar. that is, expressions in the form of:

    get book from shelf
    open door
    jump
    walk north
    punch rat
    push wall

-etc. etc. etc.

-these rules only define expressions comprised of what i'll call simple
terminal types, or perhaps terminals that are atomic and not compound, or composite
 
    consider:

      knife vs a well-sharpened knife

-for the grammars to account for compound types, i have to re-define nouns to include
modifiers, as well as what delimiters the parser looks for as it reads tokens.

-i have some ideas for what changes need to be made but at the moment, i'm
just trying to develop certain components in isolation, testing each one to
make sure each runs independently, and then hoping to connect them
all together after i've verified each one does what it is supposed
to do. i can always refine later down the road.

-for example, i wrote the Shell, made sure it worked, then wrote the Parser,
and made sure it worked, before having the Shell send input to the Parser,
and running them together.    
 
-anyway i've included a makefile which you can of course modify but so to run the
program as is, first make sure you have the NCurses API and Boost libraries on your
machine, an up to date version of GCC, or some equivalent C++ compiler, and at least
C++11, then execute the makefile:

    make analyzer

-and for those who hate clutter:

    make clean

-this will clean up the directory of any duplicates or clutter-files
generated throughout any kind of development/editing phase.

-the way program works:

-first, i have a file that provides definitions (token_table.txt) for the
terminals, which are (terminal, part of speech) pairs. when the Lexer is initialized,
the constructor will read these pairs from the file and store them in an std::unordered_map.

-this allows the program to generate a look-up/symbol table, which is basically a
mapping of a terminal to its English part of speech, so as a word is scanned,
the program can immediately identify its type and do something with it.

-after the Shell initializes, it is now running and will prompts the user for input. when
input is read (as a raw string), it defers control over to the Lexer, and waits until
the Lexer processes the text, at which point the Lexer will defer control back to the Shell,
unless a terminating expression is read ("exit").

-when the Lexer receives control, a stringstream is generated

-the Lexer tokenizes each word and creates an object with (token, type)
pairings, where token is the terminal-literal, and the type is the English
part of speech as defined by the English language.

-once the stream is tokenized, a token_stream is created, and the parser reads
tokens from the token_stream and makes decisions based on the token's type.

-once an expression is processed, the Parser will let you know whether or not it was
valid or invalid expression based on the possible words that can be used to express
a command, which at the moment is a very small set of words.

-to close the program, provide the Shell the string: "exit", and the shell
will terminate.

