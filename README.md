# Orbital Hauler

The orbiter spaceflight simulator has quite a few highly detailed and well-simulated space planes, like the DG IV, the XR-series, or the Starliner.
It does not have any purely spaceborn vessels for orbital operations of similar quality, however, so this project aims at creating one.

It's still in its infancy, though. Right now it's *literelly* a tin can with thrusters and a dockport...

## Build instructions
The project is written using Visual Studio 2019 Express. If you're using an older version, there might be issues.

Clone this repository into your Orbitersdk folder. *Not* the samples folder, if you don't want to adjust all the paths!

Clone the dependencies required for building it, also into your Orbitersdk folder:
* Olog: https://github.com/TheNewBob/Olog : `git clone https://github.com/TheNewBob/Olog.git`

After cloning the project and its dependencies into the correct folders, the project *should* build and run.
There's debug configurations for MOGE as well as D3D9 client (works also for other graphics clients, obviously).

## Notes on folder structure
The project uses a folder structure to separate its files, similar to Java packages. In order to work with it correctly, you need to turn on "show all files" in the solution explorer.
You'll also have to include the path relative to the projects root when including header files.
There is an orbiter folder, which should be used for any orbiter related files like the cfg, so they're contained in the repository. 
There's no automation to copy these into their proper orbiter folders, becuase I have no idea how to do that in visual studio.