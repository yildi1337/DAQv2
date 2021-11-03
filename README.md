# DAQv2
A Dual-Channel, 24-Bit, 256 kS/s Real-Time Hi-Speed USB 2.0 Data Acquisition (DAQ) Device based on STM32H7

# Details
The recording of analog measurement signals is often done with so-called data acquisition (DAQ) devices. Commercial devices from [National Instruments](http://www.ni.com) are commonly used, but unfortunately also very expensive. The goal of this project was to develop a first prototype of a low-cost DAQ device that can compete with commercial devices, i.e. both the hardware DAQ board as well as software for receiving and analyzing the data is necessary.

The basic structure of a DAQ is very simple. In principle, only an analog-to-digital converter (ADC) and an FPGA or microcontroller are needed in order to send the measurement data in real time to a computer. Here, the [AD7768-4 from Analog Devices](https://www.analog.com/en/products/ad7768-4.html) has been chosen, which offers up to 4 synchronous channels (but I only use 2 channels), a sampling rate of 256 kS/s and a resolution of 24 bit. Each sample consists of 3 data bytes and one status byte. So with the two channels used, the net data rate is 2 x 256 kS/s x 4 byte = 2.048 MByte/s = 16.384 MBit/s.

To keep the hardware cheap and the compatibility high, I decided to use a microcontroller (and not an FPGA) and a USB connection. Since the net data rate is higher than 12 MBit/s (USB Full-Speed), a USB Hi-Speed connection (up to 480 MBit/s) is required. However, most microcontrollers allow USB Hi-Speed connections only with an additional external PHY. Therefore, in my recent project [DAQv1](https://github.com/yildi1337/DAQv1), I have chosen a microcontroller from Microchip's PIC32MZ family, since they already have the Hi-Speed PHY integrated. Due to the poor documentation of the Microchip microcontrollers, I decided to use a microcontroller from the STM32H7 family again, which can be easily operated with an external PHY.

Technical summary of the developed platform:
* ADC: [AD7768-4BSTZ](https://www.analog.com/en/products/ad7768-4.html)
* Number of channels: 2
* Sample rate: 256 kS/s
* Resolution: 24 bit
* Reference voltage: 5 V
* Microcontroller: [STM32H753ZIT6](https://www.st.com/en/microcontrollers-microprocessors/stm32h753zi.html)
* USB 2.0 Hi-Speed (net data rate: 16.384 MBit/s)
* Preamlifier: 0-80 dB by means of two [AD8338ACPZ-R7](https://www.analog.com/en/products/ad8338.html)
* Programmable input coupling (AC with lower cutoff frequency of approx. 10 mHz and DC)
* Programmable input type (single ended and differential)
* LC Display for displaying input configuration
* Two buttons for chaning the input configuration
* Noise: See below

If you have any questions, feel free to contact me!

# Pictures

Not yet available.

# 3D Model

A 3D model of the PCB can be found in the Autodesk Fusion Cloud where it can be downloaded in various file types.

<p align="center">
    <a href="https://a360.co/31t7ZPF" target="_blank"><img src="https://github.com/yildi1337/DAQv2/blob/main/pictures/daq_v2_stm32h7_autodesk_fusion.png" /></a>
</p>

In addition, a step file of the complete device, i.e. a model of the PCB embedded into the housing ([TEKO 935.9](https://www.teko.it/de/produkte/produktfamilie/FP/serie/euro-93)), can be found in the directory *3d_model*.

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/3d_model/inventor_model_open_1.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/3d_model/inventor_model_open_2.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/3d_model/inventor_model_closed_1.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/3d_model/inventor_model_closed_2.png" />
</p>

# Schematics
Please see subdirectory *schematics*.

# Board
The PCB was designed in Autodesk EAGLE 9.6.2 and manufactured by [JLCPCB](https://jlcpcb.com/). All Gerber files can be found in the subdirectory *gerber_files*.

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/pictures/pcb_raw_top.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/pictures/pcb_raw_bottom.png" />
</p>

# Firmware
The source code of software running on the STM32H753ZIT6 can be found in the subdirectory *firmware*). For the development and debugging I used STM32CubeIDE version 1.7.0. For programming I used the ST-LINK/V2 in-circuit debugger/programmer.

# USB Debug Interface

A tool for the graphical display of status information and for controlling the DAQ v2 hardware via the virtual serial interface (second USB-B port) has been developed in Python, see subdirectory *debug_interface*.

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/debug_interface/screenshot/screenshot_usb_debug_interface.png" />
</p>

# LabVIEW Real-Time Data Receiver

A software vor real-time data reception and visualization of the signals both in time and frequency domain has been developed in LabVIEW, see subdirectory *labview*.

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/labview/screenshots/1.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/labview/screenshots/2_signal.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/labview/screenshots/2_noise.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/labview/screenshots/3.png" />
</p>

<p align="center">
  <img src="https://github.com/yildi1337/DAQv2/blob/main/labview/screenshots/4.png" />
</p>

# Noise

The voltage noise densities were measured for various cases (open inputs, shortened inputs, and 50 Ohm resistors connected to the inputs). 

Not yet available.
