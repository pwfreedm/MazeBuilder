########################################################
# Makefile for CSCI 435 
# Authors: Peter Freedman, Dr. Zoppetti
########################################################

########################################################
# Variable definitions
########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# Include directories, prefaced with "-I"
INCDIRS  := 

# C++ compiler flags
# Use the first for debugging, the second for release
#CXXFLAGS := -g -Wall -std=c++20 $(INCDIRS)
CXXFLAGS := -O3 -Wall -std=c++20 $(INCDIRS)

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Linker flags. Usually none.
LDFLAGS := 

# Library paths, prefaced with "-L". Usually none.
LDPATHS := 

# Executable name. 
EXEC := M.o

# Libraries used, prefaced with "-l".
LDLIBS := 

#programs to target with the build
TARGET := Driver
#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#   	  recipe
#############################################################
make: $(TARGET)

$(TARGET) : $(TARGET).cpp
		$(CXX) $(CXXFLAGS) -o $(EXEC) $(TARGET).cpp
#############################################################
# Clean Section
#	Everything in this block defines the function of 
#	"make clean"
#############################################################
clean:
	$(RM) $(EXEC) 
