# cheapWheel2NX
RP2040 Prototype for making a cheap steering wheel work with racing games on a switch  

# Description
This is essentially a mashup of the host_hid_to_device_cdc example from Pico-PIO-USB and the tinyusb HID example  
to make a cheap old steering wheel work with some racing games on a switch  

# Motivation
Initially I tried Mayflash Magic-NS V1 and V2 and while my T150RS works very good with those, my cheap tracer did not.  
So I sampled some DIY solutions with usb host usb device capabilities and stuck with RP2040 because of simplicity and price :)

# Cloning & Building
```
git clone --recursive https://github.com/RetroBeef/cheapWheel2NX
cd cheapWheel2NX
mkdir build
cd build
cmake ..
make
```


# Pico Prototypes with USB-A
![Pico Prototypes](extras/img/proto.jpg?raw=true "Pico Prototypes with USB-A")

# Cheap old Steering Wheel
![Steering Wheel](extras/img/wheel.jpg?raw=true "steering wheel with pedals")
