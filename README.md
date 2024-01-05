# cheapWheel2NX
RP2040 Firmware for making cheap old steering wheels work with racing games on a switch  

# Description
This was essentially a mashup of the host_hid_to_device_cdc example from Pico-PIO-USB and the tinyusb HID example to make a cheap old steering wheel work with some racing games on a switch, but now it also supports the _Logitech Speed Force Wii_ wheel, which I got for 10€ on ebay.  

# Motivation
Initially I tried Mayflash _Magic-NS V1_ and _V2_ and while my _Thrustmaster T150RS_ works very good with those, my cheap _tracer_ and the _Logitech Speed Force Wii_ did not.  
So I tried to DIY some solutions with usb host and device capabilities and stuck with RP2040 because of simplicity and price :)

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

# Logitech Speed Force Wii Steering Wheel
![Logitech Speed Force Wii](extras/img/speedforce.jpg?raw=true "Logitec Speed Force Wii")
