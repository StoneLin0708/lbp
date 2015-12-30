#lbp_svm Makefile

#
#flags
#
CC = g++
CFLAGS = -Wall -Wextra -o3 `pkg-config --cflags opencv`

#
#project files
#
SRCSA = sampleList.cpp
SRCSB = sampleCut.cpp multiImg.cpp
OBJSA = $(SRCSA:.cpp=.o)
OBJSB = $(SRCSB:.cpp=.o)
BINA = sampleList
BINB = sampleCut
#
#include files
#
LIB = `pkg-config --libs opencv`

#
#build settings
#
SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./

RELOBJSA = $(addprefix $(OBJDIR)/, $(OBJSA))
RELOBJSB = $(addprefix $(OBJDIR)/, $(OBJSB))

.PHONY: all clean prep releaseA releaseB remake

all:prep releaseA releaseB

#
#releaseA
#
releaseA: prep $(BINA)
	@echo "build !"

$(BINDIR)/$(BINA) : $(RELOBJSA)
	$(CC) $(CFLAGS) $^ -o $(BINA) $(LIB)
	@echo "Linked !"

$(RELOBJSA) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" !"

#
#releaseB
#
releaseB: prep $(BINB)
	@echo "build !"

$(BINDIR)/$(BINB) : $(RELOBJSB)
	$(CC) $(CFLAGS) $^ -o $(BINB) $(LIB)
	@echo "Linked !"

$(RELOBJSB) : $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" !"
#
#other
#
prep:
	@mkdir -p $(OBJDIR) $(BINDIR)

remake: clean all

clean:
	rm -f $(OBJS)
	rm -f $(BIN)
