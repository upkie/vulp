# Vulp

Motion control in Python, lightweight and real-time.

> [real-time](https://en.wiktionary.org/wiki/real-time#English): of a system that responds to events or signals within a predictable time after their occurrence; specifically the response time must be within the maximum allowed, but is typically synchronous.

* Real-time does not mean *high frequency*. Many meaningful tasks in robotics do not require 1 kHz, hard real-time guarantees. Balancing a full-fledged humanoid robot can be performed as slow as 5 Hz (200 ms), which is a remarkable [theoretical and empirical result](https://arxiv.org/pdf/1907.01805.pdf).

## Q and A

> Why the name "Vulp"?

[Vulp](https://en.wiktionary.org/wiki/vulp#Noun_2) means "fox" in Romansh, a language spoken in the Swiss canton of the Grisons. Foxes are arguably lightweight and quite reliable in their reaction times 😉

> How can it be real-time in Python, with garbage collection and all?

Most of Vulp's code is written in C++ and runs at a reliable rate. The interface it exposes in Python is quite thin and runs at a reliable rate as well, up to the kilohertz at least. Now, Vulp cannot guarantee that the code on your side is real-time, but you may be surprised at how reliably operations such as neural network inference or quadratic programming can run even in pure Python. Here is for example the measured control period of a complete Python controller for [Upkie](https://www.youtube.com/watch?v=NO_TkHGS0wQ) including balancing and whole-body inverse kinematics:

[[[PLOT HERE]]]
