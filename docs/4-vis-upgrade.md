# Visualizer Upgrade
Dec. 23, 2025

I think upgrade is a strong word. Really I'm implementing everything.

At Autopilot my team does something called "criticisms" which I've found really naturally allow for development. I'll be doing the same thing here.

## Criticisms

There are two things that I identify as areas of improvement, as well as 1 issue I purposely left unfixed that I need to get back to:
- There is no rocket! Right now I'm just drawing a triangle. Let's see if we can get a plane for the ground, a cylinder for the rocket, and a light source.
- Not visualizing the SceneFrame shm data. Let's see if we can get text drawn in another window or tab or something.
- (bug) No support for Mac or Windows in the launcher yet. Right now this is down to only the file issue. The parent process needs to find the other 3 executables. The process by which I do that now is Linux specific.

## Design (further thought)

For drawing the rocket...

The vis right now only has primitives. I'll need to learn how to generate meshes for cylinders and planes (simular to my triangle module) and draw them out.
Also my shaders (if we can even call them that) are extraordinarily primitive. It's time we give them a major upgrade (Phong shader time).

SceneFrame data...

I'll need to do some research on how easily I can get text up in my OpenGL context. If it's not feasible I'll just create a second window with more... robust text support.

## Versioning

At some point it'll be a good idea to start versioning this. I'll version all the components under one single number and maybe separate them in the future if it looks like they should be.

For now I say we're just working towards a v0.1. I think a v0.1 is one that has:
- [ ] A simulated world with gravity, rigid bodies, collisions, and the ability to apply forces and torques on bodies (particularly the rocket).
- [ ] A visualizer that shows the "rocket" in space, moving around.
- [ ] A controller that let's me enter commands into the rocket like "engine [on/off]" or "gimbal [pitch] [yaw]"

Then I'll either work on upgrades to the visualizer/sim OR start implementing some kind of "rocket landing" controller. Let's call this latter option v0.2 of the controller.