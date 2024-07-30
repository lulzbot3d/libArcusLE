from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import AutoPackager
from conan.tools.build import check_min_cppstd
from conan import ConanFile

required_conan_version = "<=1.56.0"


class ArcusLEConan(ConanFile):
    name = "arcusle"
    license = "LGPL-3.0"
    author = "Ultimaker B.V."
    url = "https://github.com/lulzbot3d/libArcusLE"
    description = "Fork of libArcus: A Communication library between internal components for Ultimaker software"
    topics = ("conan", "binding", "cura", "protobuf", "c++")
    settings = "os", "compiler", "build_type", "arch"
    revision_mode = "scm"
    exports = "LICENSE*"
    generators = "CMakeDeps", "CMakeToolchain", "VirtualBuildEnv", "VirtualRunEnv"

    python_requires = "lulzbase/[>=0.1.7]@lulzbot/stable"
    python_requires_extend = "lulzbase.LulzBaseConanfile"

    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": True,
        "fPIC": True,
    }
    scm = {
        "type": "git",
        "subfolder": ".",
        "url": "auto",
        "revision": "auto"
    }

    def set_version(self):
        if self.version is None:
            self.version = self._lulzdefault_version()

    def requirements(self):
        self.requires("standardprojectsettings/[>=0.1.0]@lulzbot/stable")
        for req in self._lulz_data()["requirements"]:
            self.requires(req)

    def config_options(self):
        if self.options.shared and self.settings.compiler == "Visual Studio":
            del self.options.fPIC

    def configure(self):
        self.options["protobuf"].shared = True

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 17)

    def layout(self):
        cmake_layout(self)
        self.cpp.package.libs = ["ArcusLE"]

        if self.settings.build_type == "Debug":
            self.cpp.package.defines = ["ARCUS_DEBUG"]
        if self.settings.os in ["Linux", "FreeBSD", "Macos"]:
            self.cpp.package.system_libs = ["pthread"]
        elif self.settings.os == "Windows":
            self.cpp.package.system_libs = ["ws2_32"]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        packager = AutoPackager(self)
        packager.run()
