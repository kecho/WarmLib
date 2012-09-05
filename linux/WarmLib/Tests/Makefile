# Kleber Garcia (c) 2012

LEVEL=../../

TESTS_OBJ :
	$(CC) -fPIC -I ../../../source/ -c ../../../tests/*.cpp

HOTQ_LIBRARIES = HotQ \
	         pthread  

COLD_SDK_LIBRARIES = ColdSdk\
		     dl

WARM_LIB_LIBRARIES = WarmLib \
		     ColdSdk \
                     HotQ \
                     pthread \
		     dl \

include $(LEVEL)/Makefile.inc

Tests : TESTS_OBJ
	$(CC) qtest.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/qtests		
	$(CC) mutextest.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/mutextest		
	$(CC) shm_test.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/shm_test		
	$(CC) qconsumer.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/qconsumer	
	$(CC) qproducer.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/qproducer	
	$(CC) threadtest.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/threadtest	
	$(CC) hqprocesstest.o -L$(BIN_PATH) $(addprefix -l, $(HOTQ_LIBRARIES)) -o $(BIN_PATH)/hqprocesstest	
	$(CC) coldloader.o -L$(BIN_PATH) $(addprefix -l, $(COLD_SDK_LIBRARIES)) -o $(BIN_PATH)/coldloader	
	$(CC) warmworkertest.o -L$(BIN_PATH) $(addprefix -l, $(WARM_LIB_LIBRARIES)) -o $(BIN_PATH)/warmworkertest
	$(CC) pooltest.o -L$(BIN_PATH) $(addprefix -l, $(WARM_LIB_LIBRARIES)) -o $(BIN_PATH)/pooltest
	$(CC) -shared -fPIC example_cold_plugin.o -o $(BIN_PATH)/example_cold_plugin.so
