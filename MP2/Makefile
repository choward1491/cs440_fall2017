# rule to print variables using command line
print-% : ; @echo $* = $($*)

# Main macros for compilation related objects
COMPILER:=g++
D_CFLAGS:= -O0 -g -Wall -std=c++11
R_CFLAGS:= -O2 -std=c++11
LFLAGS  := -lpng16 -lz -lm 
EXEC    := mp_exec
BIN     := bin
DOBJ    := objs_d
ROBJ    := objs_r
SRC     := src
INCLUDES:=-Isrc/ -I/usr/include/libpng16

# setup source files, object files
src    := $(wildcard $(SRC)/*.cpp)
objs   := $(patsubst $(SRC)/%.cpp, %.o, $(src))
d_objs := $(patsubst %.o, $(DOBJ)/%.o, $(objs))
r_objs := $(patsubst %.o, $(ROBJ)/%.o, $(objs))

# define make commands
all: build

rebuild: clean build

build: debug release

debug: $(d_objs)
	mkdir -p $(BIN)
	$(COMPILER) $^ -o $(BIN)/$(EXEC)_d $(LFLAGS)

release: $(r_objs)
	mkdir -p $(BIN)
	$(COMPILER) $^ -o $(BIN)/$(EXEC) $(LFLAGS)

$(DOBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(DOBJ)
	$(COMPILER) $(D_CFLAGS) $(INCLUDES) -c $< -o $@

$(ROBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(ROBJ)
	$(COMPILER) $(R_CFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	rm -r -f $(DOBJ) $(ROBJ)
	rm -f $(BIN)/$(EXEC)*
