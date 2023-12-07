import os
from os.path import join

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy
from conan.tools.scm import Git


class OBCSoftwareRecipe(ConanFile):
    name = "comms-sw"
    version = "1.0"
    revision_mode = "scm"

    # Optional metadata
    license = "MIT"
    author = "SpaceDot - AcubeSAT, acubesat.comms@spacedot.gr"
    url = "gitlab.com/acubesat/comms/software/comms_software"
    description = "Embedded software for the AcubeSAT nanosatellite's COMMS Board"
    topics = ("satellite", "acubesat", "comms", "comms-software")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": False, "ecss-services/*:ecss_config_file_path": os.path.abspath("Core/Inc/Platform/")}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "Core/Src/*", "Core/Inc/*", "Core/lib/*"
    generators = "CMakeDeps"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        git = Git(self)
        git.clone(url="git@gitlab.com:acubesat/obc/cross-platform-software.git", target=join(str(self.source_folder), "Core/lib/cross-platform-software"))
        self.run("cd Core/lib/cross-platform-software && git submodule update --init --recursive && git checkout comms-compatibility-update")
        git = Git(self)
        git.clone(url="git@gitlab.com:acubesat/comms/software/component-drivers.git", target=join(str(self.source_folder), "Core/lib/component-drivers"))
        git = Git(self)
        git.clone(url="git@gitlab.com:acubesat/comms/software/physical_layer.git", target=join(str(self.source_folder), "Core/lib/physical_layer"))
        git = Git(self)
        git.clone(url="git@gitlab.com:acubesat/comms/software/ccsds-telemetry-packets.git", target=join(str(self.source_folder), "Core/lib/ccsds-telemetry-packets"))

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.preprocessor_definitions["DEBUG"] = None
        tc.preprocessor_definitions["USE_HAL_DRIVER"] = None
        tc.preprocessor_definitions["STM32H7A3xxQ"] = None
        tc.preprocessor_definitions["STM32"] = None
        tc.preprocessor_definitions["LOGLEVEL_TRACE"] = None
        tc.variables["NO_SYSTEM_INCLUDE"] = True
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def requirements(self):
        self.requires("etl/20.37.2")
        self.requires("logger/1.0")
        self.requires("ecss-services/1.0")
