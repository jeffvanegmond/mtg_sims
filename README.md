# mtg_sims
A repository intended to use to build somewhat advanced simulations for MTG, similar to what Frank Karsten does in his articles. I (the author) noticed that he rewrites a lot of the same code, so my intent was to create a simple open source framework in c++ such that writing such simulations becomes a lot easier.

This project consists of a library (sims_lib directory) and all the simulations I've written so far (sims directory).

# Library
The mtg_sims library consists of the necessary tools to conduct a simulation. It has classes for cards, a decklist, a game, the different game zones. It also has a basic Timer class that you can use to time your simulations and a Simulation class that is effectively a simulation stub. The simulation class stores requires a behavior implementation, which returns a Result that will be incrementally stored.

# Examples
Please look in the sims directory for examples (draw_singleton.cpp is the simplest). They currently all use the structure that the Simulation class was made for. If you want to do more advanced things, you can always copy the simulate function to your own code, so that you have better access to whatever you need for simulation.
For an idea on how to integrate more statistical analysis, please look at the `burn_fetch` example, where the incremental nature of the Result struct is used to incrementally calculate the variance, as well as store a histogram. In the future I will see if I can generate outputs in a structure more suited for statistical analysis. 

# Parallellization
Parallellization can be done through the Simulation::simulateParallel method. OpenMP is used for the parallellization. If OpenMP is not available, this will call the regular simulate method. Do take note that on the author's machine, the parallel simulations for the DrawSingleton example are actually slower in release mode than the non-parallel simulations. This is likely due to an unfortunate design choice, but I (the author) am not educated in parallel programming, so deal with it...

# Contributions
You are welcome to fork this repository and contribute to it through pull requests. I will try to incorporate any improvements, but I would also really like all changes to be backwards compatible, or receive updates on them such that they behave the same with your contributions.
