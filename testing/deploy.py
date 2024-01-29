import os
import sys
import shutil
import subprocess
from enum import IntEnum


class Flags(IntEnum):
    BUILD = 0x1
    RUN = 0x2
    CONTROLLER = 0x4
    KBM = 0x8


def print_usage() -> None:
    print(
        "usage:\n"
        "\tpython deploy.py [OPTIONS--REQUIRED]\n"
        "options:\n"
        "\tb\t\texecute scons build\n"
        "\tr\t\texecute scons run\n"
        "\tc\t\tdeploy controller code\n"
        "\tk\t\tdeploy keyboard/mouse code\n"
        "example:\n"
        "\tpython deploy.py brc\n"
        "\t  - replaces the project src contents with the controller code\n"
        "\t  - attempts to build the project code\n"
        "\t  - attempts to inject the built code onto the MCB"
    )


def validate_command(argc: int, argv: list[str]) -> list[chr]:
    if argc < 2:
        print("deploy: error: no options provided", file=sys.stderr)
        return None

    if argv[1] == "--help":
        print_usage()
        os._exit(0)

    ops = ''.join(argv[1:])
    return ops


def execute_procs(base_dir: str, dest_dir: str, flags: int) -> bool:
    # removes and replace project src contents with chosen sources
    device = flags & (Flags.CONTROLLER | Flags.KBM)
    if device:
        if device == Flags.CONTROLLER | Flags.KBM:
            print(
                "deploy: error: cannot source both 'controller' and 'kbm'",
                file=sys.stderr
            )
            return False

        src_dir = base_dir
        if flags & Flags.CONTROLLER:
            src_dir += "/controller"
        elif flags & Flags.KBM:
            src_dir += "/kbm"

        shutil.rmtree(dest_dir)
        shutil.copytree(src_dir, dest_dir)

    # build and/or run project src code
    scons_dir = os.getcwd() + "/aruw-edu/aruw-edu-project"
    if flags & Flags.BUILD:
        subprocess.run(args=["scons", "build"], cwd=scons_dir)

    if flags & Flags.RUN:
        subprocess.run(args=["scons", "run"], cwd=scons_dir)

    return True


def process_ops(options: list[chr]) -> bool:
    flags = 0
    for op in options:
        match op:
            case 'b':
                flags |= Flags.BUILD
            case 'r':
                flags |= Flags.RUN
            case 'c':
                flags |= Flags.CONTROLLER
            case 'k':
                flags |= Flags.KBM
            case _:
                print(f"deploy: error: invalid option ({op})")
                return False

    # remove and replace project src folder with chosen sources
    base_dir = os.getcwd()[:-7]
    dest_dir = os.getcwd() + "/aruw-edu/aruw-edu-project/src"
    return execute_procs(
        base_dir,
        dest_dir,
        flags
    )


if __name__ == '__main__':
    options = validate_command(len(sys.argv), sys.argv)
    if options is None:
        print(
            "see \'--help\' for more information...",
            file=sys.stderr
        )
        os._exit(1)

    valid = process_ops(options)
    if not valid:
        print(
            "see \'--help\' for more information...",
            file=sys.stderr
        )
        os._exit(1)

    os._exit(0)
