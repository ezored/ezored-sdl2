from conans import ConanFile, CMake


class TargetConan(ConanFile):
    name = 'windows_app'
    version = '1.0.0'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {
        'shared': [True, False],
        'fPIC': [True, False],
    }
    default_options = {
        'shared': False,
        'fPIC': True,
        'sdl2_image:jpg': 'libjpeg',
    }
    exports_sources = '*'
    generators = 'cmake'

    def build(self):
        cmake = CMake(self)
        cmake.definitions['CMAKE_BUILD_TYPE'] = self.settings.build_type
        cmake.definitions['PROJECT_CONFIG_ARCH'] = self.settings.arch
        cmake.configure()
        cmake.build()

    def requirements(self):
        self.requires('sdl2/2.0.9@bincrafters/stable')
        self.requires('sdl2_image/2.0.4@bincrafters/stable')
        self.requires('sdl2_ttf/2.0.15@bincrafters/stable')
