CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=

VPATH=src:src/xml
OBJDIR=output

SRC=record.cpp utils.cpp main.cpp pugixml.cpp
OBJ=$(SRC:%.cpp=$(OBJDIR)/%.o)
BIN=stdf2xml

all: stdf2xml

stdf2xml: $(OBJ)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJ)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf $(OBJ) $(BIN)
