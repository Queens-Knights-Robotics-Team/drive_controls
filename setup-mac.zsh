#!/bin/zsh

# install homebrew
if ! command -v brew &> /dev/null
then
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    exit 1
fi

# install python tools
brew install python3 git
pip3 install -U scons==4.1
pip3 install pyelftools
pip3 install pipenv
git submodule update --init --recursive
cd testing/aruw-edu/aruw-edu-project/

# install ARM (target device) toolchain and tools
brew tap osx-cross/arm
brew install arm-gcc-bin
brew install openocd --HEAD

# install host gcc compiler and boost lib for tests
brew install boost gcc@11 cmake

# install clang-format-10
if [ ! -d "~/.qkrt" ]
then
    mkdir ~/.qkrt
fi
curl -sL https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/clang+llvm-10.0.0-x86_64-apple-darwin.tar.xz | tar xvz - -C ~/.qkrt
echo -n 'export PATH=$HOME/.qkrt/clang+llvm-10.0.0-x86_64-apple-darwin/bin/:$PATH' >> ~/.zshrc
source ~/.zshrc

