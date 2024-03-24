#!/bin/bash

# asks for password to run commands with elevated user privileges
if [ "$EUID" != 0 ]; then
    sudo "$0" "$@"
    exit $?
fi

home="$(getent passwd $SUDO_USER | cut -d: -f6)"

echo "[INFO] running 'sudo apt-get update'"
sudo apt-get update -qq &>/dev/null

echo "[INFO] updating git submodules recursively"
git submodule update --init --recursive &>/dev/null

if [ ! -d ${home}/.qkrt ]; then
    echo "[IMPORTANT] making '${home}/.qkrt' directory"
    mkdir ${home}/.qkrt
else echo "[INFO] '${home}/.qkrt' directory already exists"
fi

echo "packages:"

# python3
if ! command -v python3 &>/dev/null; then
    echo "  [IMPORTANT] installing 'python3'"
    sudo apt-get install python3 -y -qq >/dev/null
else echo "  [INFO] 'python3' is already installed"
fi
# check if python version > 3.8 ;
#     change strings to raw strings in all python scripts "" -> R""

# python3-pip
if ! command -v pip3 &>/dev/null; then
    echo "  [IMPORTANT] installing 'pip3'"
    sudo apt-get install python3-pip -y -qq >/dev/null
else echo "  [INFO] 'pip3' is already installed"
fi

