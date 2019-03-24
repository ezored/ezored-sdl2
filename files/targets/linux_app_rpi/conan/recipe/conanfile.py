from conans import ConanFile, CMake


class TargetConan(ConanFile):
    name = 'linux_app_rpi'
    version = '1.0.0'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {
        'shared': [True, False],
        'fPIC': [True, False],
    }
    default_options = {
        'shared': False,
        'fPIC': True,
        'sdl2_image:bmp': False,
        'sdl2_image:gif': False,
        'sdl2_image:lbm': False,
        'sdl2_image:pcx': False,
        'sdl2_image:pnm': False,
        'sdl2_image:svg': False,
        'sdl2_image:tga': False,
        'sdl2_image:xcf': False,
        'sdl2_image:xpm': False,
        'sdl2_image:xv': False,
        'sdl2_image:jpg': 'libjpeg',
        'sdl2_image:tif': False,
        'sdl2_image:png': True,
        'sdl2_image:webp': False,
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
