# FlowStim

FlowStim is a personal project to build a lightweight fluids solver into an interactive application.

This project was inspired by the 'coding advanture' of [Sebastian Lague](https://www.youtube.com/watch?v=rSKMYc1CQHE). In that case, the [Smooth Particle Hydrodynamic (SPH)](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics) method was implemented with Unity and C# to create a multiphase fluid render.

For this project, the Finite-Volume methods is implemented in C++, and developed into an interactive application using the Qt framework.

### The Physics

The tutorial '12 Steps to Navier-Stokes' [Lorena Barber](https://lorenabarba.com/blog/cfd-python-12-steps-to-navier-stokes/) is an excellent source for those unfamiliar with the general principles of Computational Fluid Dynamics. It provides a detailed step-by-step outline a basic solver implementation in Python, using the mesh-based Finite-Difference Method.

## Governing Equations

The momentum-conservation (Navier-Stokes) equations are the most critical governing equations in fluid mechanics. They relate the Advaction, Diffusion, Pressure and Source terms together. Although they cannot be solved directly, there are a wide array of numerical methods for discretising the euqations such that they can be solved iteratively. Collectively, these methods form the field fo Computational Fluid Mechanics. 

<p align="center">
	<img src="https://www.quantamagazine.org/wp-content/uploads/2018/01/Navier-StokesEquation_560.jpg" alt="Logo" width="600"/>
</p>
*Image source: [Quanta](https://www.quantamagazine.org/what-makes-the-hardest-equations-in-physics-so-difficult-20180116/)*

## Finite-Volume Method

The Finite-Volume Method approaches this problem by first breaking down the singluar flow domain into many thousands/millions of control volumes (Cells). Together these volumes (Cells) form the mesh of the domain. This is in contrast to the meshless SPH approach (see above) which aims to model fluid as a collection of particles in a continous domain. 

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
