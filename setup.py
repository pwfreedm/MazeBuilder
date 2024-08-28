from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "Maze",
        ["src/include/Maze_Bindings.cpp"],
    ),
]

setup(package_dir={"Maze": "include/Maze_Bindings.cpp"}, cmdclass={"build_ext": build_ext}, ext_modules=ext_modules)

