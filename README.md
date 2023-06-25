# COMMS Software

This repository contains the code that will run on the STM32H743 Microcontroller Unit, which is a
32-bit ARM Cortex-M7 core, on the AcubeSAT's
COMMS Board. The business logic is orchestrated by FreeRTOS tasks on the MCU.
You can check the instructions here on how to set up the toolchain to run the repository's code
### Software to be implemented
Business Logic regarding:
  - the board's [component drivers](https://gitlab.com/acubesat/comms/software/component-drivers) (transceiver, sensors, memory)
  - the Power Distribution Network and the RF Frontend
  - the [CCSDS Data Layer](https://gitlab.com/acubesat/comms/software/ccsds-telemetry-packets) and [Physical Layer](https://gitlab.com/acubesat/comms/software/physical_layer)
  - CAN inter-communication
  - ECSS Services used by COMMS

This project also relies on the [Cross Platform Software](https://gitlab.com/acubesat/obc/cross-platform-software)
repository, created to accomodate the common functionality for all of the satellite's platforms. Its
full integration with COMMS Software is still a work in progress

### Build

After cloning the repo, run the command `conan source .` to clone the needed repositories, which currently are:
- [cross-platform-software](https://gitlab.com/acubesat/obc/cross-platform-software) (at `comms-compatibility` branch)
- [component-drivers](https://gitlab.com/acubesat/comms/software/component-drivers)

If you're using CLion, you need to add in CMake options (File -> Settings -> Build, Execution, Deployment -> CMake ->
CMake Options) this `-DCMAKE_TOOLCHAIN_FILE=cmake-build-debug/build/Debug/generators/conan_toolchain.cmake -DCMAKE_CXX_COMPILER="/usr/bin/arm-none-eabi-g++" -DCMAKE_C_COMPILER="/usr/bin/arm-none-eabi-gcc"`.

If you just cmake from CLI, just add the same flags in your command.
### Conan

This repository uses [conan 2.0](https://conan.io/) to manage dependencies.

#### AcubeSAT Conan Packages
Some of the Conan packages are hosted on a private repository, so you
need to:
- have access to the [repository](https://artifactory.spacedot.gr) (if you're already on GitLab, it's the same
  credentials, and you should login at least once) and add the
  remote to your conan remotes. To do that run the following two commands
  `conan remote add conan https://artifactory.spacedot.gr/artifactory/api/conan/conan` and
  `conan remote login conan $YOUR_USERNAME`, which will prompt you to add your password.
- or, clone the repo on your own, and package it locally use `conan create . --build=missing` in the root of the repo.
- or, clone the repo on your own and add it as a submodule in the `lib` folder, and make the necessary CMakeLists.
  txt changes to include it in the build.


  To build, you need to follow these steps:
- First run `conan profile detect --force`: Generates default profile detecting GCC. However, for this project, you need to set up
  the correct architecture. Find where `conan` sets up profiles (probably `~/.conan2/profiles`) and run `cp conan-arm-profile ~/.conan2/profiles` (or another directory if conan2 stores the profiles elsewhere) in this project's folder.
- Then run `conan install . --output-folder=cmake-build-debug --build="*" -u -pr conan-arm-profile`. If you're using CLion and don't see `cmake-build-debug`, you have to `Reload CMake project` to have it generated.
  After you've run `conan install...` you can `Reload CMake project` and build as per usual.
- Make sure you followed the steps under the `Build` section
- If the *Imported target "common" included non-existent path* appears, just delete the `cmake-build-debug` folder and redo the `conan install...` command

<details>
<summary>Getting conan</summary>

You can install [conan](https://conan.io/) following the instructions from
[here](https://docs.conan.io/2/installation.html).
</details>
