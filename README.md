
An AVR Atmega328 library -- cavr328


The purpose of this library is to provide a complete set of helper functions library for manipulating ATMega328 microcontroller. Since this microcontroller is the very same that is used on Arduino, this library could be directly loaded onto Arduino Uno as well.

Although the Arduino has provided a lot of easy-to-use functions and object-oriented C++ API, it seems to lack some basic functionalities to manipulate hardware directly, which is desirable for any practical purposes with more than just a init function and a main loop. For example, direct control of timers and interrupts (except external INT0 and INT1) is not possible. 

The good thing about Arduino is that the provided APIs are very user friendly. For example, to use serial port you only need to write Serial.init(baud_rate) and then you can start printing stuffs, without dealing with annoying registers. Therefore, the purpose this library is to provide some easy-to-use functions similar with those provided with Arduino (just similar, they are definitely not compatible), but when things go too complex you can always switch back to the most original way of programming hardwares. 

What Arduino also provides is the ability to program the chip without any extra hardware. As the ATMega328 itself doesn't come with any bootloader, it's not possible to do that without a programmer. However, it is possible to first download a bootloader to the microcontroller with the capability of receiving data from USB and programming itself using the data. The ATMega328 doesn't have a USB hardware interface, but thanks to V-USB (http://www.obdev.at/products/vusb/index.html) library a software-simulated USB 1.1 protocal can be easily implemented on any AVR hardware with external interrupts and two IO pins. 

This library provides a self-programming bootloader that is compatible with USBasp (http://www.fischl.de/usbasp/). The USBasp project was originally designed for programming other microcontrollers (AT89x51, AVR tiny and mega), but with some adaptions it can also be used in a bootloader to program itself. When a chip is loaded with the bootloader, you can use any USBasp software to program the chip itself. The software on your PC won't know the difference from programming other chips or itself.

This is a project that is still under active development. Please mail caoyuan@mit.edu for any suggestions or bug reports.

