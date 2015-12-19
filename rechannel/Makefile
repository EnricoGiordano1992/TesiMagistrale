DESIGN_NAME=sommatore

EXECUTABLE=bin/$(DESIGN_NAME).x
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)

SYSTEMC=$$SYSTEMC_HOME
RECHANNEL=./lib/rechannel.a

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/ -I./lib/ -I./lib/Rechannel/ 
LIBDIR = $(SYSTEMC)/lib-linux64

CXX=g++
CFLAGS= -c -fpermissive -w -std=c++11 -DSC_INCLUDE_DYNAMIC_PROCESSES -DRC_USE_NON_OSCI_KERNEL
LD=g++

all: $(EXECUTABLE)

$(EXECUTABLE): obj/main_$(DESIGN_NAME).o obj/$(DESIGN_NAME)_testbench.o obj/$(DESIGN_NAME).o
	@echo Building $@
	$(CXX) -g $(INCDIR) -o $@ $^ $(LIBDIR)/libsystemc.a -lpthread $(RECHANNEL)

obj/main_$(DESIGN_NAME).o: src/main_$(DESIGN_NAME).cc
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $< 

obj/$(DESIGN_NAME)_testbench.o: src/$(DESIGN_NAME)_testbench.cc include/$(DESIGN_NAME)_testbench.hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $< 

obj/$(DESIGN_NAME).o: src/$(DESIGN_NAME).cc include/$(DESIGN_NAME).hh
	@echo Building $@
	$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<

clean:
	@echo Cleaning
	@rm -f obj/*.o *~
	@rm -f $(EXECUTABLE)

.PHONY: all clean
