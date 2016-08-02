# mtg_sims
A repository intended to use to build somewhat advanced simulations for MTG, similar to what Frank Karsten does in his articles. I (the author) noticed that he rewrites a lot of the same code, so my intent was to create a simple open source framework in c++ such that writing such simulations becomes a lot easier.

This project consists of a library (sims_lib directory) and all the simulations I've written so far (sims directory).

# Library
The mtg_sims library consists of the necessary tools to conduct a simulation. It has classes for cards, a decklist, a game, the different game zones. It also has a basic Timer class that you can use to time your simulations and a Simulation class that is effectively a simulation stub.

# Examples
Please look in the sims directory for examples (draw_singleton.cpp is the simplest). They currently all use the structure that the Simulation class was made for. If you want to do more advanced things (eg: parallellization for faster simulations), I recommend copying the simulate function to your own code, so that you have better access to whatever you need for simulation.

# Parallellization
Parallellization is intended, but for the simple DrawSingleton it actually decreased performance by a factor two, so for now I've disabled the setting of the OpenMP flags in CMake. It is most likely due to my own inexperience with OpenMP that I got a performance loss, so if you know what I'm doing wrong, please enlighten me.
