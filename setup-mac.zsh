#!/bin/zsh

if [ ! -d "${HOME}/.qkrt" ]; then
    mkdir ~/.qkrt
fi

git submodule update --init --recursive

if ! command -v brew &> /dev/null; then
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else; echo "[INFO] 'homebrew' is already installed"
fi

if ! command -v git &> /dev/null; then
    brew install git
else; echo "[INFO] 'git' is already installed"
fi

# install ARM (target device) toolchain and tools
brew tap osx-cross/arm
if ! brew ls --versions arm-gcc-bin &> /dev/null; then
    echo "[IMPORTANT] installing 'arm-gcc-bin' formula"
    brew install arm-gcc-bin --quiet
else; echo "[INFO] 'arm-gcc-bin' is already installed"
fi
if ! brew ls --versions openocd &> /dev/null; then
    echo "[IMPORTANT] installing 'openocd --HEAD' formula"
    brew install openocd --HEAD --quiet
else; echo "[INFO] 'openocd --HEAD' is already installed"
fi

# install host gcc compiler and boost lib for tests
if ! brew ls --versions boost &> /dev/null; then
    echo "[IMPORTANT] installing 'boost' formula"
    brew install boost --quiet
else; echo "[INFO] 'boost' is already installed"
fi
if ! brew ls --versions gcc@11 &> /dev/null; then
    echo "[IMPORTANT] installing 'gcc@11' formula"
    brew install gcc@11 --quiet
else; echo "[INFO] 'gcc@11' is already installed"
fi
if ! brew ls --versions cmake &> /dev/null; then
    echo "[IMPORTANT] installing 'cmake' formula"
    brew install cmake --quiet
else; echo "[INFO] 'cmake' is already installed"
fi

# install python tools
# TODO: create symlink for 'python' command to make scripts cross-platform
if ! command -v python3 &> /dev/null; then
    brew install python3
else; echo "[INFO] 'python' is already installed"
fi
if [ $(pip3 list | grep -c pipenv) -eq 0 ]; then
    echo "\t[IMPORTANT] installing 'pipenv' python package"
    pip3 install pipenv
else; echo "\t[INFO] 'pipenv' package already installed"
fi
if [ $(pip list | grep SCons | awk 'NF>1{print $NF}') != "4.1.0" ]; then
    if [ $(pip3 list | grep -c pyelftools) -ne 0 ];  then
        pip3 uninstall scons
    fi
    echo "\t[IMPORTANT] installing 'SCons' python package (version 4.1.0)"
    pip3 install -U scons==4.1 --quiet
else; echo "\t[INFO] 'SCons' package already installed"
fi
if [ $(pip3 list | grep -c pyelftools) -eq 0 ];  then
    echo "\t[IMPORTANT] installing 'pyelftools' python package"
    pip3 install pyelftools --quiet
else; echo "\t[INFO] 'pyelftools' package already installed"
fi
if [ $(pip3 list | grep -c Jinja2) -eq 0 ]; then
    echo "\t[IMPORTANT] installing 'Jinja2' python package"
    pip3 install Jinja2 --quiet
else; echo "\t[INFO] 'Jinja2' package already installed"
fi

# install clang-format-10
if [ ! -d "${HOME}/.qkrt/clang+llvm-10.0.0-x86_64-apple-darwin" ]; then
    echo "[IMPORTANT] downloading 'clang+llvm-10.0.0' binaries"
    curl -sL https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/clang+llvm-10.0.0-x86_64-apple-darwin.tar.xz | tar xz - -C ~/.qkrt
else; echo "[INFO] 'clang+llvm-10.0.0' already downloaded"
fi
appended=false
required="export PATH=\$HOME/.qkrt/clang+llvm-10.0.0-x86_64-apple-darwin/bin/:\$PATH"
while IFS= read -r line; do
    if [[ "${line}" == "${required}" ]]; then
        appended=true
        break
    fi
done < "${HOME}/.zshrc"
if [ "${appended}" = false ]; then
    echo "[IMPORTANT] adding 'clang+llvm-10.0.0' binaries to \$PATH"
    echo -n "${required}\n" >> ~/.zshrc
    source ~/.zshrc
else; echo "[INFO] 'clang+llvm-10.0.0' binaries already added to \$PATH"
fi

