# RocketSim

This is a project that aims to dive into:
1. Aerodynamic simulation
2. Rocket control systems (maybe machine learning?)
3. OpenGL (maybe even OpenCL we'll see)

As well as brush up my systems programming and learn some C!

## Components

The project has 3 components:
1. Simulator. This handles the physics of the system.
2. Controller. This is the rocket pilot.
3. Visualizer. I'll draw some panes to show where the rocket is, how it's moving, etc.

Each of which can be launched separately or the whole thing can be launched as one hulking process through the launch module.
This module is responsible for:
- Launching the sim, controller, and visualizer.
- Monitoring the child processes & automatically restarting or logging the failure.
- Cleanup after everything is done.

## Notes

See the `docs/` directory for more documentation.

## Compiling

Nothing yet...

## Running

Launch the `rocketsim` binary to open everything. Otherwise launch the `simulator`, `controller` and `visualizer` binaries separately.
