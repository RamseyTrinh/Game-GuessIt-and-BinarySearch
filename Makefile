#
# 'make'		build executable file 'main'
# 'make clean'	removes all .o and executable files
#

# define the Cpp compiler to use
CXX				:= g++

# define any compile-time flags
CXXFLAGS		:= -std=c++17 -Wall -Wextra -flto -g

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS			:= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# define output directory
OUTPUT			:= output

# define source directory
SRC				:= src

# define include directory
INCLUDE			:= include

# define lib directory
LIB				:= lib

ifeq ($(OS),Windows_NT)
MAIN			:= main.exe
SOURCEDIRS		:= $(SRC)
INCLUDEDIRS		:= $(INCLUDE)/SDL2
LIBDIRS			:= $(LIB)
# cannot use del command on Makefile
# must use by cmd
RM				:= rm -f
MD				:= mkdir
else
MAIN			:= main
SOURCEDIRS		:= $(shell find $(SRC) -type d)
INCLUDEDIRS		:= $(shell find $(INCLUDE) -type d)
LIBDIRS			:= $(shell find $(LIB) -type d)
# FIXPATH = $1
RM				:= rm -f
MD				:= mkdir -p
endif
# define any directories containing header files other than /usr/include
# NEW_INCLUDE	:= $(patsubst %/, %, $(INCLUDEDIRS))
# INCLUDES		:= $(patsubst %,-I%, $(NEW_INCLUDE))
# the two line code above is equal to
INCLUDES		:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS			:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
# here we go with the patsubst trick to replace with wildcards
# SOURCEFILES	:= $(patsubst %,%/*.cpp, $(SOURCEDIRS))
# find all files in SOURCEDIRS that end in .cpp
SOURCES			:= $(wildcard $(SRC)/*.cpp)

# define the C object files
# replace all .cpp with .o
OBJECTS			:= $(SOURCES:.cpp=.o)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

# OUTPUTMAIN	:= $(OUTPUT)/$(MAIN)

all: $(MAIN)
	@echo Executing 'all' complete!

# $(OUTPUT):
# 	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o $(MAIN) $(OBJECTS) $(LFLAGS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# execute whenever a file 'clean' is exist or not
.PHONY: clean
clean:
	$(RM) $(SRC)/*.o
	$(RM) $(MAIN)
	@echo Cleanup complete!

run: all
	./$(MAIN)
	@echo Executing 'run: all' complete!
