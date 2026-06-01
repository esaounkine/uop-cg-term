#define STB_PERLIN_IMPLEMENTATION

#include "terrain.h"
#include "glutwrapper.h"
#include "constants.h"
#include "texture.h"
#include "plane.h"
#include "deps/stb_perlin.h"
#include <cmath>

// height field is a grid of vertices
// heights and normals computed in InitTerrain()
// triangles emitted on each frame by DrawTerrain()
static float heights[TERRAIN_VERTS][TERRAIN_VERTS];
static float normals[TERRAIN_VERTS][TERRAIN_VERTS][3];
static unsigned int terrainTexture = 0;

static float originX = 0.0f, originZ = 0.0f;
static float lastCenterX = MAXFLOAT, lastCenterZ = MAXFLOAT;

// terrain height at a world position
static float HeightAt(float wx, float wz) {
    float x = wx / TERRAIN_NOISE_SCALE + TERRAIN_SEED * 100.0f;
    float z = wz / TERRAIN_NOISE_SCALE;
    float n = stb_perlin_fbm_noise3(x, 0.0f, z, 2.0f, 0.5f, TERRAIN_OCTAVES);
    return (n * 0.5f + 0.5f) * TERRAIN_HEIGHT_SCALE; // [-1,1] -> [0, scale]
}

// per-vertex normals from neighbour height differences (for proper shading)
static void ComputeNormals() {
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
}

static void Recentre(float cx, float cz) {
    float half = TERRAIN_SIZE * 0.5f;
    originX = floorf((cx - half) / TERRAIN_STEP) * TERRAIN_STEP;
    originZ = floorf((cz - half) / TERRAIN_STEP) * TERRAIN_STEP;

    for (int i = 0; i < TERRAIN_VERTS; i++)
        for (int j = 0; j < TERRAIN_VERTS; j++)
            heights[i][j] = HeightAt(originX + i * TERRAIN_STEP, originZ + j * TERRAIN_STEP);

    ComputeNormals();
}

static void Vertex(int i, int j) {
    float x = originX + i * TERRAIN_STEP;
    float z = originZ + j * TERRAIN_STEP;

    float tile = TERRAIN_SIZE / TERRAIN_TILES;
    glNormal3fv(normals[i][j]);
    glTexCoord2f(x / tile, z / tile);
    glVertex3f(x, TERRAIN_Y + heights[i][j], z);
}

void InitTerrain() {
    terrainTexture = LoadTextureBMP("models/terrain.bmp");
    Recentre(0.0f, 0.0f);
}

// rebuild when the plane moves away from last centre
void UpdateTerrain() {
    float px, py, pz;
    GetPlanePosition(&px, &py, &pz);
    if (fabsf(px - lastCenterX) > TERRAIN_STEP || fabsf(pz - lastCenterZ) > TERRAIN_STEP) {
        Recentre(px, pz);
        lastCenterX = px;
        lastCenterZ = pz;
    }
}

void DrawTerrain() {
    glColor3f(1.0f, 1.0f, 1.0f); // white so the texture shows true colour (and not blue-ish)

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);

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

    glDisable(GL_TEXTURE_2D);
}