# scons
if [ $(dpkg-query -W -f='${Status}' scons 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'scons'"
    sudo apt-get install scons -y -qq >/dev/null
else echo "  [INFO] 'scons' is already installed"
fi

# git
if ! command -v git &>/dev/null; then
    echo "  [IMPORTANT] installing 'git' formula"
    sudo apt-get install git -y -qq >/dev/null
else echo "  [INFO] 'git' is already installed"
fi

# openocd
if [ $(dpkg-query -W -f='${Status}' openocd 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'openocd'"
    sudo apt-get install openocd -y -qq >/dev/null
else echo "  [INFO] 'openocd' is already installed"
fi

# gcc
if [ $(dpkg-query -W -f='${Status}' gcc 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'gcc'"
    sudo apt-get install gcc -y -qq >/dev/null
else echo "  [INFO] 'gcc' is already installed"
fi

# build-essential
if [ $(dpkg-query -W -f='${Status}' build-essential 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'build-essential'"
    sudo apt-get install build-essential -y -qq >/dev/null
else echo "  [INFO] 'build-essential' is already installed"
fi

# libboost-all-dev
if [ $(dpkg-query -W -f='${Status}' libboost-all-dev 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'libboost-all-dev'"
    sudo apt-get install libboost-all-dev -y -qq >/dev/null
else echo "  [INFO] 'libboost-all-dev' is already installed"
fi

# doxygen
if [ $(dpkg-query -W -f='${Status}' doxygen 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'doxygen'"
    sudo apt-get --no-install-recommends install doxygen -y -qq >/dev/null
else echo "  [INFO] 'doxygen' is already installed"
fi

# pkg-config -- needed for openocd bootstrap script
if [ $(dpkg-query -W -f='${Status}' pkg-config 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'libusb-1.0-0-dev'"
    sudo apt-get install pkg-config -y -qq >/dev/null
else echo "  [INFO] 'pkg-config' is already installed"
fi

# cmake -- needed for generating build files for ST-Link V2 driver
if ! command -v cmake &>/dev/null; then
    echo "  [IMPORTANT] installing 'cmake'"
    sudo apt-get install cmake -y -qq >/dev/null
else echo "  [INFO] 'cmake' is already installed"
fi

# libusb-1.0-0-dev -- needed for ST-Link V2 driver
if [ $(dpkg-query -W -f='${Status}' libusb-1.0-0-dev 2>/dev/null | grep -c "ok installed") -ne 1 ]; then
    echo "  [IMPORTANT] installing 'libusb-1.0-0-dev'"
    sudo apt-get install libusb-1.0-0-dev -y -qq >/dev/null
else echo "  [INFO] 'libusb-1.0-0-dev' is already installed"
fi

# 'arm-none-eabi' toolchain
echo "arm-none-eabi:"
if [ ! -d ${home}/.qkrt/gcc-arm-none-eabi-10.3-2021.10 ]; then
    echo "  [IMPORTANT] downloading 'gcc-arm-none-eabi-10.3-2021.10' binaries"
    curl -sL 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2?rev=78196d3461ba4c9089a67b5f33edf82a&hash=5631ACEF1F8F237389F14B41566964EC' | tar xjf - -C ${home}/.qkrt
else echo "  [INFO] 'gcc-arm-none-eabi-10.3-2021.10' already downloaded"
fi
appended=false
required="export PATH=\$HOME/.qkrt/gcc-arm-none-eabi-10.3-2021.10/bin/:\$PATH"
while IFS= read -r line; do
    if [[ "${line}" == "${required}" ]]; then
        appended=true
        break
    fi
done < ${home}/.bashrc
if [ "${appended}" = false ]; then
    echo "  [IMPORTANT] adding 'gcc-arm-none-eabi-10.3-2021.10' binaries to \$PATH"
    echo -en "${required}\n" >> ${home}/.bashrc
    source ${home}/.bashrc
else echo "  [INFO] 'gcc-arm-none-eabi-10.3-2021.10' binaries already added to \$PATH"
fi

# compile and install 'openocd' from repository
echo "openocd:"
if [ ! -d ${home}/.qkrt/openocd ]; then
    echo "  [IMPORTANT] cloning 'openocd' repository from github"
    git clone https://github.com/ntfreak/openocd.git ${home}/.qkrt/openocd
else echo "  [INFO] 'openocd' repository already cloned"
fi
if [ ! -d ${home}/.qkrt/openocd/build-aux ]; then
    echo "  [IMPORTANT] compiling and installing 'openocd' from source"
    ( cd ${home}/.qkrt/openocd ; ./bootstrap ; ./configure ; make ; sudo make install ) &>/dev/null
else echo "  [INFO] 'openocd' sources already compiled and installed"
fi

# compile and install 'ST-Link V2 driver' from repository
echo "stlink:"
if [ ! -d ${home}/.qkrt/stlink ]; then
    echo "  [IMPORTANT] cloning 'stlink' repository from github"
    git clone https://github.com/texane/stlink ${home}/.qkrt/stlink
else echo "  [INFO] 'stlink' repository already cloned"
fi
if [ ! -d ${home}/.qkrt/stlink/bin ]; then
    echo "  [IMPORTANT] compiling and installing 'stlink' from source"
    ( cd ${home}/.qkrt/stlink ; cmake . ; make ; sudo make install ) &>/dev/null
    ( cd ${home}/.qkrt/stlink/bin ; sudo cp st-* /usr/local/bin ) &>/dev/null
    ( cd ${home}/.qkrt/stlink ; sudo cp config/udev/rules.d/49-stlinkv* /etc/udev/rules.d/ ) &>/dev/null
    ( cd ${home}/.qkrt/stlink ; sudo udevadm control --reload ) &>/dev/null
else echo "  [INFO] 'stlink' sources already compiled and installed"
fi

# if [ $(pip3 list | grep -c pipenv) -eq 0 ]; then
#     echo -e "  [IMPORTANT] installing 'pipenv' python package"
#     pip3 install pipenv --quiet
# else echo -e "  [INFO] 'pipenv' python package is already installed"
# fi
# if [ $(pip3 list | grep -c SCons) -eq 0 ]; then
#     echo -e "  [IMPORTANT] installing 'SCons' python package (version 4.1.1)"
#     pip3 install -U scons==4.1 --quiet
# elif [ $(pip list | grep SCons | awk 'NF>1{print $NF}') != "4.1.0" ]; then
#     echo -e "  [IMPORTANT] uninstalling incorrect version of 'SCons'"
#     pip3 uninstall -y scons --quiet
#     echo -e "  [IMPORTANT] installing 'SCons' python package (version 4.1.0)"
#     pip3 install -U scons==4.1 --quiet
# else echo -e "  [INFO] 'SCons' python package is already installed"
# fi
# if [ $(pip3 list | grep -c pyelftools) -eq 0 ];  then
#     echo -e "  [IMPORTANT] installing 'pyelftools' python package"
#     pip3 install pyelftools --quiet
# else echo -e "  [INFO] 'pyelftools' python package is already installed"
# fi
# if [ $(pip3 list | grep -c Jinja2) -eq 0 ]; then
#     echo -e "  [IMPORTANT] installing 'Jinja2' python package"
#     pip3 install Jinja2 --quiet
# else echo -e "  [INFO] 'Jinja2' python package already installed"
# fi

printf "\ndone.\n"
