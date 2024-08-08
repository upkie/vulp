# Vulp – Robot/simulation switch

![C++ version](https://img.shields.io/badge/C++-17-blue.svg?style=flat)
[![Coverage](https://coveralls.io/repos/github/upkie/vulp/badge.svg?branch=main)](https://coveralls.io/github/upkie/vulp?branch=main)
[![Documentation](https://img.shields.io/github/actions/workflow/status/upkie/vulp/docs.yml?branch=main&label=docs)](https://upkie.github.io/vulp/)

> [!IMPORTANT]\
> Vulp has been merged into the software of [Upkie wheeled bipeds](https://github.com/upkie/upkie) and is not developed as a standalone library any more. If you need similar features for your own robot, we recommend you rather start from the latest [moteus](https://pypi.org/project/moteus/) C++ bindings.

Vulp provides an action-observation loop to control robots from a standalone "agent" process, like this:

<p align="center">
<img src="https://raw.githubusercontent.com/upkie/vulp/main/docs/figures/action-observation-loop-full.svg" alt="Action-observation loop with Vulp" class="center"/>
</p>

Vulp is designed for robots built with the mjbots stack ([moteus](https://github.com/mjbots/moteus/) servo controllers and [pi3hat](https://github.com/mjbots/pi3hat/) communication board). It provides a robot/simulation switch to train or test agents in [Bullet](https://github.com/bulletphysics/bullet3) before running them on the real system. Vulp supports Linux and macOS for development, and Raspberry Pi OS for robot deployment.

## See also

* [moteus](https://pypi.org/project/moteus/): bindings for moteus brushless controllers.
* [odri_control_interface](https://github.com/open-dynamic-robot-initiative/odri_control_interface): interface to control robots built with the [ODRI](https://github.com/open-dynamic-robot-initiative) Master Board.
* [kodlab_mjbots_sdk](https://github.com/KodlabPenn/kodlab_mjbots_sdk): C++-only framework integrated with [LCM](https://lcm-proj.github.io/lcm/) for logging and remote I/O.
* [mc\_rtc](https://github.com/jrl-umi3218/mc_rtc/): C++ real-time control framework with QP-based whole-body control.
* [robot\_interfaces](https://github.com/open-dynamic-robot-initiative/robot_interfaces): IPC between non-realtime Python and real-time C++ processes with Python bindings.
* [ros2_control](https://github.com/ros-controls/ros2_control): C++ framework for real-time control using ROS2 (still a work in progress).

## Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably quite reliable in their reaction times 🦊
