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

## Example

Check out the [``upkie``](https://github.com/upkie/upkie) repository for an example where Vulp is used to implement simulation environments, real-robot spines, state observers and locomotion agents.

## Q and A

### Design choices

#### Why use dictionaries rather than an [interface description language](https://en.wikipedia.org/wiki/Interface_description_language) like Protocol Buffers?

Interface description languages like Protocol Buffers are strongly typed: they formally specify a data exchange format that has to be written down and maintained, but brings benefits like versioning or breaking-change detection. Vulp, on the other hand, follows a weakly-typed, self-describing approach that is better suited to prototyping with rapidly-changing APIs: the spec is in the code. If an agent and spine communicate with incompatible/incomplete actions/observations, execution will break, begging for developers to fix it.

#### Why the weakly-typed dictionary IPC rather than Python–C++ bindings?

Vulp is designed for prototyping: it strives to eliminate intermediaries when it can, and keep a low barrier of entry. Python bindings bring the benefits of typing and are a good choice in production contexts, but like interface description languages, they also add overhead in terms of developer training, bookkeeping code and compilation time. Vulp rather goes for a crash-early approach: fast changes, fast blunders (interface errors raise exceptions that end execution), fast fixes (know immediately when an error was introduced).

#### Is it possible to run two agents at the same time?

That is not possible. One of the core assumptions in Vulp is that the agent and the spine are two respective processes communicating via one single shared-memory area. In this Vulp differs from *e.g.* ROS, which is multi-process by design. This design choice is discussed in [#55](https://github.com/upkie/vulp/discussions/55).

#### Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably quite reliable in their reaction times 🦊
