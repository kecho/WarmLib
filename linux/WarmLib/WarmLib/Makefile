# Kleber Garcia (c) 2012
LEVEL = ../..

SRC_FILES = WarmWorkerManager.cpp\
	       WarmWorker.cpp\
	       WarmWorkerMessage.cpp\
	       WarmWorkerThreadSafeStateBoard.cpp\
	       WarmWorkerPlugin.cpp\
               WarmWorkerMessageReceiver.cpp\
	       WarmWorkerPoolManager.cpp

WWORKER_SRC_FILE = $(SRC_PATH)/WarmWorkerMain.cpp

include $(LEVEL)/Makefile.inc

WORKER = $(BIN_PATH)/wworker

$(TARGET) : $(LIB_NAME) $(WORKER)

$(WORKER) : $(WWORKER_SRC_FILE)
	$(CC) $(WWORKER_SRC_FILE) $(addprefix -I, $(INCLUDE_PATHS)) -L$(BIN_PATH) -l$(TARGET) -lHotQ -lColdSdk -lpthread -ldl -o $(WORKER) 
