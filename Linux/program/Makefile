###############################################################################
# compiler setting
###############################################################################
CC       = gcc
CXX      = g++
CFLAGS   += -g -Wall
CXXFLAGS = $(CFLAGS) -Weffc++
LIBS     = -lm -lSiSTouch -lSiSTouchIO817 -L../lib -static
INCPATH  = -I../SiSTouch -I../SiSTouchIO817
DIR      = $(shell pwd)

###############################################################################
# source files setting
###############################################################################
CXX_SOURCES = $(shell find . -name "*.cpp")
CXX_OBJS    = $(patsubst %.cpp,%.o,$(wildcard $(CXX_SOURCES)))
BIN_DIR     = ../bin
EXEC        = $(subst .,$(BIN_DIR),$(basename $(CXX_SOURCES)))

###############################################################################
.PHONY : install clean clean_all
###############################################################################
all: $(BIN_DIR) $(EXEC)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)
%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@
$(BIN_DIR)/%:%.o ExitStatus.h version.h
	$(CXX) $< -o $@ $(LIBS) $(CXXFLAGS)
	@echo 'Finished building: $@'

###############################################################################
clean:
	@rm -vfr $(CXX_OBJS)
clean_all: clean
	@rm -vfr $(EXEC) bin
###############################################################################
