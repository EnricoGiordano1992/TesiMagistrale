MODULE=switch_connector

# Needed programs
CXX     ?= g++
DEL     ?= rm -f

# general compiler configuration
OPT     ?= -O3
DEBUG   ?= -g -DRC_DEBUG
OTHER  = -DSC_INCLUDE_DYNAMIC_PROCESSES \
         -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION \
         -DRC_USE_NON_OSCI_KERNEL \
         -fpermissive -std=c++11 -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255

# Build debugging version by default.
CFLAGS  ?= $(OPT) $(OTHER)
#CFLAGS ?= $(DEBUG) $(OTHER)

# Default target architecture is linux
# This is only needed to link against SystemC properly, though.
TARGET_ARCH ?= linux

# SystemC installation
# If the variable that points to SystemC installation path is not
# set, try a sane default
SYSTEMC ?= $$SYSTEMC_HOME

# ReChannel source tree
RECHANNEL2 ?= /home/enrico-giordano/TesiMagistrale/rechannel/lib
RECHANNEL2_MAKE_DIR = $(RECHANNEL2)/src
RECHANNEL2_INC_DIR = $(RECHANNEL2)/src
RECHANNEL2_LIB_DIR = $(RECHANNEL2)/lib

VREP_INC ?= /home/enrico-giordano/VREP/programming/include
VREP_REMOTE_API_INC ?= /home/enrico-giordano/VREP/programming/remoteApi


# Include directories
INCLUDES = -I. -I$(SYSTEMC)/include -I$(RECHANNEL2_INC_DIR) -I$(RECHANNEL2) -I$(VREP_INC) -I$(VREP_REMOTE_API_INC) \
           $(EXTRA_INCLUDES)

# Library paths
#NB: make sure that VREP has libremoteApi.a (if not, see makefile in this project)
LIBDIRS = -L. -L.. -L$(SYSTEMC)/lib-linux64 \
          -L$(RECHANNEL2) $(EXTRA_LIBDIRS) -L/home/enrico-giordano/VREP/programming/remoteApiBindings/lib/lib
LIBS    = -lremoteApi -static -lrechannel -lsystemc -lm -lpthread $(EXTRA_LIBS)

#
# Look for source files
#
ifndef SRCS
# source files are not specified directly
# check source directories - if not set, use current directory
SRCDIRS ?= .
# look for source files in all source directories
SRCS    := $(wildcard $(SRCDIRS:%=%/*.cpp))
endif

#
# The variable MODULE has to be set, since this determines
# the name of the executable
#
ifndef MODULE
$(error "Error: MODULE not set. Bailing out." )
else
# Name of the executable
EXE := $(MODULE).x
endif

# Object files
OBJS := $(SRCS:.cpp=.o)
# Dependency files
DEPS := $(SRCS:.cpp=.d)

# default target: build the executable
# depends on object files and static version of
# ReChannel library
$(EXE): $(OBJS)
	@echo "* Linking example application '$(MODULE)' ..."
	$(CXX) $(CFLAGS) \
	    $(INCLUDES) \
	    $(LIBDIRS) \
	    -o $@ \
	    $(OBJS) \
	    $(LIBS)

# Shortcut: build by module name
# Additionally, this target runs the test application after
# a succesful build process. Parameters to this run can be given
# in the variable ARGS.
$(MODULE): $(EXE)
	@echo "*"
	@echo "* Starting test application '$(MODULE)'..."
	@echo "*"
	./$(EXE) $(ARGS)

# rule to compile a single source file
.cpp.o:
	$(CXX) $(CFLAGS) \
	    $(INCLUDES) \
	    -o $@ \
	    -c $<

objclean:
	@echo "* Cleaning object files..."
	$(DEL) $(OBJS)

clean: objclean $(EXTRA_CLEAN)
	@echo "* Cleaning executable ..."
	$(DEL) $(EXE)
	@echo "* Cleaning dependency files ..."
	$(DEL) $(DEPS)


# Dependency handling of source files
%.d: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -MM \
	    -MT $(<:.cpp=.o) \
	    -o  $@ \
	    -c  $<

# actually process dependencies
include $(DEPS)

# $Id: Makefile,v 1.1 2007/10/09 21:52:20 felke Exp $
