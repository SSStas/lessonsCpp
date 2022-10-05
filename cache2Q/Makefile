#resulting files names
EXECUTABLE=cache2q bestCache

#files with source code
SOURCE_cache2q=main.cpp 
SOURCE_bestCache=bestCache.cpp

#compilation options
CXX=g++
CFLAGS= -Wall -O2


all: $(EXECUTABLE);


$(EXECUTABLE):
	$(CXX) $(CFLAGS) $(SOURCE_$@) -o $@
		

clean:
	rm -rf *.o $(EXECUTABLE)
