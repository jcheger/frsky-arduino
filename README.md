Frsky D (aka FrskyD) and Frsky Smart Port (aka FrskySP) libraries for Arduino.

Those two libraries were designed to sniff and build sensors for Frsky. They still are in early stage, but good enough to have fun with.

# Installation
* got to your sketchbook/libraries folder
* download the library [FrskD.zip] (https://github.com/jcheger/frsky-arduino/blob/master/FrskyD.zip?raw=true) or [FrskySP.zip] (https://github.com/jcheger/frsky-arduino/blob/master/FrskySP.zip?raw=true)
* unzip it
* restart the Arduino IDE

# Documentation
Each library is documented. Look at doc/html/index.html in each one of them. Here is an online copy:
* [FrskyD documentation] (https://www.ordinoscope.net/static/frsky-arduino/FrskyD/doc/html/index.html)
* [FrskySP documentation] (https://www.ordinoscope.net/static/frsky-arduino/FrskySP/doc/html/index.html)

# Some examples
Look at the examples files to see how it works
* Protocol D
 * [Demo - simulate sensors] (https://github.com/jcheger/frsky-arduino/blob/master/FrskyD/examples/FrskyD_sensor_demo/FrskyD_sensor_demo.ino)
 * [Packet sniffer] (https://github.com/jcheger/frsky-arduino/blob/master/FrskyD/examples/FrskyD_sniffer/FrskyD_sniffer.ino)

* Smart Port
 * [Demo - simulate sensors] (https://github.com/jcheger/frsky-arduino/blob/master/FrskySP/examples/FrskySP_sensor_demo/FrskySP_sensor_demo.ino)
 * [RPM sensor (using EagleTree or other sensors)] (https://github.com/jcheger/frsky-arduino/blob/master/FrskySP/examples/FrskySP_rpm_sensor_freqcount/FrskySP_rpm_sensor_freqcount.ino) - [pinout draft] (https://github.com/jcheger/frsky-arduino/blob/master/FrskySP/examples/FrskySP_rpm_sensor_freqcount/EagleTree_RPM_sensors_bb.png)
