# Vulp

![C++ version](https://img.shields.io/badge/C++-17/20-blue.svg?style=flat)

Real-time motion control for Python.

Vulp is grounded on the observation that many high-value robotic tasks require [real-time](https://en.wiktionary.org/wiki/real-time#English) but **not high-frequency** performance. Notably, there is [both theoretical and empirical evidence](https://arxiv.org/pdf/1907.01805.pdf) that bipeds and quadrupeds can balance themselves at 5–15 Hz, despite being commonly implemented at 200–1000 Hz, with little impact to their performance. Vulp takes this idea to code by giving Python code an interface to interact at low-frequency with high-frequency actuators and simulators.

<!-- https://user-images.githubusercontent.com/1189580/170522853-45d8a499-17a4-41a6-8dac-6add5d02f0fd.svg -->

<!-- <p align=center>
    <img src="https://user-images.githubusercontent.com/1189580/170526614-ffaf2bc3-73c0-4fb6-b9dc-e00be3dc8e5b.svg">
</p> -->

<img src="https://user-images.githubusercontent.com/1189580/170533494-840dcf5e-5529-4211-9e80-65fb715a8392.svg" width="500" align="right">


Vulp binaries, called _spines_, talk to Python programs, called _agents_, in a standard action-observation loop. Simultaneously, they talk to actuators or simulator threads so that this loop does what we expect. In its simplest form, the ``action`` dictionary is a list of actuator commands that the spine merely forwards while collecting actuator readings in the ``observation`` dictionary. But Vulp provides an API of controller/observer pipelines to grow spines that handle more high-level objects. For example, a spine could have a model predictive controller in its pipeline reading its targets from ``action``, or a multi-contact state estimator that writing to ``observation``.

### Try it out!

<!-- GIF: https://user-images.githubusercontent.com/1189580/170491850-dfbb4786-12ff-4fe8-8080-9413d68acfc1.gif -->
<!-- Issue: https://github.com/github/feedback/discussions/17256 -->

<img src="https://user-images.githubusercontent.com/1189580/170496331-e1293dd3-b50c-40ee-9c2e-f75f3096ebd8.png" height="100" align="right" />

```console
git clone https://github.com/tasts-robots/upkie_locomotion.git
cd upkie_locomotion
./tools/bazelisk run -c opt //agents/blue_balancer:bullet
```

There is no dependency to install on Linux. Vulp comes with batteries included thanks to [Bazel](https://bazel.build/), which figures out dependencies, builds them and runs the Python controller all in one go. The syntax is the same to deploy to the Raspberry Pi [of the real robot](https://www.youtube.com/shorts/8b36XcCgh7s).

## Features and non-features

### Features

- Interface to the [mjbots pi3hat](https://mjbots.com/products/mjbots-pi3hat-r4-4b) and mjbots actuators
- Interface to the [Bullet](http://bulletphysics.org/) simulator
- Observer pipeline to output custom observations
- 🏗️ Controller pipeline to handle custom actions
- Soft real-time: spine-agent loop execution times are predictable and repeatble

### Non-features

- Hard real-time: 

## Getting started

Check out [``vulp_example``](https://github.com/tasts-robots/vulp_example) to get started with a minimal Python controller and Vulp spine.

## Things to keep in mind

* Bazel's [compilation mode](https://bazel.build/reference/command-line-reference#flag--compilation_mode) is "fastbuild" by default. Switch to ``-c opt`` for better performance when running robot experiments or simulations.

## Q and A

> Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably lightweight and quite reliable in their reaction times 😉

> How can it be real-time in Python, with garbage collection and all?

Vulp's code is written in C++ and runs at a reliable rate. The ``SpineInterface`` API it exposes in Python is quite thin and also runs at a reliable rate (up to the kilohertz). Of course Vulp's . Here is for example the measured control period of a complete Python controller for Upkie (the pink balancer from [`upkie_locomotion`](https://github.com/tasts-robots/upkie_locomotion)) running on a Raspberry Pi 4 Model B. It includes useful tasks like balancing and whole-body inverse kinematics by quadratic programming:

[[[PLOT HERE]]]
