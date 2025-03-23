# FlowStim

FlowStim is a personal project to build a lightweight fluids solver into an interactive application. The solver is implemented with the [Finite-Volume Method](https://www.cfd-online.com/Wiki/Finite_volume) in C++, and utilises the Qt framework to implement the user interface. The ultimate goal is to build an application which can be deployed on Android/IOS, and allow the user to explore flow physics through a few, low-cost interactive test cases (and also learn more C++ along the way).

This project was inspired by the 'coding adventure' of [Sebastian Lague](https://www.youtube.com/watch?v=rSKMYc1CQHE), in which the [Smooth Particle Hydrodynamic (SPH)](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics) method was implemented with C# to create a multiphase fluid renderer in Unity.

# The Physics

Whilst some background is provided here; the tutorial '12 Steps to Navier-Stokes' by [Lorena Barba](https://lorenabarba.com/blog/cfd-python-12-steps-to-navier-stokes/) is an excellent source for those unfamiliar with the general principles of Computational Fluid Dynamics. It provides a detailed step-by-step outline of a basic solver implementation in Python, using the mesh-based [Finite-Difference Method](https://www.cfd-online.com/Wiki/Finite_difference).

### Governing Equations

The momentum-conservation (Navier-Stokes) equations are the most critical governing equations in fluid mechanics. They relate the Advection, Diffusion, Pressure and Source terms together. Although they cannot be solved directly, there are a wide array of numerical methods for discretising the equations such that they can be solved iteratively. Collectively, these methods form the field of Computational Fluid Dynamics. 

<p align="center">
	<img src="https://www.quantamagazine.org/wp-content/uploads/2018/01/Navier-StokesEquation_560.jpg" alt="Logo" width="600"/>
</p>
*Image source: [Quanta](https://www.quantamagazine.org/what-makes-the-hardest-equations-in-physics-so-difficult-20180116/)*

### Finite-Volume Method

The Finite-Volume Method approaches this problem by first breaking down the singular flow domain into many thousands/millions of control volumes (Cells). Together these volumes (Cells) form the mesh of the domain. This is in contrast to the meshless [SPH](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics) approach which aims to model fluid as a collection of particles in a continuous domain. This is also in contrast to the mesh-based [Finite-Difference](https://www.cfd-online.com/Wiki/Finite_difference) method, where the domain is represented by a series of discrete points.

# Structure

The code has been divided into two (actually three) libraries; one library for the numerical modelling code (Core), and one library for the graphical user interface (App). There is also a new library for a potential alternative to the current user interface; this is the AppQML library, which implements a GUI using the more modern QtQuick framework.

The following table outlines all libraries that are currently implemented.

| Application | Library | Features | Status |
| --- |--- | --- | --- |
| Core | Domain | Builds and validates a mesh, which comprises of a collection of Cells, Faces, Vertices, and boundary sets, encapsulated within a Mesh class object. A MeshFactory class abstracts out all functionality for Mesh class construction. | Full functionality for creating a 2D structured mesh. |
|  | Field | Constructs and manages all variable fields (i.e. velocity and pressure), for the domain. A 'FieldEquation' class encapsulates all cell centre values and boundary conditions which must map to the Cells and boundaries in a corresponding Mesh class object. The 'FieldEquation' class also encapsulates all discretisation schemes for a specific field, which are implemented as components through composition. | Fully functions for constructing 2D velocity fields with 1st and 2nd order spatial discretisation, and a 1st order implicit temporal schemes. |
|  | Solver | Contains the algorithms (Solvers) for solving discretised fluid fields (FieldEquation). It also contains in-built point iteration methods for both explicit and implicit computations. | Currently functional for solving a discretisation of [Burgers' Equation](https://en.wikipedia.org/wiki/Burgers%27_equation#Complete_integral_of_the_inviscid_Burgers'_equation) (BurgersSolver), which represents a velocity field without the influence of pressure. Burgers' equation is a non-physical test case, which is currently being used to test drive the GUI libraries. |
| App | App | Implements the GUI with the QtWidgets framework. | |
| AppQML | AppQML | Implements a modern cross-platform GUI with the QtQuick framework. | Still in the exploratory spiking phase. |

The below image displays an example of the current GUI implementation for Burgers' equation. The corresponding mesh grid is displayed in the overlay window.

<img src="https://raw.githubusercontent.com/phil-taylor-sj/images/main/flowstim-gui-example.png" alt="Logo" width="1200"/>

# Installation

The installation has been tested and verified with QtWidgets on macOS.  
To set up the code, Qt must be installed on the host system. All other dependencies will be automatically retrieved/installed by the CMake build files.

```bash
# Clone
git clone https://github.com/phil-taylor-sj/FlowStim.git
cd FlowStim

# Install
mkdir build
cd build

# Build
cmake ../
cmake --build ./
```

Then run either the tests or the App by running the corresponding executable:

```
# Tests
./FlowStim_Tests

# App
./App/FlowStim
```
