from cmaketools import setup
import os

install_requires = []

def get_cmake_args():
    args = os.getenv('CMAKE_ARGS', "")
    return args.split(' ')

setup(
    name="pycppsas7bdat",
    version=1,
    author="Olivia Quinet",
    author_email="olivia.quinet@gmail.com",
    description="A package to read SAS7BDAT file with C++17 and boost::python",
    url="https://github.com/olivia76/cpp-sas7bdat",
    license="Apache License 2.0",
    src_dir="python",
    has_package_data=True,
    install_requires=install_requires,
    configure_opts = ['-DENABLE_PYTHON:BOOL=ON', 
                      '-DENABLE_TESTING:BOOL=OFF', 
                      '-DENABLE_CONAN:BOOL=ON'] + get_cmake_args()
)
