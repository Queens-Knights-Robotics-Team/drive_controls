# Agitator Control Tutorial

## Table of Contents

```{toctree}
2_agitator_control
```

## Requirements

Pleas first complete the [Tank Drive Tutorial](1_tank_drive) and all of its
required reading.

## Hardware

The agitator is an essential component of any launching mechanism. Hardware
components used are similar to the tank drive tutorial; however, the motor and
motor controller used is typically different (for the standard robot).

- Power management module. See the [Tank Drive Tutorial](1_tank_drive).
- Main microcontroller. See the Tank Drive Tutorial.
- One motor controller ([RoboMaster
  C610](https://m.dji.com/product/rm-c610-brushless-dc-motor-speed-control)).
  The wiring is identical to that described in the tank drive tutorial.
- One motor ([RoboMaster
  M2006](https://www.robomaster.com/en-US/products/components/detail/1277)). The
  motor is connected to the motor controller identical to what is described in
  the tank drive tutorial. The motor is physically connected to an "agitator"
  (also described as an "indexer" or "stirrer").
- Remote control receiver and remote. The receiver will be used in a similar
  fashion as the tank drive tutorial. During this tutorial, you will be
  connecting the remote to a computer, which you will used to receive mouse
  button click information.

## Agitator Control

Some agitator systems are more mechanically complex than others; however, all
agitators share the same purpose--to turn a container of unorganized projectiles
into a stream of projectiles that can then be fed into a launching mechanism. In
the case of ARUW's standard, the agitator is a turbine-like mechanism with 10
slots for balls to fall into. When spun, the projectiles are fed into a pair of
flywheels (rubber wheels that spin very fast) that launch projectiles out. This
design is typical of all RoboMaster standard robots.

A control system must be designed to control the rotation of the agitator. In
the simpliest case, an agitator system must rotate enough to launch one
projectile when a user commands (be that via a remote control or
keyboard/mouse). This is similar to FPS (point and click) games.

## Overview

During this tutorial, you will be developing software primarily in
`./aruw-edu/aruw-edu-project/src`. This wiki will guide you finishing an
incomplete subsystem. Furthermore, you will be designing a [PID
controller](https://en.wikipedia.org/wiki/PID_controller) that will be used in
the agitator subsystem and you will use to control the agitator. Next is a
description of each section you will be developing. You will then be using a
pre-built command from taproot
([`MoveIntegralCommand`](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1setpoint_1_1_move_integral_command.html#exhale-class-classtap-1-1control-1-1setpoint-1-1-move-integral-command))
to command the agitator subsystem to rotate.

- **`VelocityAgitatorSubsystem`**: A subsystem that "owns" an agitator motor.
  Receives velocity instructions ("Rotate agitator at 1 rad/second) from a
  command and sets the desired output of the agitator motor.

- **`EduPid`**: A generic [PID](https://en.wikipedia.org/wiki/PID_controller)
  [control systen](https://en.wikipedia.org/wiki/Control_system) encapsulated in
  an object. This object will be used in the `VelocityAgitatorSubsystem`, but
  will be generic enough to be used for many other applications.

- **`Robot`**: Where instance of the `VelocityAgitatorSubsystem` and
  `MoveIntegralCommand` are instantiated. Same object where instances of the
  `ChassisSubsystem` and `ChassisTankDriveCommand` were instantiated in the tank
  drive tutorial.

Like in the previous tutorial, you will find comments of the form `// STEP <n>
(Agitator Control)` to guide you through completing the tutorial.

## Getting Started

It is recommended that you branch off of your git branch you created for the
tank drive tutorial when working on agitator control. By branching off of the
branch you already created, when you finish agitator control, you will have
software for a functional chassis and agitator subsystem all in the same place.
At the end of all the tutorials, if you continue to follow this pattern, you
will have all the software for a standard robot.

## `EduPid`

### What is a PID controller?

Coming from [National
Instruments]("https://www.ni.com/en-us/innovations/white-papers/06/pid-theory-explained.html"),
the following explains why PID control is such a popular design.

> Proportional-Integral-Derivative (PID) control is the most common control
> algorithm used in industry and has been universally accepted in industrial
> control. The popularity of PID controllers can be attributed partly to their
> robust performance in a wide range of operating conditions and partly to their
> functional simplicity, which allows engineers to operate them in a simple,
> straightforward manner.
>
> As the name suggests, PID algorithm consists of three basic coefficients;
> proportional, integral and derivative which are varied to get optimal
> response.

When designing a "closed loop system," we aim to take into account external
disturbances that we would otherwise ignore in an "open loop system."
Implementing a closed-loop control system typically leads to better performance
in real-world applications because external disturbances are numerous and must
be accounted for. We can account for such disturbances by gathering feedback
about our system and comparing that to the desired set-point state.

Many of the control systems we build in ARUW utilizes PID control. We will get
into a more concrete example of PID control in [a below
section](#what-you'll-be-doing-agitator-control), but first we must understand
the mathematical algorithm that is PID control.

_**Note:** Brian Douglas has a number of enlightening videos on control theory
on YouTube. At the very least I recommend watching his two [videos on PID
control](https://www.youtube.com/watch?v=UR0hOmjaHp0&list=PLUMWjy5jgHK20UW0yM22HYEUTMJfla7Mb)
to get familiar with PID and some real world applications._

```{figure} ../uploads/pid_control.png
:name: pid control
:width: 600px
:align: center

The block diagram above is a high level block diagram of a basic PID controller.
It is typical to use block diagrams to describe a control system.
```

```{note}
A _control system_ is a system, process, program, or similar that
regulates something. A widely used example of a control system is a process that
controls temperature of a furnace. The process regulates the temperature of the
furnace in a way designed by the control systems engineer.
```

As can be seen in the diagram above, the difference between the set-point and
the process variable (see below) is calculated and fed into the proportional,
integral, and derivative equations, which are then summed and outputted to the
device we are controlling. The arrow going from "Output" to the leftmost circle
(with the delta) represents the feedback in the system described above and is
often called the process variable. Without this arrow, the system is no longer
closed.

#### Desired set-point, process variable, and error

Before PID terms can be computed, we must understand how error in our system is
conceived. How we define a set-point can be driven by many things, including
what sort of control we want over the system and in what sort of ways we can
measure the system.

For our applications, when designing a simple PID controller, let us consider
the different ways that you can tell a motor to move:

You can tell a motor a...
- desired position.
- desired velocity.
- desired acceleration.
- desired jerk.
- etc.

As you can see, you can think up a lot of ways to theoretically control a motor,
but typically because we can directly measure the position and velocity of a
motor, the bread and butter PID controllers we design will have a position or
velocity set-point.

A process variable is the current measured value of a particular part of a
process which is being monitored or controlled
([wikipedia](https://en.wikipedia.org/wiki/Process_variable)). For PID control,
the process variable has the same units as the desired set-point. If we wish to
control the shaft angle of a motor, for example, the set-point would be a
desired shaft angle and the process variable would be the actual shaft angle of
the motor.

Putting together the desired set-point and process variable, the error is just
the difference between the two. Let us next consider separately the P, I, and D
terms of PID control.

#### Proportional

The proportional term of the PID controller applies an output proportional the
the error, (the difference between the desired set-point and process variable).
We multiply the error by some $K_p$ and add this to the output. 

#### Integral

The integral portion of PID sums the error over time (akin to taking the
integral) and multiplies this by some $K_i$. This integral is added to the
output and is typically used for reducing small error present over time. Because
we are working on a real time system (the MCB), we must integrate in the
discrete time domain.

##### The discrete time domain and integration

We think of all the work we do on the MCB and any other microcontroller as being
done in a discrete time domain. By this, I mean when doing operations on a
microcontroller, the time steps and operations are discrete. There will be some
nonzero time step between each loop of the controller (typically for the work we
do, a few milliseconds).

To give you an example of what I mean by a "discrete integral", consider the
following pseudo-code which computes the discrete integral of some `value`,
where `dt` is the time we delay between integral calculations.

```cpp
float integral = 0.0f;
while(true)
{
    integral = integral + value * dt;
    delay(dt);
}
```

We can think of a discrete integral as an approximation of a true integral using
the following summation:

```{math}
\int_0^{t} f(x)dx \approx \sum_{x=0}^t f(x)\Delta_t
```

where $\Delta_t$ is the time difference between the measurement of $f(t)$ and
$f(t+1)$.

So if this summation can be thought of as an approximate integral, let us
consider the discrete integral between $[0, t+1]$ given the integral from $[0,
t]$ is $\sum_{x=0}^t f(x)\Delta_x$ and given we have $f(t+1)$. This integral is
really the summation just written above, but from 0 to $t+1$ instead of from 0
to $t$. Since we don't have the summation between $[0, t+1]$ but we do have the
summation between $[0, t]$, however, we can write the following:

```{math}
\sum_{x=0}^{t+1} f(x) \Delta_x = \sum_{x=0}^{t} f(x) \Delta_x + f(t+1)\Delta_{t+1}
```

So we can conclude that the discrete integral from $[0, t+1]$ is simply the
discrete integral from $[0, t]$ plus $f(t+1)\Delta_t$. This can be proven more
formally using induction, which you can look into if you are interested. 

Going back to the code, we think of `integral = integral + value * dt` as the
step where we add to the integral we have already computed, $\sum_{x=0}^t
f(x)\Delta_x$, and the new data measurement $f(t+1) \Delta_{t+1}$ to get the new
integral.

Hopefully this idea of approximating an integral makes sense, otherwise review
your calculus 2 knowledge and for more in depth information [this
website](http://hplgit.github.io/prog4comp/doc/pub/p4c-sphinx-Python/._pylight004.html).

#### Derivative

The derivative term multiples the derivative of the error by some
proportionality $K_d$ and subtracts this from the overall output. This is
usually used to compensate for a very large error, which can result in a very
large proportional term. When the error is very large, the derivative will be
very large and so the derivative term in the PID controller will decrease the
overall output to a more reasonable amount. On the other hand, when the error is
very small, the derivative is small and so the proportional term can dominate.

In the discrete domain (see [The discrete time domain and
integration](#The-discrete-time-domain-and-integration)), the derivative of some
error, or $\frac{d e(t)}{dt}$, is equal to $\frac{\Delta_{e}}{\Delta_t}$, where
$\Delta_t$ is the length of time between successive iterations and $\Delta_{e}$
is the difference between successive error measurements, $\Delta_{e} = e(t) -
e(t-1)$.

#### The final equation

Putting together P, I, and D we get the final continuous equation:

```{math}
u(t) = K_p e(t) + K_i \int_0^t e(t) dt + K_d \frac{d e(t)}{dt}
```

or the following discrete equation:

```{math}
u(t) = K_p e(t) + K_i \sum_{x=0}^t e(x) \Delta_t + \frac{K_d}{\Delta_t} (e(t) - e(t-1))
```

With this basic PID controller, we can control a wide variety of systems.

Open up `aruw-edu` in the development container and navigate to the file
`edu_pid.hpp` (`aruw-edu-project/src/control/algorithms/edu_pid.hpp`). We will
generalize the PID controller such that however we measure error does not
matter. This will allow us to use this PID controller in a number of different
scenarios.

The constructor of this class contains the initialization of the controller.
This PID controller will have user settable constants $K_p$, $K_i$, and
$`K_d$ described in [the above section](#what-is-a-pid-controller).
Additionally, while this was not mentioned above, it is typical to control the
PID controller's maximum output and the maximum integral value, so we will allow
the user to set these parameters as well in the constructor.

### STEP 1: `runControllerDerivateError` function

Open up `aruw-edu-project/src/control/algorithms/edu_pid.cpp`. Here you must
implement `float runControllerDerivateError(float error, float dt)`. Refer to
the section [what is a PID controller](#what-is-a-pid-controller) for help with
implementing this function. In particular, consider carefully the following
equation:

```{math}
u(t) = K_p e(t) + K_i \sum_{x=0}^t e(x) \Delta_t + K_d \Delta_t (e(t)-e(t-1)
```

`runControllerDerivativeError` should compute $u(t)$ given $e(t)$ and
$\Delta_t$. 

A few things to keep in mind:
- After calculating $u(t)$, store it in the variable `output`. Return `output`
  from `runControllerDerivativeError`.

- Store the previous error for the derivative term in `prevError`.

- After calculating `currErrorI`, make sure to limit it to `maxICumulative`
  using `tap::algorithms::limitVal`.

- Limit `output` similarly to `maxOutput`.

## `VelocityAgitatorSubsystem`

Next, you will be finishing the implementation of the
`VelocityAgitatorSubsystem` object in
`aruw-edu-project/src/control/agitator/velocity_agitator_subsystem.hpp` and
`aruw-edu-project/src/control/agitator/velocity_agitator_subsystem.cpp`. The
`VelocityAgitatorSubsystem` object inherits from the `IntegralSetpointSubsystem`
object, which has a bunch of pure virtual functions that you must override.

```{tip}
C++ inheritance structure is similar to java. If you are unfamiliar with 
inheritance, refer to [this guide](https://www.tutorialspoint.com/cplusplus/cpp_inheritance.htm).
In the subsystem command framework, all commands and subsystems inherit from the
 `Subsystem` and `Command` class, respectively.

An important difference between C++ and java inheritance is that object
functions are by default not overridable by children objects. You must add the
`virtual` keyword before any function you wish to be overridden.

Furthermore, interfaces in C++ are not a unique type as they are in java. 
Instead, individual functions can be defined as "pure virtual" by adding `= 0`
to the end of a function definition. Any object that has inherited from a class
with pure virtual functions **must** override these pure virtual functions.
```

The class diagram depecting the inheritance structure of the
`VelocityAgitatorSubsystem` and associated parent classes is shown below,
including all the functions you must implement.

```{mermaid}
classDiagram
class Subsystem
class SetpointSubsystem
class IntegrableSetpointSubsystem
class VelocityAgitatorSubsystem

Subsystem : virtual initialize()
Subsystem : virtual refresh()

SetpointSubsystem : virtual getSetpoint()
SetpointSubsystem : virtual setSetpoint(float newAngle)
SetpointSubsystem : virtual getCurrentValue()
SetpointSubsystem : virtual calibrateHere()
SetpointSubsystem : virtual isCalibrated()
SetpointSubsystem : virtual isOnline()
SetpointSubsystem : virtual getVelocity()

IntegrableSetpointSubsystem : virtual getCurrentValueIntegral()

VelocityAgitatorSubsystem : initialize() override
VelocityAgitatorSubsystem : refresh() override
VelocityAgitatorSubsystem : getSetpoint() override
VelocityAgitatorSubsystem : setSetpoint(float newAngle) override
VelocityAgitatorSubsystem : getCurrentValue() override
VelocityAgitatorSubsystem : calibrateHere() override
VelocityAgitatorSubsystem : isCalibrated() override
VelocityAgitatorSubsystem : isOnline() override
VelocityAgitatorSubsystem : getVelocity() override
VelocityAgitatorSubsystem : getCurrentValueIntegral() override

Subsystem <|-- SetpointSubsystem
SetpointSubsystem <|-- IntegrableSetpointSubsystem
IntegrableSetpointSubsystem <|-- VelocityAgitatorSubsystem
```

```{note}
We are inheriting from this interface instead of extending `Subsystem` because
this allows us to use commands in taproot that operate with an
`IntegrableSetpointSubsystem`. In this tutorial, we will be using the
`MoveIntegralCommand` to rotate the agitator; however, a number of other more
powerful commands exist in taproot that you could also use with the 
`IntegrableSetpointSubsystem`.
```

### STEP 1: create constructor

When declaring the `VelocityAgitatorSubsystem`'s constructor, be sure to
construct the `velocityPid` object, passing in the PID config struct.

### STEP 2: `initialize` function

Here, you should initialize the agitator motor.

### STEP 3: `getSetpoint` function

This function should return the `velocitySetpoint` (a local variable of
`VelocityAgitatorSubsystem`).

### STEP 4: `getCurrentValue` function

This function should return the agitator's actual velocity, as reported by the
`DjiMotor` object. Use the `DjiMotor`'s `getShaftRPM` function. Note the motor
you are using has a gear ratio of `AGITATOR_GEAR_RATIO_M2006`. Convert the shaft
RPM to agitator rotational speed in radians/second.

### STEP 5: `calibrateHere` function

This function should return `false` if the motor is offline and `true` if it is
online. Furthermore, if the motor is online, the `agitatorCalibratedZeroAngle`
should be set to `getUncalibratedAgitatorAngle`. Also `calibrated` should be set
to `true`.

### STEP 6: `refresh` function

This function should perform a couple of steps. Remember from the tank drive
tutorial, the `refresh` function is called at some fixed frequency (in
aruw-edu's case, 500Hz). Thus, this function should run the PID controller your
created in the previous section. Furthermore, you should only run the pid
controller if the agitator is calibrated. Refer to the following pseudocode when
implementing the `refresh` function:

```
if offline:
  calibrated = false

if calibrated:
  run PID controller
  set motor output to PID controller's getOutput function
else:
  call calibrateHere function
```

```{note}
Use the function `tap::arch::clock::getTimeMilliseconds()` to determine the
current time when computing `dt`.
```

```{note}
When running the PID controller, you want to run a velocity PID controller.
Thus, the error that you pass to the controller is the difference between the
target velocity and the actual velocity, in angular velocity (rad/second).
```

### STEP 7: `isOnline` function

This function should return the `DjiMotor`'s `isMotorOnline` function.

### STEP 8: `getCurrentValueIntegral` function

Since the setpoint being returned by the `VelocityAgitatorSubsystem` is a
velocity in radians, the integral of the setpoint is a position in radians. This
should be the calibrated angle (i.e. `getUncalibratedAgitatorAngle() -
agitatorCalibratedZeroAngle`).

## `Robot`

Next, you will be finishing the `Robot` object in
`aruw-edu-project/src/control/standard.hpp` and
`aruw-edu-project/src/control/standard.cpp`. This is the same object you
modified in the tank drive tutorial.

Steps 1-5 are quite similar to the tank drive tutorial, so refer to {ref}`these
sections <tank_drive_robot>` in the other tutorial for these steps.

### STEP 6: declare `HoldRepeatCommandMapping` (`rightSwitchUp`)

Please refer to the
[docs](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1_hold_repeat_command_mapping.html)
for how a `HoldRepeatCommandMapping` works. In this case, you will associate the
`MoveIntegralCommand` you created in the previous steps with this command
mapping. In doing so, when this command mapping is registered, every time the
right switch is in the up position, the `MoveIntegralCommand` you declared will
be repeatedly scheduled, causing the agitator to rotate rapidly.

For tests to function properly, when declaring the object, name it
`rightSwitchUp`.

### STEP 7: declare `HoldCommandMapping` (`leftMousePressed`)

Please refer to the
[docs](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1_hold_command_mapping.html)
for how a `HoldCommandMapping` works. In this case, you will associate the
`MoveIntegralCommand` you created in the previous steps with this command
mapping. In doing so, when this command mapping is registered, every time the
left mouse is pressed, the agitator will rotate once, causing a projectile to be
launched.

### STEP 8: construct `HoldRepeatCommandMapping` and `HoldCommandMapping`

Here, you will use member initialization to construct the command mappings you
declared in steps 6 and 7.

Command mappings can be a bit tricky to construct. You can use [aruw-mcb's
`soldier_control.cpp`](https://gitlab.com/aruw/controls/aruw-mcb/-/blob/develop/aruw-mcb-project/src/aruwsrc/control/soldier_control.cpp#L393)
as a reference for how to create a `HoldRepeatCommandMapping` or
`HoldCommandMapping`. While constructing both mappings, you must pass the
`MoveIntegralCommand` you previously declared as an element in the [`cmds`
vector](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1_command_mapping.html#_CPPv4N3tap7control14CommandMapping14CommandMappingEP7DriversKNSt6vectorIP7CommandEERK14RemoteMapState).

### STEP 9: Register `HoldRepeatCommandMapping` and `HoldCommandMapping`

The reference to the `Drivers` that the `Robot` owns has a
[`CommandMapper`](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1_command_mapper.html)
object. You must manually register any command mappings with this instance of
the `CommandMapper` by calling the mapper's
[`addMap`](https://aruw.gitlab.io/controls/taproot/api/classtap_1_1control_1_1_command_mapper.html#_CPPv4N3tap7control13CommandMapper6addMapEP14CommandMapping)
function.

## Run Tests

At this point, you should have completed all required software for the agitator
control tutorial. Before running the software you wrote on a robot, it is
expected that you code has unit tests associated with it. For this tutorial,
unit tests have been provided, so you do not have to write your own (see
`./aruw-edu-project/test/control`). To compile and run tests for the agitator
control, run `scons run-tests tutorial_to_test=agitator_control`.

## Run Code on a Robot

Refer to the tank drive's {ref}`run code on a
robot <tank_drive_run_code_on_a_robot>` section for how to run code on a robot.
Note: because you have not implemented a frictionwheel subsystem, you will not
be able to launch projectiles. When running on a robot, be sure that no balls
are in the hopper of the robot you are testing on to avoid breaking hardware.

## Final Steps


Refer to the {ref}`tank drive tutorial's final steps<tank_drive_final_steps>`.
