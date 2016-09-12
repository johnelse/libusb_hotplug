libusb_hotplug:
	g++ -ggdb -std=c++14 libusb_hotplug.cpp -o $@ $(shell pkg-config --cflags --libs libusb-1.0)

clean:
	rm -f libusb_hotplug
