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
  
## Set up gdb-server

Quoting [these instructions](https://github.com/elmot/h7-trace-probe/blob/master/README.md): 
 1. Right-click `CMakeLists.txt`in the `Project` tool window and then click `Load CMake Project`
 1. Your CMake project will be parsed. If there are errors, correct them and select `Tools -> CMake -> Reset Cache and Reload Prpject` from the main menu.
 Repeat until everything is fixed and the CMakeLists is successfully parsed.
 1. Create `Run Configurations` for the CM7 core
    1. Select create new [Embedded GDB Server](https://www.jetbrains.com/help/clion/embedded-gdb-server.html) run configuration.
    1. Select `Target`. There shall be the only one.
    1. Select `Executable`. There shall be the only one.
    1. Set `tcp::<port number>` to `'target remote' args` field. 
    The port number may be virtually any in range 1024..65535 but it must not clash with the counterpart project port number.
    Use the same number below  
    1. Locate `ST-LINK_gdbserver` (use the `locate` command) executable and select for as `GDB server`
    The executable resides in the in `STM32CubeIDE` installation folder, 
    `plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.????/tools/bin/` subfolder. 
    Actual name varies from version too version, and exact name can't be provided
    1. Locate `STM32_Programmer_CLI` (use the `locate` command) executable similar way, but the folder expected to be 
    `plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.???/tools/bin/` then add 
    *path to the folder* as a `-cp` key value to `GDB Server args` field
    1. Add `-t -d -p <port number> -m <core num>` keys to `GDB Server args` field. 
       * The `<core num>` parameter is `0` for Cortex-M7.
       * Final arguments form is `-cp <STM32_Programmer_CLI bin folder path> -t -d -p <port number> -m <core num>`
 1. Click `Debug` for the CM7 project. The firmware for this kernel should be flashed and started.

 ### Example

 * target remote args: `tcp::2000`
 * GDB Server: `/opt/st/stm32cubeide_1.9.0/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.linux64_2.0.200.202202231230/tools/bin/ST-LINK_gdbserver`
* GDB Server args: `-cp /opt/st/stm32cubeide_1.9.0/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.linux64_2.0.200.202202231230/tools/bin/ -t -d -p 2000 -m 0`
