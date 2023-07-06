all: build

BUILD_TYPE := Release
PYTHON_VERSION := 3.8.12
VENV_NAME := venv38
ENABLE_CONAN := ON
ENABLE_PYTHON := OFF

.PHONY: configure
configure:
	cmake -S . -B ./build -DENABLE_CONAN:BOOL=${ENABLE_CONAN} -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CXX_FLAGS}" \
	-DENABLE_PYTHON:BOOL=${ENABLE_PYTHON}

.PHONY: build
build: configure
	cmake --build ./build --config ${BUILD_TYPE}

.PHONY: build-python
build-python: configure pyenv-init

.PHONY: build-debug
build-debug:
	mkdir -p build-debug; cd build-debug; cmake -DCMAKE_BUILD_TYPE=Debug ..; cmake --build .

.PHONY: clean
clean:
	make -C build clean
