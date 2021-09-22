CXX= g++
CXXFLAGS= -I/usr/local/include -I/usr/share/qt4/mkspecs/linux-g++-64 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
LDFLAGS= -lgecodeflatzinc -lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel -lgecodeset -lgecodefloat -lmpfr -lgmp -lgecodeint -lgecodekernel -lgecodesupport -lQtGui -lQtCore
LIST= resource cpu

all: ${LIST}

resource: resource.cpp
	${CXX} ${CXXFLAGS} resource.cpp ${LDFLAGS} -o resource

cpu: cpu.cpp
	${CXX} ${CXXFLAGS} cpu.cpp ${LDFLAGS} -o cpu


clean:
	rm -f ${LIST}
