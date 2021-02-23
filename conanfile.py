from conans import ConanFile, CMake


class CubicInterpolationConan(ConanFile):
    name = "CubicInterpolation"
    version = "0.1"
    license = "MIT"
    author = "Maximilian Sackel mail@maxsac.de"
    url = "https://github.com/MaxSac/cubic_interpolation"
    description = "Leightweight interpolation library based on boost and eigen."
    topics = ("interpolation", "splines", "cubic", "bicubic", "boost", "eigen3")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake_find_package"
    exports_sources = "*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
        if self.settings.compiler.libcxx == "libstdc++":
            raise Exception(
                "This package is only compatible with libstdc++11. Maybe this issue"
                "can be solved by adding '-s complier.libcxx=libstdc++11'."
            )

    def requirements(self):
        self.requires("boost/1.75.0")
        self.requires("eigen/3.3.9")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("license*", dst="licenses", ignore_case=True, keep_path=False)
        self.copy("*.h", dst="include", src="src")
        self.copy("*.hpp", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["CubicInterpolation"]
