On a Raspberry Pi 4 Model B with a pi3hat connected to 6 servos (2 buses, 3 servos per bus), we measure how long it takes a transport cycle to send stop commands to and query observations from all servos, using (1) the moteus Python API and (2) a Vulp spine.

## moteus Python API

Code without time measurements:

```python
import moteus
import moteus_pi3hat

async def main(frequency: int = 200):
    transport = moteus_pi3hat.Pi3HatRouter(
        servo_bus_map={
            1: [1, 2, 3],
            2: [4, 5, 6],
        },
    )
    servos = {
        servo_id: moteus.Controller(id=servo_id, transport=transport)
        for servo_id in [1, 2, 3, 4, 5, 6]
    }
    rate = aiorate.Rate(frequency)
    for i in range(10_000):
        commands = [c.make_stop(query=True) for c in servos.values()]
        await transport.cycle(commands)
        await rate.sleep()
```

This code runs at around 2.5 ms per cycle on the Pi. That is the expected performance, as other quadrupeds reported running at 200 Hz with 12 servos using this API.

## Vulp API

We run the same test with the pi3hat spine from [`upkie_locomotion`](https://github.com/tasts-robots/upkie_locomotion/). Code without time measurements:

```python
import vulp.spine

async def main(spine: vulp.spine.SpineInterface, frequency: int = 200):
    rate = aiorate.Rate(frequency)
    for i in range(10_000):
        spine.get_observation()
        spine.set_action({})
        await rate.sleep()
```

This loop runs at 0.7 ± 0.3 ms (average and standard deviation over 10,000 cycles).

Note that we observe the same outcome, to the digit, for ``frequency in [50, 100, 200, 400]``; however, performance on the Pi degrades to 0.9 ± 0.4 ms for ``frequency = 500`` Hz. This is why we rate Vulp for frequencies up to 400 Hz.
