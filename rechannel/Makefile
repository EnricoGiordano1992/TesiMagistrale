DESIGN_NAME=mul
ABSTRACTION_LEVEL=RTL

EXECUTABLE=bin/$(DESIGN_NAME)_$(ABSTRACTION_LEVEL).x
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)

SYSTEMC=$$SYSTEMC_HOME
RECHANNEL=./lib/rechannel.a

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/ -I./lib/ -I./lib/Rechannel/ 
LIBDIR = $(SYSTEMC)/lib-linux64

CXX=g++
CFLAGS= -c -fpermissive -w -std=c++11
LD=g++

all: $(EXECUTABLE)

$(EXECUTABLE): obj/main_$(DESIGN_NAME)_$(ABSTRACTION_LEVEL).o 
	@echo Building $@
	$(CXX) -g $(INCDIR) -o $@ $^ $(LIBDIR)/libsystemc.a -lpthread $(RECHANNEL)

obj/main_$(DESIGN_NAME)_$(ABSTRACTION_LEVEL).o: src/main_$(DESIGN_NAME)_$(ABSTRACTION_LEVEL).cc
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $< 

clean:
	@echo Cleaning
	@rm -f obj/*.o *~
	@rm -f $(EXECUTABLE)

.PHONY: all clean
