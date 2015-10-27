#lbp_svm makefile

#
#flags
#
CC = g++
CFLAGS = -Wall -Wextra

#
#project files
#
SRCS = lbp_svm.cpp multiImg.cpp lbpFeature.cpp lbpFeatureSvm.cpp detectMultiScale.cpp pcaDimReduce.cpp ycbcr.cpp
OBJS = $(SRCS:.cpp=.o)
BIN = lbp_svm
#
#include files
#
LIB = -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_imgproc -lopencv_objdetect

#
#build settings
#
DBGOBJDIR = obj/debug
RELOBJDIR = obj/release
DBGOBJS = $(addprefix $(DBGOBJDIR)/, $(OBJS))
RELOBJS = $(addprefix $(RELOBJDIR)/, $(OBJS))

DBGCFLAGS = -g
RELCFLAGS = -o3  

DBGBINDIR = bin/debug
RELBINDIR = bin/release
DBGBIN =$(DBGBINDIR)/$(BIN)
RELBIN =$(RELBINDIR)/$(BIN)

.PHONY: all clean debug prep release remake

all:prep release

#
#
#
debug: $(DBGBIN)

$(DBGBIN) : $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) $^ -o $(DBGBIN) $(LIB)

$(DBGOBJDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(DEGCFLAGS) -o $@ $<
#
#
#

release: $(RELBIN)

$(RELBIN) : $(RELOBJS)
	echo $^
	$(CC) $(CFLAGS) $(RELCFLAGS) $^ -o $(RELBIN) $(LIB)
$(RELOBJDIR)/%.o:%.cpp
	#echo $@ , $<
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<


#lbp_svm.o:test.cpp lbpFeatureSvm.h
#	$(CC) -c $(CFLAGS) $(LIB) test.cpp lbpFeatureSvm.h
#multiImg.o:multiImg.cpp multiImg.h
#	$(CC) -c $(CFLAGS) $(LIB) multiImg.cpp multiImg.h
#lbpFeature.o:lbpFeature.cpp lbpFeature.h multiImg.h
#	$(CC) -c $(CFLAGS) $(LIB) lbpFeature.cpp lbpFeature.h multiImg.h
#lbpFeatureSvm.o:lbpFeatureSvm.cpp lbpFeatureSvm.h multiImg.h
#	$(CC) -c $(CFLAGS) $(LIB) lbpFeatureSvm.cpp lbpFeatureSvm.h multiImg.h
#detectMultiScale.o:detectMultiScale.cpp lbpFeatureSvm.h
#	$(CC) -c $(CFLAGS) $(LIB) detectMultiScale.cpp lbpFeatureSvm.h
#pcaDimReduce.o:pcaDimReduce.cpp pcaDimReduce.h
#	$(CC) -c $(CFLAGS) $(LIB) pcaDimReduce.cpp pcaDimReduce.h
#ycbcr.o:ycbcr.cpp ycbcr.h
#	$(CC) -c $(CFLAGS) $(LIB) ycbcr.cpp ycbcr.h


prep:
	@mkdir -p $(DBGOBJDIR) $(DBGBINDIR) $(RELOBJDIR) $(RELBINDIR)
remake: clean all

clean:
	rm -f $(DBGOBJS)
	rm -f $(RELOBJS)
