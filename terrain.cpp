#include "terrain.h"
#include "glutwrapper.h"
#include "constants.h"
#include <cmath>

void DrawTerrain() {
    glColor3f(0.4f, 0.35f, 0.25f);
    glNormal3f(0.0f, 1.0f, 0.0f);

    float half = TERRAIN_SIZE * 0.5f;
    int steps = (int) (TERRAIN_SIZE / TERRAIN_STEP);

    for (int i = 0; i < steps; i++) {
        float x0 = -half + i * TERRAIN_STEP;
        float x1 = -half + (i + 1) * TERRAIN_STEP;
        for (int j = 0; j < steps; j++) {
            float z0 = -half + j * TERRAIN_STEP;
            float z1 = -half + (j + 1) * TERRAIN_STEP;

            glBegin(GL_TRIANGLES);
            glVertex3f(x0, TERRAIN_Y, z0);
            glVertex3f(x0, TERRAIN_Y, z1);
            glVertex3f(x1, TERRAIN_Y, z0);

            glVertex3f(x1, TERRAIN_Y, z0);
            glVertex3f(x0, TERRAIN_Y, z1);
            glVertex3f(x1, TERRAIN_Y, z1);
            glEnd();
        }
    }
}

