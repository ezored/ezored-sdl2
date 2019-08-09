import os

def run(proj_path, target_name, params):
    archs = [
            {
                'arch': 'x86',
                'conan_arch': 'x86',
                'conan_profile': 'ezored_linux_app_profile',
            },
            {
                'arch': 'x86_64',
                'conan_arch': 'x86_64',
                'conan_profile': 'ezored_linux_app_profile',
            },
        ]
    if os.getenv("CONAN_ARCHS"):
        archs = []
        env_archs = os.getenv("CONAN_ARCHS").split(",")
        for it in env_archs:
            archs.append({
                'arch': it,
                'conan_arch': it,
                'conan_profile': 'ezored_linux_app_profile',
            })

    return {
        'project_name': 'Sample',
        'build_types': ['Debug', 'Release'],
        'archs': archs,
        'assets_dir': 'projects/samples/sdl2/assets',
    }
