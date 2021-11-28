CC = g++

INC  = ../include
LIB  = ../lib
OBJ  = ../obj
BIN  = .

PFLAGS = -Wall -W -Wpointer-arith -Wcast-qual \
         -Wcast-align -Wmissing-declarations \
         -Winline -O3 -g

GLLIBS = -lglut -lGLEW -lGL

$(LIB)/libcg.a:
	cd ..; make; cd applications

trabalho1: $(LIB)/libcg.a trabalho1.cpp
		$(CC) $(-g) $(PFLAGS) shader.cpp trabalho1.cpp -o $(BIN)/trabalho1 -I$(INC) -L$(LIB) -lcg $(GLLIBS)

.PHONY: clean

clean:
	cd ../; make clean; cd applications
	rm -f appWindow appTriangles appColoredTriangle appColoredCube \
	      appImage appPoints
