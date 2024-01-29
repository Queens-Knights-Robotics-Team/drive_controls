# script usage:
#   python deploy.py [option--REQUIRED]
#
# options:
#   -c      deploys the controller code
#   -k      deploys the keyboard/mouse code


import os
import sys
import shutil
import subprocess


def print_usage() -> None:
    print(
        "usage:\n"
        "\tpython deploy.py [SINGLE OPTION--REQUIRED]\n"
        "options:\n"
        "\t-c       deploy controller code\n"
        "\t-k       deploy keyboard/mouse code\n"
        "example:\n"
        "\tpython deploy.py -c"
    )


def replace_sources(src_dir: str, dest_dir: str) -> None:
    shutil.rmtree(dest_dir)
    shutil.copytree(src_dir, dest_dir)


if __name__ == '__main__':
    # validate required option
    option = ''.join(sys.argv[1:])
    if len(option) != 2 or option[0] != '-':
        print_usage()
        os._exit(1)

    # remove and replace project src folder with chosen sources
    cwd = os.getcwd()
    src_dir = cwd + "/aruw-edu/aruw-edu-project/src"
    match option[1]:
        case 'c':
            replace_sources(
                cwd[:-7] + "/controller",
                src_dir
            )
        case 'k':
            replace_sources(
                cwd[:-7] + "/kbm",
                src_dir
            )

    # build and run the project
    scons_dir = cwd + "/aruw-edu/aruw-edu-project"
    subprocess.run(args=["scons", "build"], cwd=scons_dir)
    subprocess.run(args=["scons", "run"], cwd=scons_dir)

    os._exit(0)
