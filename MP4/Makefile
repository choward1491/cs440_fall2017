# rule to print variables using command line
print-% : ; @echo $* = $($*)

# Main macros for compilation related objects
COMPILER:=g++
D_CFLAGS:= -O0 -Wall -std=c++14
R_CFLAGS:= -O2 -std=c++14
LFLAGS  := -lpng16 -lz -lm \
			-lallegro_ttf -lallegro_color -lallegro_primitives -lallegro_font -lallegro_main -lallegro
EXEC    := mp_exec
BIN     := bin
DOBJ    := objs_d
ROBJ    := objs_r
SRC     := src src/utility src/tests src/RL src/game src/game/gui src/game/logic
INCLUDES:= $(addprefix -I,$(SRC)) -I/usr/include/libpng16 -I/usr/include/

# setup source files, object files
src    := $(foreach SRC, $(SRC), $(wildcard $(SRC)/*.cpp))
objs   := $(notdir $(patsubst %.cpp, %.o, $(src)))
d_objs := $(patsubst %.o, $(DOBJ)/%.o, $(objs))
r_objs := $(patsubst %.o, $(ROBJ)/%.o, $(objs))

# setup vpath
VPATH = $(SRC)

# compile shit
all: build

rebuild: clean build

build: debug release

debug: $(d_objs)
	mkdir -p $(BIN)
	$(COMPILER) $^ -o $(BIN)/$(EXEC)_d $(LFLAGS)

release: $(r_objs)
	mkdir -p $(BIN)
	$(COMPILER) $^ -o $(BIN)/$(EXEC) $(LFLAGS)

$(DOBJ)/%.o: %.cpp
	mkdir -p $(DOBJ)
	$(COMPILER) $(D_CFLAGS) $(INCLUDES) -c $< -o $@

$(ROBJ)/%.o: %.cpp
	mkdir -p $(ROBJ)
	$(COMPILER) $(R_CFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	rm -r -f $(DOBJ) $(ROBJ)
	rm -f $(BIN)/$(EXEC)*
