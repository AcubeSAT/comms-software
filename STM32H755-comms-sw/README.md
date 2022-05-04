Development of the On-Board software of COMMS  
====

## Instructions

The software is written for the NUCLEO-H755ZI-Q board and contains work in progress.
The MCU is dual core, thus for setting up the project and the CmakeLists.txt files we have followed
the instructions that can be found in [this repository](https://github.com/elmot/h7-trace-probe/blob/master/README.md).
It also contains the instructions to run an  Embedded GDB Server run configuration for this project.

## Content

It is expected to contain software implementing the following:

* CCSDS Data link layer protocols
* CCSDS OQPSK modulation
* CCSDS GMSK modulation and demodulation
* CCSDS 1/2 Convolutional coding
* CCSDS BCH decoding
  