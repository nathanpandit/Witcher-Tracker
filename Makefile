SRCS = src/main.cpp src/Alchemy.cpp src/Bestiary.cpp src/Inventory.cpp

TARGET = witchertracker

default: $(TARGET)

$(TARGET): $(SRCS)
	g++ -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o
