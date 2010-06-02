OPTIONS=-g -DDEBUG
#OPTIONS=-O3 -DNDEBUG
CC=gcc
CPP=g++
CFLAGS=-I/usr/local/include -I/opt/include -Wall -W -ansi -pedantic $(OPTIONS)
CXXFLAGS=-I/usr/local/include -I/opt/include -I./libspatialite/headers -Wall -W -ansi -pedantic -Wno-long-long $(OPTIONS)
LDFLAGS=-L./obj -L/opt/lib -L/usr/local/lib
EXEC=parser
LIBS=-ldl -lstdc++ -lpthread -lgeos -lgeos_c -lproj -lexpat
#-lspatialite

SRC_OSM=changeset.cpp element.cpp member.cpp node.cpp point.cpp relation.cpp top.cpp way.cpp
SRC_XML=parserbounds.cpp parser.cpp parsernd.cpp parserosmchange.cpp parserplanet.cpp parsertag.cpp parserchangeset.cpp parsermember.cpp parsernode.cpp parserosm.cpp parserrelation.cpp parserway.cpp
SRC_SPLT=spatialite.c sqlite3.c
SRC_MAIN= baseosm.cpp basesqlite3.cpp main.cpp spatialite.cpp
# basefreeroute.cpp

OBJ_OSM=$(SRC_OSM:%.cpp=osm/%.o)
OBJ_XML=$(SRC_XML:%.cpp=xml/%.o)
OBJ_SPLT=$(SRC_SPLT:%.c=libspatialite/%.o)
OBJ_MAIN=$(SRC_MAIN:%.cpp=%.o)

OBJ=$(OBJ_OSM) $(OBJ_XML) $(OBJ_SPLT) $(OBJ_MAIN)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CPP) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.cpp %.h
	$(CPP) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(OBJ)
	rm -rf $(EXEC)

run:
	make all
	rm -f *.db
	bzcat andorra.osm.bz2 | ./$(EXEC) test.db > $(EXEC).log
