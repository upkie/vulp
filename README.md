# Vulp

Real-time motion control from Python

> [real-time](https://en.wiktionary.org/wiki/real-time#English): of a system that responds to events or signals within a predictable time after their occurrence; specifically the response time must be within the maximum allowed, but is typically synchronous.

* Real-time does not mean *high frequency*. Many meaningful tasks in robotics do not require 1 kHz, hard real-time guarantees. Balancing a full-fledged humanoid robot can be performed as slow as 5 Hz (200 ms), which is a remarkable [theoretical and empirical result](https://arxiv.org/pdf/1907.01805.pdf).
