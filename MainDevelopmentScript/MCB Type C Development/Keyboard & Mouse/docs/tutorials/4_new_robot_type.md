# Adding a New Robot Type Tutorial

## Recommended Reading

I recommend reading up on [preprocessor
macros](https://gcc.gnu.org/onlinedocs/cpp/Macros.html) before going through
this tutorial. I also recommend reading the taproot wiki page on [build targets
overview](https://gitlab.com/aruw/controls/taproot/-/wikis/Build-Targets-Overview).
Also make sure you know how [including files in
C++](https://www.geeksforgeeks.org/c-c-include-directive-with-examples/) works.

## Overview

In this tutorial, you will be adding a new robot type. This tutorial will guide
you through all steps that are required to add a new robot type. The way
[aruw-mcb](https://gitlab.com/aruw/controls/aruw-mcb) works (our main
robot-specific repository) is that we define all robot types and compile
software individually for each robot.

In particular, we define a
[macro](https://gcc.gnu.org/onlinedocs/cpp/Macros.html) based on which robot we
want to compile software for. If we want to compile software for the standard
robot, we define the macro `TARGET_STANDARD` during compilation. This way, code
inside of pre-processor conditional blocks are compiled like this:

```cpp
#if defined(TARGET_STANDARD)
initializeStandard();
#endif
```

In this way, we can conditionally run code based on which robot it is being
compiled for.

It happens that there are actually a limited number of places that must be
changed that are specific to each robot. In general, all subsystems and commands
are designed to be generic to any robot. We strive to avoid adding preprocessor
directives to define constants for each robot type in a subsystem or command.

Instead, follow the below sections to add a new robot type of your choosing to
`aruw-edu`.

## Getting Started

Create a new branch off of where you have been working on the other tutorials
for this tutorial.

## Modifying Build System

Firstly, you must modify the build system to accept a new robot type. The build
system used for our projects is [SCons](https://scons.org/), which consists of
`SConstruct` files, `SConscript` files, and python scripts. Instead of typing
out gcc compiler commands for each file that must be compiled each time a file
is modified, the build system performs these build commands for us. Our job is
to then configure the build environment, which includes adding build flags
(options used to configure the build in a particular way). One such build flag
we add is the robot type preprocessor, which has the form `TARGET_<robot>`,
where `<robot>` is the name of the robot in all upper case.

To add a new robot type to the build, open
`aruw-edu-project/build_tools/extract_robot_type.py` and add a robot type string
of the form `TARGET_<robot>` to the list of `VALID_ROBOT_TYPES` (again, replace
`<robot>` with an actual name in all upper case). You must now use the name
`TARGET_<robot>` everywhere for the rest of the tutorial. From now on, this name
will be referred to as the new robot type macro.

The last thing that needs to be done is modifying
`aruw-edu-project/build_tools/parse_args.py`. Here, modify the `USAGE` string so
it lists the new robot type you added to the build system as a valid argument.

## Modifying GitLab CI

Next, you should modify the [GitLab CI](https://docs.gitlab.com/ee/ci/). CI is
used to ensure software for each robot compiles and all unit tests pass. Each
build job will build the software using a different robot type preprocessor for
the hardware, sim, and test targets.

```{figure} ../uploads/ci_checks_template_project.png
:width: 700px
:align: center

An example CI visualization in GitLab for the
[taproot-template-project](https://gitlab.com/aruw/controls/taproot-template-project).
Each robot has a unique job for building for hardware, sim, and tests.
```

Thus, when you modify code for say the standard robot, CI will make sure you
don't break the other robots' builds.

### Adding CI jobs

Add the following jobs to the gitlab CI:

```
"<robot>: build for hardware"
"<robot>: build for sim"
"<robot>: build for tests"
"<robot>: run tests"
```

Where `<robot>` is the name of your robot. Use the `standard` CI jobs as an
example for what your GitLab jobs should look like.

## Adding `<robot>.cpp` and `<robot>.hpp`

Each robot has a class named `Robot` that contains instance of all subsystems
and commands specific to the robot. For example, a standard might have chassis,
turret, and launcher subsystems, while an engineer might have an arm subsystem.
Your job next will be to add a `Robot` class for the new robot you are creating.

Add files named `<robot>.cpp` and `<robot>.hpp` to
`aruw-edu-project/src/control` (where `<robot>` is the actual name of your
robot). Copy over `aruw-edu-project/src/control/standard.hpp/cpp` into the files
you created, removing any subsystems/commands/command mappings that likely won't
be relevant to your new robot type.

```{attention}
Currently, aruw-mcb handles instantiation and initialization of robot subsystems 
nd commands differently from `aruw-edu`. A style more similar to `aruw-edu`
should be deployed in the future; however, currently aruw-mcb sports an old but
similar enough style for instantiating all robot-specific subsystems and
commands.

The main difference is that instead of having a `Robot` object instantiate
subsystems and commands, all subsystems and commands are instantiated directly
in a source file specific to the robot.
```

## Modifying `robot.hpp`

Now that we have created a `Robot` class for your robot, we must allow
`aruw-edu-project/src/main.cpp` the ability to access it. We do this by
modifying `robot.hpp`. As shown below, `robot.hpp` contains the following
conditional preprocessor directive:

```cpp
#if defined(TARGET_STANDARD)
#include "standard.hpp"
#else
#error "standard only robot supported"
#endif
```

This conditional in effect says that if the standard robot is defined, use the
`Robot` object defined in `standard.hpp`. Add an `elif defined()` case that
includes `<robot.hpp>` so that if the macro for your robot is defined, the
`Robot` object for your robot will be used.

## Testing

To make sure you have set up CI correctly and that your new robot project
builds, commit and push your code and create a merge request from the branch you
have been working on. You should see all your new CI jobs pass.

## Conclusion

While you have added a new robot type, it currently probably doesn't do much. To
make your robot do more interesting things, you might have to make more
subsystems and commands, then you will have to instantiate subsystems and
commands that your robot requires in the `Robot` object specific to your robot
(like you did for the tank drive and agitator control tutorials).
