CXX = g++
CXXFLAGS = -g -Wall -std=c++17
SRCS += djynkz.cpp \
WavReader.cpp \
Sound.cpp \
WavWriter.cpp \
Note.cpp \
NoteContainer.cpp \
LowPassFilter.cpp \
Distortion.cpp \
CabinetModeler.cpp \
MetalZone.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = djynkz.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	del /Q *.o *.exe