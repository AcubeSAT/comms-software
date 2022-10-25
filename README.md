## conan

To build, you first have to cd `cmake-build-debug` (or to the respective CMake build dir) and run `conan install ..
--build=missing`. If you're using CLion and don't see `cmake-build-debug`, you have to `Reload CMake project` to have it generated.
After you've run `conan install ..` you can `Reload CMake project` and build as per usual.

<details>
<summary>Getting conan</summary>

You can install [conan](https://conan.io/) following the instructions from
[here](https://docs.conan.io/en/latest/installation.html). Just to be safe, you can follow the standard procedure from
[here](https://docs.conan.io/en/latest/getting_started.html):

- `conan profile new default --detect`: Generates default profile detecting GCC and sets old ABI. This step and the
  one below is needed to [manage the GCC >=5 ABI](https://docs.conan.io/en/latest/howtos/manage_gcc_abi.html#manage-gcc-abi)
- `conan profile update settings.compiler.libcxx=libstdc++11 default`: Sets `libcxx` to C++11 ABI

</details>