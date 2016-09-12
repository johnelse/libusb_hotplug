#include <iomanip>
#include <iostream>

#include <libusb.h>

class Discovery
{
public:
    Discovery()
    {
    }

    ~Discovery()
    {
    }

    void Start()
    {
        std::cout << "Start" << std::endl;
        if (!started)
        {
            int ret;
            ret = libusb_hotplug_register_callback(nullptr,
                                                   (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
                                                   LIBUSB_HOTPLUG_ENUMERATE,
                                                   LIBUSB_HOTPLUG_MATCH_ANY, // vid
                                                   LIBUSB_HOTPLUG_MATCH_ANY, // pid
                                                   LIBUSB_HOTPLUG_MATCH_ANY, // class
                                                   OnUsbHotplugCallback,
                                                   (void*)this,
                                                   &handle);

            if (LIBUSB_SUCCESS == ret)
            {
                started = true;
            }
        }
    }

    void Stop()
    {
        std::cout << "Stop" << std::endl;
        libusb_hotplug_deregister_callback(nullptr, handle);
        started = false;
    }

    static void LogVidPid(int16_t vid, int16_t pid)
    {
        std::cout << "Got a device: " << std::hex << vid << "," << pid << std::dec << std::endl;
    }

private:
    int OnUsbHotplug(struct libusb_context *ctx,
                     struct libusb_device *device,
                     libusb_hotplug_event event)
    {
        struct libusb_device_descriptor descriptor;

        int ret = libusb_get_device_descriptor(device, &descriptor);
        if (LIBUSB_SUCCESS == ret)
        {
            LogVidPid(descriptor.idVendor, descriptor.idProduct);
        }
        else
        {
            std::cout << "Got a device but failed to look up its descriptor" << std::endl;
        }
    }

    static int OnUsbHotplugCallback(struct libusb_context *ctx,
                                    struct libusb_device *device,
                                    libusb_hotplug_event event,
                                    void* discovery)
    {
        ((Discovery*)discovery)->OnUsbHotplug(ctx, device, event);
    }

    bool started;
    libusb_hotplug_callback_handle handle;
};

int main(int argc, char** argv)
{
    libusb_init(nullptr);
    std::cout << "Press return to quit..." << std::endl;

    Discovery discovery;
    discovery.Start();

    std::cin.get();

    discovery.Stop();

    libusb_exit(nullptr);
    return 0;
}

