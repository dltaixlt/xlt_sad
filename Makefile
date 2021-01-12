#
# Makefile for xlt speech and music classification
#   make
#   make clean	- remove object file, binaries
#
# $Id: Makefile 1.0 drizztdu 2020/03/27 dicheck $

# This makefile uses gcc and GNU make
# Tested with gcc 4.4.1, GNU make v3.81 and GNU Binutils 2.20

# Command line tools
CC = gcc
LD = gcc
AR = ar

# Directory names
APP_SRC_DIR = .
LIB_SRC_DIR = ./src
INC_DIR = ./include
LIB_DIR = ./src/lib

# Artefact names
LIB = $(LIB_DIR)/libxltsad.a
EXE = xltsad

# Library and application sources and objects
LIB_SRCS = $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_OBJS = $(LIB_SRCS:.c=.o)

APP_SRCS = $(wildcard $(APP_SRC_DIR)/*.c)
APP_OBJS = $(APP_SRCS:.c=.o)

# Flags for the C compiler, linker
CFLAGS = -c -g -I$(INC_DIR)
LDFLAGS = -L$(LIB_DIR)
ARFLAGS = rcs
LIBS = -lxltsad -lm 

all : $(EXE)

$(LIB_DIR) : 
	@echo Creating library directory
	@mkdir -p $(LIB_DIR)

$(EXE) : $(LIB_DIR) $(LIB_OBJS) $(LIB) $(APP_OBJS)
	@echo Linking executable
	@echo $(LD) $(LDFLAGS) $(APP_OBJS) -o $(EXE) $(LIBS) 
	@$(LD) $(LDFLAGS) $(APP_OBJS) -o $(EXE) $(LIBS)

%.o: %.c
	@echo Compiling $<
	@echo $(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(LIB_OBJS)
	@echo Creating $(LIB)
	@echo $(AR) $(ARFLAGS) $(LIB) $(LIB_OBJS)
	@$(AR) $(ARFLAGS) $(LIB) $(LIB_OBJS)

clean : 
	@echo Cleaning Up
	@rm $(EXE) $(LIB_OBJS) $(APP_OBJS) $(LIB) $(SHARELIB)

