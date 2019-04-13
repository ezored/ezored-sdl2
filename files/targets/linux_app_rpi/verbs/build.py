"""Build executable"""

import os

import ezored.app.const as const
from ezored.modules import file
from ezored.modules import log
from ezored.modules import runner
from ezored.modules import target
from ezored.modules import util


# -----------------------------------------------------------------------------
def run(params):
    proj_path = params['proj_path']
    target_name = params['target_name']
    target_config = target.get_target_config(proj_path, target_name)

    archs = target_config['archs']
    build_types = target_config['build_types']
    param_dry_run = util.list_has_key(params['args'], '--dry-run')

    if param_dry_run:
        log.info("Running in dry mode...")

    if archs and len(archs) > 0:
        for arch in archs:
            for build_type in build_types:
                log.info('Building for: {0}/{1}...'.format(
                    arch['conan_arch'],
                    build_type
                ))

                # conan build
                build_dir = os.path.join(
                    proj_path,
                    const.DIR_NAME_BUILD,
                    target_name,
                    build_type,
                    arch['conan_arch'],
                    const.DIR_NAME_BUILD_TARGET,
                )

                clean_build_dir = True

                if param_dry_run and os.path.isdir(build_dir):
                    clean_build_dir = False

                if clean_build_dir:
                    file.remove_dir(build_dir)
                    file.create_dir(build_dir)

                run_args = [
                    'conan',
                    'build',
                    os.path.join(
                        proj_path,
                        const.DIR_NAME_FILES,
                        const.DIR_NAME_FILES_TARGETS,
                        target_name,
                        const.DIR_NAME_FILES_TARGET_CONAN,
                        const.DIR_NAME_FILES_TARGET_CONAN_RECIPE,
                        const.FILE_NAME_FILES_TARGET_CONAN_RECIPE_CONANFILE_PY,
                    ),
                    '--source-folder',
                    os.path.join(
                        proj_path,
                        const.DIR_NAME_FILES,
                        const.DIR_NAME_FILES_TARGETS,
                        target_name,
                        const.DIR_NAME_FILES_TARGET_CMAKE,
                    ),
                    '--build-folder',
                    os.path.join(
                        proj_path,
                        const.DIR_NAME_BUILD,
                        target_name,
                        build_type,
                        arch['conan_arch'],
                        const.DIR_NAME_BUILD_TARGET,
                    ),
                    '--install-folder',
                    os.path.join(
                        proj_path,
                        const.DIR_NAME_BUILD,
                        target_name,
                        build_type,
                        arch['conan_arch'],
                        const.DIR_NAME_BUILD_CONAN,
                    ),
                ]

                runner.run(
                    run_args,
                    build_dir
                )

                # copy assets
                assets_dir = os.path.join(
                    proj_path,
                    const.DIR_NAME_PROJECTS,
                    'samples',
                    'basic',
                    'assets',
                )

                build_assets_dir = os.path.join(
                    build_dir,
                    'bin',
                    'assets',
                )

                file.remove_dir(build_assets_dir)
                file.copy_all_inside(assets_dir, build_assets_dir)
    else:
        log.error('Arch list for "{0}" is invalid or empty'.format(
            target_name
        ))
