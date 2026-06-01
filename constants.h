#ifndef CONSTANTS_H
#define CONSTANTS_H


#define PI 3.14159265f
#define DEG2RAD (PI / 180.0f)

#define TERRAIN_SIZE 1000.0f
#define TERRAIN_GRID 128
#define TERRAIN_STEP (TERRAIN_SIZE / TERRAIN_GRID)
#define TERRAIN_Y (-100.0f)
#define TERRAIN_HEIGHT_SCALE 150.0f
#define TERRAIN_SEED 1
#define TERRAIN_VERTS (TERRAIN_GRID + 1)
#define TERRAIN_TILES 10.0f
#define TERRAIN_NOISE_SCALE 250.0f
#define TERRAIN_OCTAVES 4
#define TERRAIN_RECENTRE_CELLS 8 // rebuild the field after moving this many cells

#define PLANE_START_Y 150.0f

#define CAM_DIST   150.0f
#define CAM_HEIGHT  50.0f
#define CAM_MOUSE_SENS  0.3f
#define CAM_PITCH_LIMIT 80.0f
#define CAM_ZOOM_STEP   0.1f
#define CAM_ZOOM_MIN    0.3f
#define CAM_ZOOM_MAX    3.0f

#endif

