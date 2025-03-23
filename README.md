# FlowStim

FlowStim is a personal project to build a lightweight fluids solver into an interactive application.

This project was inspired by the 'coding advanture' of [Sebastian Lague](https://www.youtube.com/watch?v=rSKMYc1CQHE). In that case, the [Smooth Particle Hydrodynamic (SPH)](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics) method was implemented with Unity and C# to create a multiphase fluid render.


### The Physics

The Finite-Volume Method is a mesh based approach to solving the momentum-conservation equationz for fluid flow. The domain is divided into a number of control volumes (Cells), which are connected through adjoining Faces.
The image source cannot be controlled and the forgotten

<p align="center">
	<img src="https://www.quantamagazine.org/wp-content/uploads/2018/01/Navier-StokesEquation_560.jpg" alt="Logo" width="600"/>
</p>
*Image source: [Quanta](https://www.quantamagazine.org/what-makes-the-hardest-equations-in-physics-so-difficult-20180116/)*

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
