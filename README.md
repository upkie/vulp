# Vulp

![C++ version](https://img.shields.io/badge/C++-17/20-blue.svg?style=flat)

Real-time motion control for Python. 🚧 **Pre-release.**

Vulp is grounded in the observation that many high-value robotic tasks require [real-time](https://en.wiktionary.org/wiki/real-time#English) but **not high-frequency** performance. Notably, there is [both theoretical and empirical evidence](https://arxiv.org/pdf/1907.01805.pdf) that bipeds and quadrupeds can balance themselves at 5–15 Hz, despite being commonly implemented at 200–1000 Hz, with little impact to their performance. Vulp takes this idea to code by giving Python code an interface to interact at low-frequency with high-frequency actuators and simulators.

<!-- https://user-images.githubusercontent.com/1189580/170533494-840dcf5e-5529-4211-9e80-65fb715a8392.svg -->
<img src="https://user-images.githubusercontent.com/1189580/170685599-fc763737-d217-4f81-b0c9-e3728321e3c4.svg" width="500" align="right">

Vulp binaries, called _spines_, talk to Python programs, called _agents_, in a standard action-observation loop. Simultaneously, they talk to actuators or simulator threads so that this loop does what we think it does. In its simplest form, the ``action`` dictionary is a list of actuator commands that the spine merely forwards it while collecting actuator readings in the ``observation`` dictionary. But Vulp provides a pipeline API to plug additional controllers (for higher-level actions) and observers (for richer observations). For example, a spine can run an inverse kinematics controller that reads task targets from ``action``, or a ground-contact state estimator that writes additional entries to ``observation``.

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

All design decisions have their pros and cons. Take a look at the features and non-features below to decide if Vulp is a fit to your use case.

### Features

- Run the same Python code on simulated and real robots
- Interface to the [mjbots pi3hat](https://mjbots.com/products/mjbots-pi3hat-r4-4b) and mjbots actuators
- Interface to the [Bullet](http://bulletphysics.org/) simulator
- Observer pipeline to extend observations
- 🏗️ Controller pipeline to extend actions
- Soft real-time: spine-agent loop interactions are predictable and repeatble

### Non-features

- Low frequency: Vulp is designed for tasks that run in the 1–400 Hz range (like balancing bipeds and quadrupeds)
- No hard real-time guarantee: the code is empirically reliable, that's it

### Alternatives

If any of these points is a no-go to you, you may also want to check out these existing alternatives:

* [mc\_rtc](https://github.com/jrl-umi3218/mc_rtc/) - C++ real-time control framework from which Vulp inherited, among others, the idea of running the same code on simulated and real robots. C++ controllers are bigger cathedrals to build but they can run at higher frequencies.
* [moteus](https://pypi.org/project/moteus/) - Python bindings for moteus brushless controllers also [run well up to 200 Hz](https://github.com/tasts-robots/vulp/blob/main/doc/loop_cycles.md#moteus-python-api), which makes them a simpler alternative if you don't need the simulation/real-robot switch.

## Getting started

Check out [``vulp_example``](https://github.com/tasts-robots/vulp_example) to get started with a minimal Python controller and Vulp spine.

## Q and A

> How can motion controllers be real-time in Python, with garbage collection and all?

Python agents talk with Vulp spines via the ``SpineInterface``, which can process both actions and observations in [about 0.7 ± 0.3 ms](doc/loop_cycles.md). This leaves plenty of room to implement other control components in a low-frequency loop. You may also be surprised at how Python performance has improved in recent years (most "tricks" that were popular ten years ago have been optimized away in modern 3.8+ versions of CPython). To consider one data point, here is the measured control period of a complete Python controller for Upkie (the pink balancer from [`upkie_locomotion`](https://github.com/tasts-robots/upkie_locomotion)) running on a Raspberry Pi 4 Model B (Quad core Cortex-A72 64-bit SoC @ 1.5GHz). It performs non-trivial tasks like balancing and whole-body inverse kinematics by quadratic programming:

<p align="center">
    <img src="https://user-images.githubusercontent.com/1189580/170681724-6b4eec14-779e-442a-969a-5fa053a457c0.svg" width="800">
</p>

Note that the 0.7 ± 0.3 ms processing time mentioned above runs on the Python side, and is therefore included in the 5.0 ms cycles represented by the orange curve above. Meanwhile the spine is set to a reference frequency of 1.0 kHz and its corresponding cycle period was measured here at 1.0 ± 0.05 ms.

> I just started a simulation spine but it's surprisingly slow, how come?

Make sure you switch Bazel's [compilation mode](https://bazel.build/reference/command-line-reference#flag--compilation_mode) to "opt" when running both robot experiments and simulations. The compilation mode is "fastbuild" by default. Note that it is totally fine to run agents in "fastbuild" during development while testing them with a spine that runs in "opt".

> Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably lightweight and quite reliable in their reaction times 😉
