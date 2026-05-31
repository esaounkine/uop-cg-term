#include "terrain.h"
#include "glutwrapper.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// height field is a grid of vertices
// heights and normals computed in InitTerrain()
// triangles emitted on each frame by DrawTerrain()
static float heights[TERRAIN_VERTS][TERRAIN_VERTS];
static float normals[TERRAIN_VERTS][TERRAIN_VERTS][3];

static float Rand() {
    return (float) rand() / RAND_MAX;
}

static void Vertex(int i, int j) {
    float half = TERRAIN_SIZE * 0.5f;
    float x = -half + i * TERRAIN_STEP;
    float z = -half + j * TERRAIN_STEP;
    glNormal3fv(normals[i][j]);
    glVertex3f(x, TERRAIN_Y + heights[i][j], z);
}

void InitTerrain() {
    // fixed seed is left intentionally, to make the scene reproducible
    srand(TERRAIN_SEED);

    for (int i = 0; i < TERRAIN_VERTS; i++)
        for (int j = 0; j < TERRAIN_VERTS; j++)
            heights[i][j] = 0.0f;

    for (int h = 0; h < TERRAIN_HILLS; h++) {
        float cx = Rand() * TERRAIN_GRID;
        float cz = Rand() * TERRAIN_GRID;
        float radius = (0.12f + 0.16f * Rand()) * TERRAIN_GRID;

        float peak = (0.15f + 0.20f * Rand()) * TERRAIN_HEIGHT_SCALE;

        for (int i = 0; i < TERRAIN_VERTS; i++) {
            for (int j = 0; j < TERRAIN_VERTS; j++) {
                float dx = i - cx;
                float dz = j - cz;
                float d = sqrtf(dx * dx + dz * dz);
                if (d < radius) {
                    heights[i][j] += peak * 0.5f * (1.0f + cosf(d / radius * 3.14159265f));
                }
            }
        }
    }

    for (int i = 0; i < TERRAIN_VERTS; i++) {
        for (int j = 0; j < TERRAIN_VERTS; j++) {
            int il = i > 0 ? i - 1 : i;
            int ir = i < TERRAIN_GRID ? i + 1 : i;
            int jd = j > 0 ? j - 1 : j;
            int ju = j < TERRAIN_GRID ? j + 1 : j;

            float nx = -(heights[ir][j] - heights[il][j]) / ((ir - il) * TERRAIN_STEP);
            float nz = -(heights[i][ju] - heights[i][jd]) / ((ju - jd) * TERRAIN_STEP);
            float ny = 1.0f;
            float len = sqrtf(nx * nx + ny * ny + nz * nz);

            normals[i][j][0] = nx / len;
            normals[i][j][1] = ny / len;
            normals[i][j][2] = nz / len;
        }
    }

    printf("Initialised terrain: %dx%d grid from %d random hills\n",
           TERRAIN_GRID, TERRAIN_GRID, TERRAIN_HILLS);
}

void DrawTerrain() {
    glColor3f(0.40f, 0.50f, 0.28f);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < TERRAIN_GRID; i++) {
        for (int j = 0; j < TERRAIN_GRID; j++) {
            Vertex(i, j);
            Vertex(i, j + 1);
            Vertex(i + 1, j);

            Vertex(i + 1, j);
            Vertex(i, j + 1);
            Vertex(i + 1, j + 1);
        }
    }
    glEnd();
}
