#lbp_svm makefile

#
#flags
#
CC = g++
CCFLAGS = -Wall -Wextra

#
#project files
#
SRCS = test.c detect.c lbpFeature.c lbpFeatureSvm.c detectMultiScale.c pcaDimReduce.c ycbcr.c
OBJS = $(SRCS:.c=.o)
BIN = test detect

#
#include files
#
LIB = -l opencv_core -l opencv_highgui -l opencv_ml -l opencv_imgproc -l opencv_objdetect

#
#build settings
#
DBGOBJDIR = obj/debug/
RELOBJDIR = obj/release/
DBGOBJS = $(addprefix $(DBGOBJDIR)/, $(OBJS))
RELOBJS = $(addprefix $(RELOBJDIR)/, $(OBJS))

DBGCFLAGS = -g
RELCFLAGS = -o3  

DBGBINDIR = bin/debug/
RELBINDIR = bin/release/
BINDBG = $(addprefix $(DBGBINDIR)/, $(BINS)) 
BINREL = $(addprefix $(DBGBINDIR)/, $(BINS)) 

.PHONY: all clean debug prep release remake test detect

all:prep release

detect:$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o detect $(LIB)
test1:$(OBJS)
	$(CC)  $(CFLAGS) $(OBJS) -o test1 $(LIB)
test.o:test.cpp lbpFeatureSvm.h
	$(CC) -c $(CFLAGS) $(LIB) test.cpp lbpFeatureSvm.h
multiImg.o:multiImg.cpp multiImg.h
	$(CC) -c $(CFLAGS) $(LIB) multiImg.cpp multiImg.h
lbpFeature.o:lbpFeature.cpp lbpFeature.h multiImg.h
	$(CC) -c $(CFLAGS) $(LIB) lbpFeature.cpp lbpFeature.h multiImg.h
lbpFeatureSvm.o:lbpFeatureSvm.cpp lbpFeatureSvm.h multiImg.h
	$(CC) -c $(CFLAGS) $(LIB) lbpFeatureSvm.cpp lbpFeatureSvm.h multiImg.h
detectMultiScale.o:detectMultiScale.cpp lbpFeatureSvm.h
	$(CC) -c $(CFLAGS) $(LIB) detectMultiScale.cpp lbpFeatureSvm.h
pcaDimReduce.o:pcaDimReduce.cpp pcaDimReduce.h
	$(CC) -c $(CFLAGS) $(LIB) pcaDimReduce.cpp pcaDimReduce.h
ycbcr.o:ycbcr.cpp ycbcr.h
	$(CC) -c $(CFLAGS) $(LIB) ycbcr.cpp ycbcr.h


clean:
	rm -f $(OBJS)
