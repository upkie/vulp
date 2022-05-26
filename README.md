# Vulp

![C++ version](https://img.shields.io/badge/C++-17/20-blue.svg?style=flat)

Real-time motion control for Python.

> [real-time](https://en.wiktionary.org/wiki/real-time#English): of a system that responds to events or signals within a predictable time after their occurrence; specifically the response time must be within the maximum allowed, but is typically synchronous.

* Real-time does not mean *high frequency*. Many meaningful tasks in robotics do not require 1 kHz, hard real-time guarantees. Balancing a full-fledged humanoid robot can be performed as slow as 5 Hz (200 ms), which is a remarkable [theoretical and empirical result](https://arxiv.org/pdf/1907.01805.pdf).

### Try it out!

<!-- GIF: https://user-images.githubusercontent.com/1189580/170491850-dfbb4786-12ff-4fe8-8080-9413d68acfc1.gif -->
<!-- Issue: https://github.com/github/feedback/discussions/17256 -->

<img src="https://user-images.githubusercontent.com/1189580/170496331-e1293dd3-b50c-40ee-9c2e-f75f3096ebd8.png" height="100" align="right" />

```console
git clone https://github.com/tasts-robots/upkie_locomotion.git
cd upkie_locomotion
./tools/bazelisk run -c opt //agents/blue_balancer:bullet
```

There is no dependency to install on Linux. Vulp comes with batteries included thanks to [Bazel](https://bazel.build/), which figures out the dependencies, builds and runs the Python controller above all in one go. The syntax is the same to deploy to the Raspberry Pi [on the real robot](https://www.youtube.com/shorts/8b36XcCgh7s).

Check out [``vulp_example``](https://github.com/tasts-robots/vulp_example) to get started with a minimal Python controller and Vulp spine.

## Features and non-features

### Features

- Compatible with mjbots actuators via the [pi3hat](https://mjbots.com/products/mjbots-pi3hat-r4-4b)
- ...

### Non-features

- ...

## Things to keep in mind

* Bazel's [compilation mode](https://bazel.build/reference/command-line-reference#flag--compilation_mode) is "fastbuild" by default. Switch to ``-c opt`` for better performance when running robot experiments or simulations.

## Q and A

> Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably lightweight and quite reliable in their reaction times 😉

> How can it be real-time in Python, with garbage collection and all?

Most of Vulp's code is written in C++ and runs at a reliable rate. The interface it exposes in Python is quite thin and runs at a reliable rate as well, up to the kilohertz at least. Now, Vulp cannot guarantee that the code on your side is real-time, but you may be surprised at how reliably operations such as neural network inference or quadratic programming can run even in pure Python. Here is for example the measured control period of a complete Python controller for [Upkie](https://www.youtube.com/watch?v=NO_TkHGS0wQ) including balancing and whole-body inverse kinematics:

[[[PLOT HERE]]]
