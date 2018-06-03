Shell and Language Analyzer (prototype)

-naive implementation of a Shell, Lexer/Tokenizer, and Lexical Parser
-production rules are as follows (and please forgive incorrect usages of some
words. my knowledge/understanding is very much so introductory. corrections
and feedback of course always welcomed!):

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

-i have some ideas for what changes need to be made but at the moment, i'm
just trying to develop certain components in isolation, testing each one to
make sure it runs independent of everything, and then hoping to connect them
all together.
-for example, i wrote the shell, made sure it worked, then wrote the parser,
and made sure it worked, before having the shell send input to the parser.    
 
-to run, first make sure you have the NCurses API and Boost libraries on your
machine, an up to date version of GCC, and at least C++11. if all of the above,
simply execute the makefile:

    make analyzer

-and for those who hate clutter:

    make clean

-this will clean up the directory of any duplicates or clutter-files
generated throughout any kind of development/editing phase.

-consult the makefile if you wish to see what flags are set (there are quite a
few), or wish to make modifications

-the way program works:

-first, i have a file that provides definitions (token_table.txt) for the
terminals

-this allows the program to generate a what amounts to a look-up/symbol table, which is basically a
mapping of a terminal to its English part of speech, so as a word is scanned,
the program can immediately identify it's type

-to start, the shell prompts the user and then input is grabbed as a whole or
"raw" string

-a stringstream is then generated

-the Lexer processeses each word and creates an object with (token, type)
pairings, where type is the part of speech as defined by the English language.
now that i think of it, i could probably re-write this to make use of
std::pair<>

-once the stream is tokenized, a token_stream is created, and the parser reads
words from the token_stream and makes decisions based on the token's now defined
'type'.

-once an input is processed, the shell will let you know whether or not it was
valid or invalid based on 1. the grammars, and 2. the terminal, part of speech
pairings that initialized the symbol_table used in the tokenization process.

-to close the program, provide the shell the string: "exit", and the shell
will terminate.

