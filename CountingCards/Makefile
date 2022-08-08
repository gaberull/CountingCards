# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
#  -std=c++17 - this flag sets c++ version to 17, but xcode uses GNU

CC = g++

CPPFLAGS = -Wall -g -std=gnu++17
# dependencies:
DEPS = dealer.hpp hand.hpp shoe.hpp
# object files:
OBJ = main.o dealer.o hand.o shoe.o
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

# TODO: try *.o on counting rule instead of OBJ

all: counting

counting: $(OBJ)
	$(CC) $(CPPFLAGS) -o counting $(OBJ)

%.o: %.cpp $(DEPS)
	$(CC) $(CPPFLAGS) -c $<

clean:
	rm -f counting $(OBJ)