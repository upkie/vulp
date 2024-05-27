# Vulp – Robot/simulation switch

[![CI](https://github.com/upkie/vulp/actions/workflows/ci.yml/badge.svg)](https://github.com/upkie/vulp/actions/workflows/ci.yml)
[![Documentation](https://img.shields.io/github/actions/workflow/status/upkie/vulp/docs.yml?branch=main&label=docs)](https://upkie.github.io/vulp/)
[![Coverage](https://coveralls.io/repos/github/upkie/vulp/badge.svg?branch=main)](https://coveralls.io/github/upkie/vulp?branch=main)
![C++ version](https://img.shields.io/badge/C++-17-blue.svg?style=flat)
[![Conda version](https://img.shields.io/conda/vn/conda-forge/vulp.svg)](https://anaconda.org/conda-forge/vulp)
[![PyPI version](https://img.shields.io/pypi/v/vulp)](https://pypi.org/project/vulp/)

Vulp provides an action-observation loop to control robots from a standalone "agent" process, like this:

<p align="center">
<img src="https://raw.githubusercontent.com/upkie/vulp/main/docs/figures/action-observation-loop-full.svg" alt="Action-observation loop with Vulp" class="center"/>
</p>

The agent can be a simple Python script with few dependencies.

Vulp is designed for robots built with the mjbots stack ([moteus](https://github.com/mjbots/moteus/) servo controllers and [pi3hat](https://github.com/mjbots/pi3hat/) communication board). It provides a robot/simulation switch to train or test agents in [Bullet](https://github.com/bulletphysics/bullet3) before running them on the real system.

Vulp supports Linux and macOS for development, and Raspberry Pi OS for robot deployment.

## Installation

### From conda-forge

```console
conda install -c conda-forge vulp
```

### From PyPI

```console
pip install vulp
```

## Example

Check out the [``upkie``](https://github.com/upkie/upkie) repository for an example where Vulp is used to implement simulation environments, real-robot spines, state observers and locomotion agents.

## Details

More accurately, Vulp is a tiny inter-process communication (IPC) protocol shipped with reference libraries (currently in Python and C++, other languages welcome). It is suitable for tasks that require [real-time](https://en.wiktionary.org/wiki/real-time#English) but *not* high-frequency performance. The main use case for this is balancing, as there is [theoretical and empirical evidence](https://arxiv.org/abs/1907.01805) suggesting that bipeds and quadrupeds can balance themselves as leisurely as 5–15 Hz, although balance control is frequently implemented at 200–1000 Hz. And if you are wondering whether Python is suitable for real-time applications, we were too! Until we [tried it out](#performance).

In Vulp, a fast program, called a _spine_, talks to a slow program, called an _agent_, in a standard action-observation loop. Spine and agent run in separate processes and exchange ``action`` and ``observation`` dictionaries through shared memory. For instance, ``action`` can be a set of joint commands and ``observation`` a set of joint observations. Vulp provides a pipeline API to grow more complex spines with additional controllers (for higher-level actions) and observers (for richer observations). For example, a spine can run an inverse kinematics solver, or output its own ground contact estimation.

### Features and non-features

All design decisions have their pros and cons. Take a look at the features and non-features below to decide if Vulp is a fit to your use case.

#### Features

- Run the same Python code on simulated and real robots
- Interfaces with to the [mjbots pi3hat](https://mjbots.com/products/mjbots-pi3hat-r4-4b) and mjbots actuators
- Interfaces with to the [Bullet](http://bulletphysics.org/) simulator
- Observer pipeline to extend observations
- 🏗️ Controller pipeline to extend actions
- Soft real-time: spine-agent loop interactions are predictable and repeatable
- Unit tested, and not only with end-to-end tests

#### Non-features

- Low frequency: Vulp is designed for tasks that run in the 1–400 Hz range (like balancing bipeds or quadrupeds)
- Soft, not hard real-time guarantee: the code is empirically reliable by a large margin, that's it
- Weakly-typed IPC: typing is used within agents and spines, but the interface between them is only checked at runtime

### Alternatives

If any of the non-features is a no-go to you, you may also want to check out these existing alternatives:

* [kodlab_mjbots_sdk](https://github.com/KodlabPenn/kodlab_mjbots_sdk) - C++-only framework integrated with [LCM](https://lcm-proj.github.io/lcm/) for logging and remote I/O. Still a work in progress, only supports torque commands as of writing this note.
* [mc\_rtc](https://github.com/jrl-umi3218/mc_rtc/) - C++ real-time control framework from which Vulp inherited, among others, the idea of running the same code on simulated and real robots. The choice of a weakly-typed dictionary-based IPC was also inspired by mc\_rtc's data store. C++ controllers are bigger cathedrals to build but they can run at higher frequencies.
* [robot\_interfaces](https://github.com/open-dynamic-robot-initiative/robot_interfaces) - Similar IPC between non-realtime Python and real-time C++ processes. The main difference lies in the use of Python bindings and action/observation types (more overhead, more safeguards) where Vulp goes structureless (faster changes, faster blunders). Also, robot\_interfaces enforces process synchronization with a [time-series API](https://people.tuebingen.mpg.de/mpi-is-software/robotfingers/docs/robot_interfaces/doc/timeseries.html) while in Vulp this is up to the agent (most agents act greedily on the latest observation).
* [ros2_control](https://github.com/ros-controls/ros2_control) - A C++ framework for real-time control using ROS2 (still a work in progress). Its barrier of entry is steeper than the other alternatives, making it a fit for production rather than prototyping, as it aims for compatibility with other ROS frameworks like [MoveIt](https://moveit.ros.org/). A Vulp C++ spine is equivalent to a ROS ``ControllerInterface`` implementing the dictionary-based IPC protocol.

If your robot is built with some of the following open hardware components, you can also use their corresponding Python bindings directly:

* [moteus](https://pypi.org/project/moteus/) - bindings for moteus brushless controllers also [run well up to 200 Hz](https://github.com/upkie/vulp/blob/main/docs/loop_cycles.md#moteus-python-api).
* [odri_control_interface](https://github.com/open-dynamic-robot-initiative/odri_control_interface) - interface to control robots built with the [ODRI](https://github.com/open-dynamic-robot-initiative) Master Board.

Using control bindings directly is a simpler alternative if you don't need the action-observation loop and simulation/real-robot switch from Vulp.

## Q and A

### Performance

#### How can motion control be real-time in Python, with garbage collection and all?

Python agents talk with Vulp spines via the ``SpineInterface``, which can process both actions and observations in [about 0.7 ± 0.3 ms](docs/loop_cycles.md). This leaves plenty of room to implement other control components in a low-frequency loop. You may also be surprised at how Python performance has improved in recent years (most "tricks" that were popular ten years ago have been optimized away in CPython 3.8+). To consider one data point, here are the cycle periods measured in a complete Python agent for Upkie (the Pink balancer from [`upkie`](https://github.com/upkie/upkie)) running on a Raspberry Pi 4 Model B (Quad core ARM Cortex-A72 @ 1.5GHz). It performs non-trivial tasks like balancing and whole-body inverse kinematics by quadratic programming:

<p align="center">
    <img src="https://user-images.githubusercontent.com/1189580/172820003-ade5aee1-fdca-41d7-958a-baf397a2caa3.png" width="800">
</p>

Note that the aforementioned 0.7 ± 0.3 ms processing time happens on the Python side, and is thus included in the 5.0 ms cycles represented by the orange curve. Meanwhile the spine is set to a reference frequency of 1.0 kHz and its corresponding cycle period was measured here at 1.0 ± 0.05 ms.

#### I just started a simulation spine but it's surprisingly slow, how come?

Make sure you switch Bazel's [compilation mode](https://bazel.build/reference/command-line-reference#flag--compilation_mode) to "opt" when running both robot experiments and simulations. The compilation mode is "fastbuild" by default. Note that it is totally fine to compile agents in "fastbuild" during development while testing them on a spine compiled in "opt" that keeps running in the background.

#### I have a Bullet simulation where the robot balances fine, but the agent repeatedly warns it "Skipped X clock cycles". What could be causing this?

This happens when your CPU is not powerful enough to run the simulator in real-time along with your agent and spine. You can call [`Spine::simulate`](https://upkie.github.io/vulp/classvulp_1_1spine_1_1Spine.html#a886ef5562b33f365d86e77465dd86204) with ``nb_substeps = 1`` instead of ``Spine::run``, which will result in the correct simulation time from the agent's point of view but make the simulation slower than real-time from your point of view.

#### I'm running a pi3hat spine, why are my timings more erratic than the ones plotted above?

Make sure you configure CPU isolation and set the scaling governor to ``performance`` for real-time performance on a Raspberry Pi.

### Design choices

#### Why use dictionaries rather than an [interface description language](https://en.wikipedia.org/wiki/Interface_description_language) like Protocol Buffers?

Interface description languages like Protocol Buffers are strongly typed: they formally specify a data exchange format that has to be written down and maintained, but brings benefits like versioning or breaking-change detection. Vulp, on the other hand, follows a weakly-typed, self-describing approach that is better suited to prototyping with rapidly-changing APIs: the spec is in the code. If an agent and spine communicate with incompatible/incomplete actions/observations, execution will break, begging for developers to fix it.

#### Why the weakly-typed dictionary IPC rather than Python–C++ bindings?

Vulp is designed for prototyping: it strives to eliminate intermediaries when it can, and keep a low barrier of entry. Python bindings bring the benefits of typing and are a good choice in production contexts, but like interface description languages, they also add overhead in terms of developer training, bookkeeping code and compilation time. Vulp rather goes for a crash-early approach: fast changes, fast blunders (interface errors raise exceptions that end execution), fast fixes (know immediately when an error was introduced).

#### Is it possible to run two agents at the same time?

That is not possible. One of the core assumptions in Vulp is that the agent and the spine are two respective processes communicating via one single shared-memory area. In this Vulp differs from *e.g.* ROS, which is multi-process by design. This design choice is discussed in [#55](https://github.com/upkie/vulp/discussions/55).

#### Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably quite reliable in their reaction times 🦊
