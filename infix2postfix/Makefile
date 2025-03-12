GRAMMAR = Arithmetic
PROG = infix2postfix
ANTLR_VERSION = $(shell dpkg --get-selections | grep antlr4 | grep runtime4 | egrep -o "([0-9]+\.[0-9]+)")
ANTLR = antlr-$(ANTLR_VERSION)-complete.jar
RUNTIME_INCLUDE = /usr/include/antlr4-runtime
RUNTIME_LIB = /usr/lib/x86_64-linux-gnu/libantlr4-runtime.so.$(ANTLR_VERSION)
GRAMMARSRC = $(GRAMMAR)Lexer.cpp $(GRAMMAR)Parser.cpp $(GRAMMAR)BaseListener.cpp $(GRAMMAR)Listener.cpp
SRC = $(GRAMMARSRC) $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(GRAMMAR)Parser.cpp $(PROG)

$(PROG): $(OBJ)
	$(CXX) -o $@ $^ $(RUNTIME_LIB)

%.o: %.cpp
	$(CXX) -I $(RUNTIME_INCLUDE) -c $<

$(ANTLR):
	wget https://www.antlr.org/download/$(ANTLR)

$(GRAMMARSRC): $(GRAMMAR).g4 $(ANTLR)
	CLASSPATH=$(ANTLR):$(CLASSPATH) java org.antlr.v4.Tool -Dlanguage=Cpp $<

clean:
	rm -f $(OBJ) $(PROG)

clobber: clean
	rm -f $(GRAMMAR).interp $(GRAMMAR)Lexer.interp $(GRAMMAR)Lexer.tokens $(GRAMMAR).tokens
	rm -f $(GRAMMAR)Lexer.h $(GRAMMAR)Parser.h
	rm -f $(GRAMMAR)Lexer.cpp $(GRAMMAR)Parser.cpp
	rm -f $(GRAMMAR)BaseListener.h $(GRAMMAR)Listener.h
	rm -f $(GRAMMAR)BaseListener.cpp $(GRAMMAR)Listener.cpp

dist-clean: clobber
	rm -f $(ANTLR)
