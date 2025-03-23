# FlowStim

FlowStim is a personal project to build a lightweight fluids solver into an interactive application.

This project was inspired by the 'coding advanture' of [Sebastian Lague](https://www.youtube.com/watch?v=rSKMYc1CQHE). In that case, the [Smooth Particle Hydrodynamic (SPH)](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics) method was implemented with Unity and C# to create a multiphase fluid render.

### The Physics?

The Finite-Volume Method is a mesh based approach to solving the momentum-conservation equation for fluid flow. The domain is divided into a number of control volumes (Cells), which are connected through adjoinitng Faces. 

### Organisation



### Installation

```
#Clone
git clone https://github.com/phil-taylor-sj/FlowStim.git
cd FlowStim

# Install
mkdir build
cd build

# Build
cmake ../
cmake --build ./
```

Then run either the tests or the App by running the corresponding executable.
```
#Tests
./FlowStim_Tests

#App
./App/FlowStim
```
