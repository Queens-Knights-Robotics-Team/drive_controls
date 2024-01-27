# Tank Drive Tutorial

## Table of Contents

```{toctree}
1_tank_drive
```

## Hardware

The chassis is a fundamental subsystem for many RoboMaster robots. Our 2022
standard chassis consists of the following hardware:

- Main microcontroller. The
  [microcontroller](https://en.wikipedia.org/wiki/Microcontroller) used on the
  standard is the [RoboMaster type A Board (aka
  MCB)](https://gitlab.com/aruw/controls/taproot/-/wikis/Definitions#main-control-board-mcb).
  The microcontroller has peripherals to interact with all other hardware
  subsystems including the chassis subsystem.
- Power distribution board (the RoboMaster [Power Management
  Module](https://www.robomaster.com/en-US/products/components/detail/1432) is
  used). This module provides power to all electronic systems on the robot,
  including the microcontroller and chassis motors.
- Four motor controllers ([RoboMaster C620 motor
  controllers](https://store.dji.com/product/rm-c620-brushless-dc-motor-speed-controller)).
  The motor controllers are wired to the microcontroller so the main
  microcontroller can send commands to the motor controllers using a protocol
  called
  [CAN](https://gitlab.com/aruw/controls/taproot/-/wikis/Definitions#controller-area-network-can).
  Furthermore, the motor controllers send motor information to the main
  microcontroller using the same protocol. This includes information such as the
  measured velocity of the motor and the motor shaft's rotational position. More
  information about the motor controller may be found in the [C620
  datasheet](https://rm-static.djicdn.com/tem/17348/RoboMaster%20C620%20Brushless%20DC%20Motor%20Speed%20Controller%20V1.01.pdf).
- Four motors ([RoboMaster M3508
  motors](https://www.robomaster.com/en-US/products/components/general/M3508)).
  Each motor is connected to the motor controller. The motor takes commands from
  the motor controller and sends information back to the motor controller. Each
  motor is connected to a [mecanum
  wheel](https://en.wikipedia.org/wiki/Mecanum_wheel).
- Remote control receiver. The [DR-16
  receiver](https://www.robomaster.com/en-US/products/components/detail/1837) is
  used to wirelessly receive remote control information from a paired
  [remote](https://gitlab.com/aruw/controls/taproot/-/wikis/Definitions#remote).

```{figure} ../uploads/chassis.png
:name: chassis
:width: 800px
:align: center

The robot you will be developing software for has the following hardware
configuration when viewed from above.
```

## Tank Drive

Your goal is to write a program that will allow you to control a robot with
**"tank drive",** a simple but common drive system. "Tank drive" is when you
control a robot with 2 joysticks, where the y-axis (vertical) of each joystick
controls the left and right side of the robot’s drive motors respectively. This
is how big military tanks work (hence the name).

For instance:
- Push both joysticks forward and the robot will move forward.
- Push both joysticks backward and the robot will move backward.
- Push left joystick forward and right joystick backward and robot will spin
  clockwise.

## Code Architecture

The robot we develop is complicated, so we attempt to keep our work sane by
using an architecture that we have developed previously that does some of the
tedious and more challenging operations for us. This curriculum focuses only on
learning the basics of using **subsystems** and **commands** (our high level
control architecture), as well as the **operator interface**, which interprets
remote and computer input and allows a user to control the robot. There is a lot
more that goes into controlling the robot that we will glaze over in this
tutorial.

Much of the subsystem and command framework developed previously was at a high
level based on FIRST programming. There is a good explanation of how subsystems
and commands work
[here](https://docs.wpilib.org/en/latest/docs/software/commandbased/what-is-command-based.html).
On this page, you should read the following sections:
- What Is "Command-Based" Programming?
- Subsystems and Commands
- How Commands Are Run

In addition, I recommend looking at the following pages:
- [Subsystems](https://docs.wpilib.org/en/latest/docs/software/commandbased/subsystems.html)
- [Commands](https://docs.wpilib.org/en/latest/docs/software/commandbased/commands.html)
- [The Command
  Scheduler](https://docs.wpilib.org/en/latest/docs/software/commandbased/command-scheduler.html)

Note though that while our code is not designed exactly like what is found in
FIRST, the ideas are similar. Our implementation of command groups, for example,
is very different.

For more information about our implementation of the command/subsystem
framework, see [this taproot wiki
page](https://gitlab.com/aruw/controls/taproot/-/wikis/Command-Subsystem-Framework).
Our implementation is not close to perfect, but hopefully you will be able to
improve it some time in the future!

## Installing the Development Environment

Before starting this tutorial, you must install the development environment.
Please refer to the README's ["New user
guide"](https://gitlab.com/aruw/controls/aruw-edu#new-user-guide) section to set
up aruw-edu.

Once you have installed aruw-edu, create a git branch on the repository with
some reasonable name. This is where you will be developing software for this
tutorial.

Reference [this Git
tutorial](https://gitlab.com/aruw/controls/taproot/-/wikis/Git-Tutorial) for
information about cloning, creating, and checking out a new branch if you are
unfamiliar with using Git.

## Notes Before Beginning

- This tutorial is not a comprehensive of C++ or the codebase. It is designed to
  guide your learning in a structured manner. As you go through this tutorial,
  it is expected that you work with others, ask leads questions about parts, and
  most importantly, search the internet for various questions that are not
  specific to the project. There is a plethora of information about C++ and C++
  tutorials you can refer to as you work through these tutorials.

- As you start working through this tutorial, **build your code often**. Refer
  to the ["Building and running via the
  terminal"](https://gitlab.com/aruw/controls/aruw-edu#building-and-running-via-the-terminal)
  section in aruw-edu's readme for more information about building the repo.
  This will help tremendously because it allows you to easily identify where
  errors are coming from. Unlike some IDEs, VSCode Intellisense is not always
  accurate and will sometimes not identify errors (especially more complex,
  annoying ones). So, please, please build often.

- The library that you will be using to develop your chassis tank drive software
  is called
  [taproot](https://gitlab.com/aruw/controls/taproot/-/wikis/definitions#taproot).
  Please look over the section called ["Code Generated in User
  Projects"](https://gitlab.com/aruw/controls/taproot/-/wikis/Code-Generation-in-User-Projects)
  in the taproot wiki. In aruw-edu, taproot is included as a git submodule at
  the top level of the repository. Generated taproot software that you will be
  interacting with is located in `./aruw-edu/aruw-edu-project/taproot`.

- If you are stuck on some part of this tutorial, please reach out for help.
  Solutions are available; however, these should not be referenced unless
  absolutely necessary. Doing so defeats the purpose of this tutorial.

- Various C++ tips that are not specific to our codebase will be in a "tip"
  block.

## Overview

During this project, you will be developing software in
`./aruw-edu/aruw-edu-project/src`. This wiki will guide you through finishing a
variety of incomplete subsystems and commands. Next is a description of each
section you will be developing.

- **`ControlOperatorInterface`**: Accesses inputs provided by a user from the
  [remote](https://gitlab.com/aruw/controls/taproot/-/wikis/definitions#remote).
  Provides an abstraction layer between remote input and user control used in
  commands (commands won't interact with the `Remote` object--rather they will
  call functions from the `ControlOperatorInterface` object). For example, the
  operator interface translates "move the left stick up and right stick down" to
  "I want to turn left".

- **`ChassisSubsystem`**: A subsystem that "owns" the four chassis drive motors.
  Interprets chassis movement directives sent by a command that is running and
  sends motor control commands to the motors. For example, a command will direct
  the chassis subsystem to move forward at 1 m/s. The subsystme will convert
  this directive into desired motor output commands for all four motors. 

- **`ChassisTankDriveCommand`**: Translates user input ("I want to turn left")
  into instructions that the `ChassisSubsystem` can understand.

- **`Robot`**: Where instances of the `ChassisSubsystem`,
  `ChassisTankDriveCommand`, and other control-related object instances are
  instantiated.

```{tip}
In C++, pointers, references, and variables are two very important
concepts that you must understand to successfully complete this tutorial.
Often, tutorials might state something like "pass variable X to function Y."
This statement in itself may be ambiguous, as the function may expect the 
variable to be passed as a pointer, reference, or raw variable. In order to
follow such an instruction, you must be able to understand how you are passing
variable X, be that as a reference, pointer, or variable. If you do not have
experience with C++ pointers, review [this slideshow](https://courses.cs.washington.edu/courses/cse333/20sp/lectures/03-c-pointers.pdf).
If you do not have experience with C++ references, review [this slideshow](https://courses.cs.washington.edu/courses/cse333/20sp/lectures/11-c++-refs-const-classes.pdf).
It is not expected that you are a master of pointers, references, and variables
before starting this tutorial, but it generally useful to have a base
understanding of how they work.

To test your understanding, answer the following questions:

  - Label `foo`, `bar`, and `baz` as pointers, references, or variables.
    ```cpp
    void function()
    {
      int foo{42};
      int *bar = &foo;
      int &baz = foo;
    }
    ```
    <details>
    <summary>Answer</summary>
      <code>foo</code>: variable<br>
      <code>bar</code>: pointer<br>
      <code>baz</code>: reference
    </details>

  - In the function `f1` defined below, how would you call `f2`, `f3`, and `f4`
    respectively, passing the variable `foo` to these three functions?
    ```cpp
    void f2(int var)
    {
      // Print var and address of var
      std::cout << var << ", " << &var << std::endl;
    }

    void f3(int *var)
    {
      // Print var and address of var
      std::cout << *var << ", " << var << std::endl;
    }

    void f4(int &var)
    {
      // Print var and address of var
      std::cout << var << ", " << &var << std::endl;
    }

    void f1()
    {
      int foo{42};
      // Now call f2, f3, and f4, passing foo to each.
    }
    ```
    <details>
    <summary>Answer</summary>
    <code>f2(foo)</code>, <code>f3(&foo)</code>, <code>f4(foo)</code>
    </details>

    Bonus question, will `f2`, `f3` and `f4` print identical values when the
    functions above are called from `f1()`?

    <details>
    <summary>Answer</summary>
    No, <code>f2</code> will print a different result compared to <code>f3</code> and <code>f4</code>. While <code>var</code>
    contains the same value in all three functions, the address of <code>var</code> is not the same in <code>f2</code>'s case since a copy of foo is
    being created for <code>f2</code> when you pass <code>foo</code> as a variable instead of a pointer
    or reference.
    </details>

    Assume `f1()` has now been changed to the following:
    ```cpp
    void f1()
    {
      int *foo = new int(42);
      // Now call f2, f3, and f4, passing foo to each
      delete foo;
    }
    ```
    How would you call `f2`, `f3`, and `f4` respectively, passing the variable
    `foo` to these three functions?

    <details>
    <summary>Answer</summary>
    <code>f2(*foo)</code>, <code>f3(foo)</code>, <code>f4(*foo)</code>
    </details>
```

## `ControlOperatorInterface`

First you will be adding a couple of functions to the `ControlOperatorInterface`
object in `aruw-edu-project/src/control/control_operator_interface.hpp` and
`aruw-edu-project/src/control/control_operator_interface.cpp`. In these files,
you will find comments of the form `// STEP <n> (Tank Drive):`. Hints
corresponding to these sections are listed below to help you along.

```{tip}
In C++, we break apart separate logical units into classes similar
to Java. In the normal case, each C++ class will have an associated **header**
file (suffixed commonly by `.hpp`) and **source** file (suffixed commonly by
`.cpp`). Typical naming convention associates class and file names together. For
instance, a class named `ControlOperatorInterface` will be housed in corresponding source
and header files named `control_operator_interface.cpp` and `control_operator_interface.hpp`
respectively.
```

You will be adding the `getChassisTankLeftInput` and `getChassisTankRightInput`
functions to the operator interface.

### STEP 1: Declare Functions

Declare the functions `getChassisTankLeftInput` and `getChassisTankRightInput`.
**The two functions you create should have 0 function parameters and return a
`float`.**

The functions should be declared as public members of the
`ControlOperatorInterface` class. The function should return a value between
`[-1 ,1]`.

In general, functions are _declared_ in headers (`.hpp` files) and _defined_ in
source files (`.cpp` files). The operator interface is no exception (for
reference refer to [this Stack Overflow
post](https://stackoverflow.com/questions/25274312/is-it-a-good-practice-to-define-c-functions-inside-header-files)).

```{tip}
You can think of "declaring" a function as a way of documenting
the blueprint of a function for others to then use. The declaration will include
the name of the function, return value, and input parameters. The declaration
**will not** contain any of the actual logic. On the other hand, "defining" a
function can be thought of as the actual meat of the function that gives the
function meaning. An outside observer may use a declared function without
knowing the actual meat of the function as long as they know the declaration.

For example, in the example class below, the function `greatFunction` has been
declared as a public member function of the class `Example`.

This is because `greatFunction` is declared in the example class within the
section with `public` at the top.

```cpp
// example.hpp

class Example {
public:
  void greatFunction();  // Declaration
};

-----------------------------
// example.cpp

void Example::greatFunction() {  // Definition
  // Do great things
}
```

```{tip}
C++ classes contain `public`, `protected`, and `private` sections
of code. In general, a function that is in the public section means anyone can
call that function. A function that is in the protected section means any child
object can call that function, and function in the private section means only
the object can call that function.
```

```{note}
When declaring the functions, add the keyword `mockable` in front of the
declaration to aid with unit testability. This is a requirement.
```

### STEP 2: Define Functions

In the tip above, `greatFunction` is defined in `example.cpp`. In general, the
formula for defining a function for a class outside of the class involves adding
`<class name>::` in front of the function name.

As you define the function, you have to actually write logic in the functions.

Refer to [the remote's generated
documentation](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1communication_1_1serial_1_1_remote.html)
to learn how to interact with the remote. The functions you define should call
the remote's `getChannel` function. The left vertical switch should be used
to get user input for the left tank drive function and the right vertical switch
for the right tank drive function.

```{note}
Be sure to limit the values returned by the `getChannel` function between
`[-1, 1]`. Use the `limitVal` function defined in `tap/algorithms/math_user_utils.hpp`.
```

## `ChassisSubsystem`

Next, you will be finishing the `ChassisSubsystem` class in
`aruw-edu-project/src/control/chassis_subsystem.hpp` and
`aruw-edu-project/src/control/chassis_subsystem.cpp`.

Again, follow the steps in the files.

### STEP 1: Create `ChassisSubsystem` Constructor

If you are unfamiliar with C++ class structure, read [this page on C++
classes](https://cplusplus.com/doc/tutorial/classes/). Each subsystem and
command is encapsulated in a class. A class constructor is needed to initialize
various objects that are declared in the class. In the case of the
`ChassisSubsystem`, the private and protected variables in the
`ChassisSubsystem` are shown below.

```cpp
private:
    /// Desired wheel output for each motor
    std::array<float, static_cast<uint8_t>(MotorId::NUM_MOTORS)> desiredOutput;

    /// PID controllers. Input desired wheel velocity, output desired motor current.
    std::array<Pid, static_cast<uint8_t>(MotorId::NUM_MOTORS)> pidControllers;

protected:
    /// Motors.
    std::array<Motor, static_cast<uint8_t>(MotorId::NUM_MOTORS)> motors;
```

The `desiredOutput`, `pidControllers` and `motors` arrays must be constructed in
the `ChassisSubsystem`'s constructor. In addition, any other objects that you
declare in the `ChassisSubsystem` must be constructed as well. Refer to the
section on [member initialization in
constructors](https://cplusplus.com/doc/tutorial/classes/) in the C++ classes
tutorial.

```{tip}

#### Member Initialization of Arrays in Classes

Once you understand member initialization of objects in classes, you must learn
specifically how to initialize an array (`std::array`). See the example below:

Here `Foo` constructs the array `a` with `int` elements 0, 1, 2, and 3.

```cpp
// foo.h

class Foo {
 public:
  Foo();

 private:
  std::array<int, 4> a;
};

------------------------------
// foo.cpp

Foo::Foo() : a{0, 1, 2, 3} {}
```

### STEP 2: `initialize` Function

This function should call each of the `DjiMotor`'s `initialize` function.

```{tip}
Any collection that has an iterator defined allows you to use a
[foreach loop](https://www.geeksforgeeks.org/g-fact-40-foreach-in-c-and-java/)
to iterate through all elements in the collection. `std::array` happens to
define an iterator. I recommend using a foreach loop to initialize each motor.
```

### STEP 3: `setVelocityTankDrive` Function

As the name and function comment suggests, this function takes in left and right
chassis speed in m/s. The post condition of this function is that the
`desiredOutput` array should store desired wheel speed (in rotations per second)
for each wheel. For example, given the left side speed is 1 m/s and the right
side speed is 0 m/s, the desired wheelspeed of the two left motors should be
positive and converted from linear speed to rotational speed, and the two right
wheels should have a desired wheelspeed of 0. Note the desired wheel speed
should be limited by `setVelocityTankDrive` to `[-MAX_WHEELSPEED_RPM,
MAX_WHEELSPEED_RPM]`. Look through the code to find how to convert m/s to RPM as required. Note that
the motors are geared approximately 19:1, so any conversion should should
include a multiplication by by 19. Also, the wheel diameter is 0.076 m.

```{tip}
An [enum class](https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum)
called `MotorId` is defined in `chassis_subsystem.hpp`. Use this enum to index
into the `desiredOutput` array. Since enum classes do not implicitly convert to
other types, you must manually cast the enum types to `int`s.

For example, to index into the left front desired output array, write
`desiredOutput[static_cast<int>(MotorId::LF)]`
```

### STEP 4: `refresh` Function

The `refresh` function is called repeatedly at some specified frequency. In
`aruw-mcb`, this rate is 500 Hz. This function should do the following:

- Call each `pidControllers`'s `update` function. This function expects an
  input. The input should be the difference between the desired RPM and the
  motor's actual RPM. This will automatically set the motor's speed so that the chassis will move at the correct velocity.
- Call each motor's `setDesiredOutput` function, passing to this function the
  `pidControllers`'s `getValue` function.

## `ChassisTankDriveCommand`

Next you will be finishing the `ChassisTankDriveCommand` object in
`aruw-edu-project/src/control/chassis_drive_command.hpp` and
`aruw-edu-project/src/control/chassis_drive_command.cpp`. Again, complete all
the steps in both files.

### STEP 1: Constructor

The constructor of this object should store the passed in `chassis` in the
private member function called `chassis`. Same goes for the
`ControlOperatorInterface`. Also, you should call the `Command`'s
`addSubsystemRequirement`. This is a required step for the `Command` to function
properly.

### STEP 2: `execute` Function

This function should set the chassis's desired speed to values reported by the
`ControlOperatorInterface`. Use the operator interface's
`getChassisTankLeftInput` and `getChassisTankRightInput` functions. These
functions return values between `[-1, 1]`, so convert this input to
`[-MAX_CHASSIS_SPEED_MPS, MAX_CHASSIS_SPEED_MPS]` before passing to the
`ChassisSubsystem`'s `setVelocityTankDrive` function.

### STEP 3: `end` Function

This function should set the chassis's desired speed to all 0s.

(tank_drive_robot)=
## `Robot`

Next, you will be finishing the `Robot` object in
`aruw-edu-project/src/control/standard.hpp` and
`aruw-edu-project/src/control/standard.cpp`. The `Robot` object is a singleton,
which means a single `Robot` object is exected to be created. This object is
created in `main.cpp`.

Again, follow the steps in the two files and refer to the documentation below to
help guide you through finishing the `Robot` object.

### STEP 1: Declare `ChassisSubsystem`

Note that the subsystem is scoped inside the namespace `control`. Thus
`chassis::` will have to be appended on to the object name.
```{tip}
Unlike in Java, declaring a variable of class or struct type
automatically instantiates that type -- there are no "null references" in C++,
and in fact, reference semantics as seen in Java are only applicable to C++
pointers. If you don't know how to instantiate objects in C++, look
[here](http://www.cplusplus.com/doc/tutorial/classes/).
```

### STEP 2: Declare `ChassisTankDriveCommand`

This should be very similar to step 1.

### STEP 3: Construct Subsystem and Command

Now that you have declared your subsystem and command in `standard.hpp`, you
must construct these objects in `standard.cpp`. Use your understanding of C++
member initialization to construct these the `ChassisSubsystem` and
`ChassisTankDriveCommand`.

### STEP 4: Initialize Chassis

In the `initializeSubsystems()` function in `standard.cpp`, you must manually
initialize the `chassisSubsystem` object. Call `chassisSubsystem`'s `initialize`
function here.

### STEP 5: Register Chassis

Any `Subsystem` you would like the command scheduler to manage must be
registered with it during runtime. The `Drivers` object that the `Standard` has
a reference to contains the main `CommandScheduler` that you should register the
`ChassisSubsystem` you created with. Refer to the `CommandScheduler`'s
[`registerSubsystem`
function](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1_command_scheduler.html#_CPPv4N3tap7control16CommandScheduler17registerSubsystemEP9Subsystem).

### STEP 6: Set Tank Drive Command as Default

We want the tank drive command you constructed to be a default command of the
chassis subsystem command you created. This way, the command scheduler will
execute the tank drive command whenever no other commands that have the chassis
subsystem as a dependency are running (so for now, all the time). To do so, the
`Subsystem` class (the `ChassisSubsystem` is a child of `Subsystem`) has a
function called `setDefaultCommand`. In the `setDefaultSoldierCommands`
subsystem, call this function, passing the `ChassisTankDriveCommand` you
declared previously.

## Run Tests

At this point, you should have completed all required software for the tank
drive tutorial. Before running the software you wrote on a robot, it is expected
that you code has unit tests associated with it. For this tutorial, unit tests
have been provided, so you do not have to write your own (see
`./aruw-edu-project/test/control`).

To compile and run tests for the chassis tank drive, run `scons run-tests
tutorial_to_test=tank_drive`.

```{attention}
One test, `ARUW_EDU_Standard.Initialization` will not pass until you have
completed other tutorials. Otherwise, all other tests should pass when running
`scons run-tests tutorial_to_test=tank_drive`
```

(tank_drive_run_code_on_a_robot)=
## Run Code on a Robot

It is now time to test and debug your code. Before you begin, **it is imperative
that you read the [debugging safety
information](https://gitlab.com/aruw/controls/taproot/-/wikis/Debugging-Safety-Information)
page**. I cannot stress this enough.

It is now time to run and debug your code. Reading the [debugging information
for using the the
ST-Link](https://gitlab.com/aruw/controls/taproot/-/wikis/Debugging-With-STLink)
and [debugging information for using the
J-Link](https://gitlab.com/aruw/controls/taproot/-/wikis/Debugging-With-JLink)
will give you a good base for debugging your code.

After reading these guides, test your code on the soldier. You should ask
another member for help getting the robot set up, though below is a list of some
things you should follow specific to this tutorial:

- You probably need a robot. Ask someone if you don't know where to find one.
- Before you begin, **prop the robot up on blocks**.
- Unplug power to everything but the chassis (again, ask another member for help
  so this is done correctly).
- Ensure the remote is connected to the robot's MCB.

(tank_drive_final_steps)=
## Final Steps

After you are completed debugging, you should have functional tank drive
control. Following the same steps from above, **add, commit, and push your code
to the main repo** so it can be reviewed. Again, I recommend our [Git
tutorial](https://gitlab.com/aruw/controls/taproot/-/wikis/Git-Tutorial) if you
are having trouble.

Finally, find another member to review your code to ensure quality by opening up
a merge request.
