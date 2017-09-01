# rule to print variables using command line
print-% : ; @echo $* = $($*)

COMPILER:=g++
D_CFLAGS:= -O0 -Wall
R_CFLAGS:= -O2
LFLAGS  := 
EXEC    := mp_exec
BIN     := bin
DOBJ    := objs_d
ROBJ    := objs_r
SRC     := src
INCLUDES:=-Iheaders/

src    := $(wildcard $(SRC)/*.cpp)
objs   := $(patsubst $(SRC)/%.cpp, %.o, $(src))
d_objs := $(patsubst %.o, $(DOBJ)/%.o, $(objs))
r_objs := $(patsubst %.o, $(ROBJ)/%.o, $(objs))

all: build

make_obj_dir:
	mkdir -p $(DOBJ)
	mkdir -p $(ROBJ)

build: debug release

debug: $(d_objs)
	mkdir -p $(BIN)
	$(COMPILER) $< -o $(BIN)/$(EXEC)_d $(LFLAGS)

release: $(r_objs)
	mkdir -p $(BIN)
	$(COMPILER) $< -o $(BIN)/$(EXEC) $(LFLAGS)

$(DOBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(DOBJ)
	$(COMPILER) $(D_CFLAGS) $(INCLUDES) -c $< -o $@

$(ROBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(ROBJ)
	$(COMPILER) $(R_FLAGS) $(INCLUDES) -c $< -o $@

clean: 
	rm -r -f $(DOBJ) $(ROBJ)
	rm -f $(BIN)/$(EXEC)*

rebuild: clean build
