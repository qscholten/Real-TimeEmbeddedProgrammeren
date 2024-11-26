
GCC := g++

OUTPUT := opgave2a
SOURCES := $(wildcard *.cpp)
CCFLAGS := -std=c++11 -pthread

all: $(OUTPUT)
	
$(OUTPUT):$(SOURCES)
	$(GCC) -o $(OUTPUT) $(CCFLAGS) $(SOURCES)
	
clean:
	rm $(OUTPUT)
	
.PHONY: all
