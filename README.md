# Arduino Hexapod Robot

![Hexapod](/img/walking.gif)

This is an Arduino based hexapod robot. The main goal of the project was coming up with a control system that would allow smooth transitions between gaits.

### Project Structure
- **Console** is a Python app that allows to configure the robot over wifi using websockets. It comes handy as the robot uses a *lot* of different settings (servo calibration offsets, oscillators, ...).

- **Joystick** is the source code of the  **Arduino Nano** based radio controller used to control the robot's direction.

- **Hexapod** is the source code of the robot itself, including the Central Pattern Generator, the servos controller, the websocket client, ...

### Hardware

The robot was built using an **Afinibot 3D printer**.

![3D Printing](/img/printing.png)

Pieces were designed using **OpenSCAD**, except for the shoulders which were grabbed here: [Mini Quadruped - optimized for 9G servos](http://www.thingiverse.com/thing:38159).


The microcontroller is a **Wemos D1** board which is a Wifi enabled Arduino compatible board. This way the robot's configuration can be modified on runtime using websockets and JSON.

### Prototyping

The robot's locomotion control was prototyped using **Unity**. The simulation source code can be found here: [UnityHexapodSimulator](https://github.com/etienne-p/UnityHexapodSimulator), and a demo video is available on [youtube](https://www.youtube.com/watch?v=RsYVKNpakKs).

![Unity Simulation](/img/simulation.gif)

### Locomotion Control

The locomotion control is based on a [Central Pattern Generator](https://en.wikipedia.org/wiki/Central_pattern_generator). Coupled oscillators using the [kuramoto model](https://en.wikipedia.org/wiki/Kuramoto_model) generate signals used to control the servos.
