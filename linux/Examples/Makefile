# Kleber Garcia (c) 2012

LEVEL = ..

include $(LEVEL)/Makefile.inc

EX_BIN_PATH = $(BIN_PATH)/Examples
SRC_PATH = $(LEVEL)/../examples/
INCLUDE_PATHS = $(LEVEL)/../examples/primes $(LEVEL)/../source/
WWORKER = $(BIN_PATH)/wworker

PRIME_DLL_SRC = $(SRC_PATH)/primes/plugin/*.cpp
PRIME_GEN_EXAMPLE_SRC = $(SRC_PATH)/primes/*.cpp

LIBRARIES = WarmLib \
	    ColdSdk \
            HotQ \
            pthread \
            dl

$(EX_BIN_PATH) :
	mkdir $(EX_BIN_PATH)
$(WWORKER) : 
	cp $(BIN_PATH)/wworker $(EX_BIN_PATH)/.

primedll : $(EX_BIN_PATH) $(WWORKER) $(PRIME_DLL_SRC)
	$(CC) -shared -fPIC $(addprefix -I, $(INCLUDE_PATHS)) $(PRIME_DLL_SRC) -o $(EX_BIN_PATH)/primeplugin.so

primegenexample : $(PRIME_GEN_EXAMPLE_SRC)
	$(CC) $(addprefix -I, $(INCLUDE_PATHS)) $(PRIME_GEN_EXAMPLE_SRC) -L$(BIN_PATH) $(addprefix -l, $(LIBRARIES))  -o $(EX_BIN_PATH)/primegenexample

primegen : primedll primegenexample

examples : primegen
