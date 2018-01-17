###############################################################################
# compiler setting
###############################################################################
CC       = gcc
CXX      = g++
DEFINES  = -DAPP_VERSION=\"1.5.3.119\"
CFLAGS   += -g -Wall $(DEFINES) -pthread -fexceptions
CXXFLAGS = $(CFLAGS) -Weffc++ $(DEFINES) -pthread -fexceptions
LIBS     = -lm -lCTBase -lSiSProcedure -lSiSLog -lSiSExpressionLib -lSiSDeviceIO -L../lib -static
INCPATH  = -I. -I../SiSLog -I../SiSExpressionLib -I../SiSDeviceIO -I../SiSProcedure -I../CTBase -I../CTBase/getFirmwareId -I../CTBase/updateFW 
DIR      = $(shell pwd)

###############################################################################
# source files setting
###############################################################################
CXX_SOURCES  = $(shell find . -name "*.cpp")
CXX_OBJS     = $(patsubst %.cpp,%.o,$(wildcard $(CXX_SOURCES)))
BIN_DIR      = ../bin
EXEC_SOURCES = $(shell find . -maxdepth 1 -name "*.cpp")
EXEC         = $(subst .,$(BIN_DIR),$(basename $(EXEC_SOURCES)))

###############################################################################
.PHONY : install clean clean_all
###############################################################################
all: $(BIN_DIR) $(EXEC)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)
	
%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@
	
$(BIN_DIR)/%:%.o
	$(CXX) $< -o $@ $(LIBS) $(CXXFLAGS)
	@echo 'Finished building: $@'

###############################################################################
clean:
	@rm -vfr $(CXX_OBJS)
clean_all: clean
	@rm -vfr $(EXEC) bin
###############################################################################
