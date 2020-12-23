# Makefile for fonc

.PHONY: all clean

SRCS = chattabitem.cpp connectdlg.cpp dnsresolver.cpp main.cpp nakenclient.cpp \
			 ngnc.cpp prefsdlg.cpp prefs.cpp icons.cpp

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

CC? = gcc
CXX? = g++

# Dependencies
DEP_INCLUDES = `fox-config --cflags`
DEP_LIBS = `fox-config --libs` -lcrypto -lssl

CFLAGS = -Wall -g3 -I.

EXE = ngnc

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CFLAGS) -o $(EXE) $(OBJS) $(DEP_LFLAGS) $(DEP_LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(DEP_INCLUDES) -MMD -MP -MT $@ -o $@ -c $<

# reswrap isn't distributed with some distros
#icons.h: data/spy.ico
#	reswrap -o icons.h -i -z -r spy_ico data/spy.ico
#icons.o: icons.h
#	reswrap -o icons.cpp -z -e -r spy_ico data/spy.ico

clean:
	rm -f $(OBJS) $(EXE) $(DEPS)

# Include automatically generated dependency files
-include $(DEPS)
