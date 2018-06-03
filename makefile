#compiler used
CXX = g++
#language version
cv = -std=++17
#dependencies
FILES = parser.h parser.cpp main.cpp 
#flags
FLAGS = -ggdb -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
#libraries
LIBS = -lncurses -I /usr/lib/boost_1_64_0/
#executable name
OUT_EXE = analyzer.exe

#target definition
analyzer: $(FILES)
	$(CXX) $(CV) $(FLAGS) -o $(OUT_EXE) $(FILES) $(LIBS)

clean:
	rm .*.* | rm .*.*~ | rm *.*~
