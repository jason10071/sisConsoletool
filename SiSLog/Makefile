###############################################################################
# compiler setting
###############################################################################
CC       ?= gcc
CXX      ?= g++
CFLAGS   += -g -Wall -fexceptions
CXXFLAGS = $(CFLAGS) -Weffc++ -fexceptions
LIBS     = -lm
INCPATH  = -I.
DIR      = $(shell pwd)
BASENAME = $(shell basename $(DIR))

###############################################################################
# source files setting
###############################################################################
C_SOURCES        = $(shell find . -name "*.c")
CXX_SOURCES      = $(shell find . -name "*.cpp")
C_STATIC_OBJS    = $(patsubst %.c,%_static.o,$(wildcard $(C_SOURCES)))
CXX_STATIC_OBJS  = $(patsubst %.cpp,%_static.o,$(wildcard $(CXX_SOURCES)))
C_DYNAMIC_OBJS   = $(patsubst %.c,%_dynamic.o,$(wildcard $(C_SOURCES)))
CXX_DYNAMIC_OBJS = $(patsubst %.cpp,%_dynamic.o,$(wildcard $(CXX_SOURCES)))
STATIC_OBJS      = $(C_STATIC_OBJS)  $(CXX_STATIC_OBJS)
DYNAMIC_OBJS     = $(C_DYNAMIC_OBJS) $(CXX_DYNAMIC_OBJS)
LIB_DIR          = ../lib
LIB_A            = "$(LIB_DIR)/lib$(BASENAME).a"
LIB_SO           = "$(LIB_DIR)/lib$(BASENAME).so"

###############################################################################
.PHONY : clean clean_all
###############################################################################
#all: $(LIB_DIR) $(LIB_A) $(LIB_SO)
all: $(LIB_DIR) $(LIB_A)

%_static.o:%.c
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@

%_static.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

%_dynamic.o:%.c
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@ -fPIC

%_dynamic.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@ -fPIC
	
$(LIB_A): $(STATIC_OBJS)
	$(AR) rcs $@ $(STATIC_OBJS)
	@echo 'Finished building: $@'

$(LIB_SO): $(DYNAMIC_OBJS)
	$(CXX) $(DYNAMIC_OBJS) -shared -o $@ $(CXXFLAGS)
	@echo 'Finished building: $@'

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)
###############################################################################
clean:
	@rm -vfr $(STATIC_OBJS) $(DYNAMIC_OBJS)
clean_all: clean
	@rm -vfr $(LIB_A)  $(LIB_SO)
###############################################################################
