libusb_hotplug: libusb_hotplug.cpp
	g++ -ggdb -std=c++14 $< -o $@ $(shell pkg-config --cflags --libs libusb-1.0)

clean:
	rm -f libusb_hotplug
