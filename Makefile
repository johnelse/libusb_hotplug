PROGRAM=libusb_hotplug

$(PROGRAM): $(PROGRAM).cpp
	g++ -ggdb -std=c++14 -pthread $< -o $@ $(shell pkg-config --cflags --libs libusb-1.0)

clean:
	rm -f $(PROGRAM)
