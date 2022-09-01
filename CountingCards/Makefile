CC = g++

CPPFLAGS = -Wall -g -std=gnu++17
# dependencies:
DEPS = dealer.hpp hand.hpp shoe.hpp bank.hpp
# object files:
OBJ = main.o dealer.o hand.o shoe.o

all: counting

counting: $(OBJ)
	$(CC) $(CPPFLAGS) -o counting $(OBJ)

main.o: main.cpp shoe.hpp bank.hpp dealer.hpp
	$(CC) $(CPPFLAGS) -c $<

dealer.o: dealer.cpp dealer.hpp shoe.hpp hand.hpp bank.hpp
	$(CC) $(CPPFLAGS) -c $<

hand.o: hand.cpp shoe.hpp hand.hpp 
	$(CC) $(CPPFLAGS) -c $<

shoe.o: shoe.cpp shoe.hpp
	$(CC) $(CPPFLAGS) -c $<

clean:
	rm -f counting $(OBJ)

####  NOTES TO SELF BELOW    ###############

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
#  -std=c++17 - this flag sets c++ version to 17, but xcode uses GNU

# symbol definitions:
#  $@ - file on left side of :	  (colon)
#  $^ - right side of : (name of all prerequesites w/ spaces inbetween)
#  $< - first item in dependencies list (right of colon ?)
#  % - pattern matching. %.c: %.o matches filenames that end in .c to 
#      filesnames that end in .o

#counting rule could look like:
# counting: $(OBJ)
#     $(CC) $(CPPFLAGS) -o $@ $^   

# $(CC) -c -o $@ $< $(CFLAGS)

#%.o: %.cpp $(DEPS)
#	$(CC) $(CPPFLAGS) -c $<

# TODO: try *.o on counting rule instead of OBJ