CXX := g++
OSNAME := $(shell uname -s | tr A-Z a-z)
CXXFLAGS := -std=c++11 -I/usr/include/jsoncpp
LDFLAGS := -ljsoncpp
GECODECXXFLAGS :=
GECODELDFLAGS :=
PROGS := bin/DDC-RA
SRCDIR := elements
SRCS := $(shell find $(SRCDIR) -name '*.cpp')
BUILDDIR := build
TARGETDIR := bin
TARGETSRC := DDC-RA.cpp
OBJECTS := $(addprefix $(BUILDDIR)/,$(SRCS:%.cpp=%.o))
TOPOFILE := input.json


ifeq ($(OSNAME),linux)
        GECODECXXFLAGS += -I/usr/local/include -I/usr/share/qt4/mkspecs/linux-g++-64 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
	GECODELDFLAGS += -lgecodeflatzinc -lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel -lgecodeset -lgecodefloat -lmpfr -lgmp -lgecodeint -lgecodekernel -lgecodesupport -lQtGui -lQtCore
endif
ifeq ($(OSNAME),darwin)
        GECODECXXFLAGS += -F/Library/Frameworks
	GECODELDFLAGS += -framework gecode
endif


all: $(PROGS)


$(PROGS): $(OBJECTS) $(TARGETSRC)
	@echo " Linking..."$@
	@mkdir -p $(TARGETDIR)
	@$(CXX) $(CXXFLAGS) $(GECODECXXFLAGS) $(TARGETSRC) -o $(PROGS) $(OBJECTS) $(LDFLAGS) $(GECODELDFLAGS)
	@echo " Copying topology file..."
	@cp $(TOPOFILE) $(TARGETDIR)
	@echo " Done! Run $(PROGS) in $(TARGETDIR) folder"

$(BUILDDIR)/$(SRCDIR)%.o: $(SRCDIR)%.cpp
	@echo " Compiling..."$<
	@mkdir -p $(BUILDDIR)/$(SRCDIR)
	@$(CXX) $(CXXFLAGS) $(GECODECXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@echo " Cleaning..."
	@rm -fr ${BUILDDIR} ${TARGETDIR}
	@rm -fr *.csv
	@echo " Done!"
