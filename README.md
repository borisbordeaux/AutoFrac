# AutoFrac

Software for CAD of lacunar fractal structures on Linux

## How to build

You need to install some dependencies:
```bash
sudo apt install libglfw3-dev libglew-dev libgl-dev libfmt-dev libspdlog-dev libarmadillo-dev liblua5.4-dev 
```
Then you can build this project.
Replace `{NB_CORES}` by the number of cores to use to compile the project.
```bash
git clone https://github.com/borisbordeaux/AutoFrac.git
cd AutoFrac
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j {NB_CORES}
```

## How to use

The main pipeline is:
- write a script defining the topology of a fractal structure
- import it in the software to visualize the fractal

### The script

The script must be written in Lua.
The fractal topology has to be described using the BC-IFS model.
Some example files are provided inside the `res/script/` folder.
Here follows all the available Lua functions relative to the BC-IFS model:
```Lua
-- create a state with a name and an internal dimension
state(stateName, internalDimension)
```
```Lua
-- create a boundary with a name from a state to another
boundary(boundaryName, fromStateName, toStateName)
```
```Lua
-- create a subdivision with a name from a state to another
-- it is possible to specify some colors associated to this subdivision
subdivision(subdivisionName, fromStateName, toStateName [, frontColor[, backColor]])
```
```Lua
-- create a permutation with a name from a state to another
-- (most of the time, permutation doesn't change the state)
permutation(permutationName, fromStateName, toStateName)
```
```Lua
-- create a grid for a state with an array of figures
-- a figure is an array of paths
-- a path is an array of transitions
-- each array of paths is considered as a polyline where:
--  - each line gives a spring
--  - each paths gives a mass
grid(stateName, figuresArray)

-- it's also possible to create a grid from the boundary automatically
-- in this case, the grid of each boundary state should have been defined
-- with either grid() or gridFromBoundary() function
gridFromBoundary(stateName)
```
```Lua
-- define a space for a state with an array of boundary or internal transitions
space(stateName, arrayOfBoundaryOrInternalTransitions)
```
```Lua
-- define a primitive for a state with an array of figures
-- a figure is an array of paths
-- a path is an array of transitions
-- each figure is considered as a face where:
--  - each path is a point of the face
--  - the points are interpreted in CCW order
primitive(stateName, figuresArray)
```
```Lua
-- define a constraint for a state between two paths starting from that state
-- the ending state of each path should be the same to have a valid constraint
constraint(stateName, firstPath, secondPath)
```
```Lua
-- define an initial matrix for a subdivision transition of a state
-- the matrix is defined with an array of array of numbers, in row-major order
-- the type must be either 'VAR' or 'CONST'
initMat(stateName, subdivisionName, matrix, type)
```