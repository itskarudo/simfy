SOURCES := pixl.cpp

CFLAGS=`pkg-config --cflags opencv4`
LDFLAGS=`pkg-config --libs opencv4`

TARGET := pixl

$(TARGET): $(SOURCES)
	$(CXX) $(LDFLAGS) $(CFLAGS) $(SOURCES) -o $@
