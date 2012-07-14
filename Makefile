CXX_FLAGS = -ggdb -Wall -Werror 

all: mbr_reader

mbr_reader: mbr_reader.cpp
	$(CXX) $(CXX_FLAGS) mbr_reader.cpp -o mbr_reader

clean:
	rm -f *.o
	rm -f mbr_reader
