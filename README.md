# Computer Graphics - Term Project (Flight simulator) 

OpenGL flight simulator following the exercise assignment.
The model of an airplane flies over an endless, textured landscape.

## Controls
 
- The speed can be reduced or increased (using `w` and `s` keys), but ultimately the plane will always go forward.
- The plane can be steered left, right, up and down (using arrows). It makes an attempt to mimic a real plane.
- The camera zoom can be adjusted (using `+` and `-` keys).
- Camera can be controlled and roatated 360 degrees around the plane using the mouse.

## Structure

The project uses the CMake layout, same as in the labs.

There was an attempt to organised the code by feature (to have one `.cpp`/`.h` pair per entity).
(Though ultimately there is a lot of procedural code that doesn't look very easy to maintain.)

All the external dependencies are put into the `deps` directory.

All models/textures live in the `models` directory.

The GLUT import is in its own header file (`deps/glutwrapper.h`) to support the MacOS specifics (the import is called `GLUT/glut.h` on MacOS and `GL/glut.h` on Linux). 

- `main.cpp` does only GLUT init and callback registration
- `visuals.cpp` maintains the render loop and scene setup
- `constants.h` holds all parameter values
- `plane` is the airplane module
  - loads `plane.obj` (that I found on the internet - [here](https://free3d.com/3d-model/wwii-plane-japan-kawasaki-ki61-v1--150484.html); it came with textures)
  - I used [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) to load the object
  - the propeller (shape 0 of the object) is rotated continuously about its centre, which is calculated at load time (average of the verts)
- `terrain` is the height field module, it produces the landscape, generated using Perlin noise (using [stb_perlin](https://github.com/nothings/stb))
- `texture` is the texture loading util
  - JPEG, using [stb_image](https://github.com/nothings/stb) (used for the plane)
  - BMP, using the `BMPLoader` util from the labs (used for the ground)
- `camera` is the camera module

## Setup

The terrain is a grid (cells of 128 x 128 size) covering a world window 1000 x 1000 size.
The height field is a function (Perlin noise seeded by `TERRAIN_SEED`, which is kept static to make the height field reproducible, without persistence).

As the plane flies, the grid window re-centres and the heights are re-sampled from the noise function.
This is done after the plane crosses `TERRAIN_RECENTRE_CELLS` number of cells, to avoid rebuilding on every frame

The ground texture (`terrain.bmp`) is tiled all over the grid with `GL_REPEAT`.

The plane maintains a position, speed, yaw, pitch and roll (can never memorise which one is which), expressed by respective variables in the code.
The plane constantly moves at the set speed, changing its position on the landscape. 
`UpdatePlane()` advances it along the nose, taking the other variables into the calculation.

The propeller spins continuously, regardless of the speed setting.

`glFog` is coloured grey-ish to blend with anything.
