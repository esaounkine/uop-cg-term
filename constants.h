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
#define PLANE_SPEED 2.0f
#define PLANE_SPEED_MIN 0.5f
#define PLANE_SPEED_MAX 8.0f
#define PLANE_SPEED_STEP 0.5f
#define PLANE_TURN_STEP 2.0f
#define PLANE_PITCH_STEP 2.0f
#define PLANE_PITCH_LIMIT 60.0f // prevent from going vertical
#define PLANE_BANK_ANGLE 25.0f
#define PLANE_ROLL_DECAY 0.95f // how quickly the plane returns to level after turning: 1.0 = never, 0.0 = instantly

#define FOG_R 0.85f
#define FOG_G 0.88f
#define FOG_B 0.92f
#define FOG_START 250.0f
#define FOG_END (TERRAIN_SIZE / 2)

#define CAM_DIST 150.0f
#define CAM_HEIGHT 50.0f
#define CAM_MOUSE_SENS 0.3f
#define CAM_PITCH_LIMIT 80.0f
#define CAM_ZOOM_STEP 0.1f
#define CAM_ZOOM_MIN 0.3f
#define CAM_ZOOM_MAX 3.0f

#endif
