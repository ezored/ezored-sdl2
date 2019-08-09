#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
from cpt.packager import ConanMultiPackager


def docker_entry_script():
    return " ".join(["python make.py conan install_profiles &&",
                     "python make.py target linux_app conan &&",
                     "python make.py target linux_app build &&",
                     "python make.py target linux_app package"])

if __name__ == "__main__":
    builder = ConanMultiPackager(docker_entry_script=docker_entry_script())
    builder.add()
    builder.run()
