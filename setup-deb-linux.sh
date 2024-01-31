#!/bin/bash

if [ ! -d "${HOME}/.qkrt" ]; then
    echo "[IMPORTANT] making '~/.qkrt' directory"
    mkdir ~/.qkrt
else echo "[INFO] '~/.qkrt' directory already exists"
fi

if ! command -v git &> /dev/null; then
    echo "[IMPORTANT] installing 'git' formula"
    sudo apt-get install git -y -qq >/dev/null
else echo "[INFO] 'git' is already installed"
fi
git submodule update --init --recursive

# install general tools
if [ $(dpkg-query -W -f='${Status}' openocd 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "[IMPORTANT] installing 'openocd'"
    sudo apt-get install openocd -y -qq >/dev/null
else echo "[INFO] 'openocd' is already installed"
fi
if [ $(dpkg-query -W -f='${Status}' gcc 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "[IMPORTANT] installing 'gcc'"
    sudo apt-get install gcc -y -qq >/dev/null
else echo "[INFO] 'gcc' is already installed"
fi
if [ $(dpkg-query -W -f='${Status}' build-essential 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "[IMPORTANT] installing 'build-essential'"
    sudo apt-get install build-essential -y -qq >/dev/null
else echo "[INFO] 'build-essential' is already installed"
fi
if [ $(dpkg-query -W -f='${Status}' libboost-all-dev 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "[IMPORTANT] installing 'libboost-all-dev'"
    sudo apt-get install libboost-all-dev -y -qq >/dev/null
else echo "[INFO] 'libboost-all-dev' is already installed"
fi
if [ $(dpkg-query -W -f='${Status}' doxygen 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "[IMPORTANT] installing 'doxygen'"
    sudo apt-get --no-install-recommends install doxygen -y -qq >/dev/null
else echo "[INFO] 'doxygen' is already installed"
fi

# install python tools
if ! command -v python3 &> /dev/null; then
    echo "[IMPORTANT] installing 'python3'"
    sudo apt-get install python3 -y -qq >/dev/null
else echo "[INFO] 'python3' is already installed"
fi
if ! command -v pip3 &> /dev/null; then
    echo "[IMPORTANT] installing 'pip3'"
    sudo apt-get install python3-pip -y -qq >/dev/null
else echo "[INFO] 'pip3' is already installed"
fi
if [ $(pip3 list | grep -c pipenv) -eq 0 ]; then
    echo -e "  [IMPORTANT] installing 'pipenv' python package"
    pip3 install pipenv --quiet
else echo -e "  [INFO] 'pipenv' python package is already installed"
fi
if [ $(pip3 list | grep -c SCons) -eq 0 ]; then
    echo -e "  [IMPORTANT] installing 'SCons' python package (version 4.1.1)"
    pip3 install -U scons==4.1 --quiet
elif [ $(pip list | grep SCons | awk 'NF>1{print $NF}') != "4.1.0" ]; then
    echo -e "  [IMPORTANT] uninstalling incorrect version of 'SCons'"
    pip3 uninstall -y scons --quiet
    echo -e "  [IMPORTANT] installing 'SCons' python package (version 4.1.0)"
    pip3 install -U scons==4.1 --quiet
else echo -e "  [INFO] 'SCons' python package is already installed"
fi
if [ $(pip3 list | grep -c pyelftools) -eq 0 ];  then
    echo -e "  [IMPORTANT] installing 'pyelftools' python package"
    pip3 install pyelftools --quiet
else echo -e "  [INFO] 'pyelftools' python package is already installed"
fi
if [ $(pip3 list | grep -c Jinja2) -eq 0 ]; then
    echo -e "  [IMPORTANT] installing 'Jinja2' python package"
    pip3 install Jinja2 --quiet
else echo -e "  [INFO] 'Jinja2' python package already installed"
fi

# install the 'arm-non-eabi' toochain
if [ ! -d "${HOME}/.qkrt/gcc-arm-none-eabi-10.3-2021.10" ]; then
    echo "[IMPORTANT] downloading 'gcc-arm-none-eabi-10.3-2021.10' binaries"
    curl -sL 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2?rev=78196d3461ba4c9089a67b5f33edf82a&hash=5631ACEF1F8F237389F14B41566964EC' | tar xjf - -C ~/.qkrt
else echo "[INFO] 'gcc-arm-none-eabi-10.3-2021.10' already downloaded"
fi
appended=false
required="export PATH=\$HOME/.qkrt/gcc-arm-none-eabi-10.3-2021.10/bin/:\$PATH"
while IFS= read -r line; do
    if [[ "${line}" == "${required}" ]]; then
        appended=true
        break
    fi
done < "${HOME}/.bashrc"
if [ "${appended}" = false ]; then
    echo "[IMPORTANT] adding 'gcc-arm-none-eabi-10.3-2021.10' binaries to \$PATH"
    echo -en "${required}\n" >> ~/.bashrc
    source ~/.bashrc
else echo "[INFO] 'gcc-arm-none-eabi-10.3-2021.10' binaries already added to \$PATH"
fi

