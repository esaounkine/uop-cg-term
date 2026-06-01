#define TINYOBJLOADER_IMPLEMENTATION

#include <vector>
#include <cmath>
#include "glutwrapper.h"
#include "plane.h"
#include "constants.h"
#include "deps/tiny_obj_loader.h"
#include "texture.h"

static tinyobj::attrib_t attrib;
static std::vector<tinyobj::shape_t> shapes;
static std::vector<tinyobj::material_t> materials;

static unsigned int planeTexture = 0;

// plane position and direction
static float planeX = 0.0f;
static float planeZ = 0.0f;
static float planeY = PLANE_START_Y;
static float planeRotX = -90.0f;
static float planeRotY = 0.0f;
static float planeRoll = 0.0f;
static float planePitch = 0.0f;
static float planeSpeed = PLANE_SPEED;

// propeller (shapes[0]) spins about its centre
static float propAngle = 0.0f;
static float propCX = 0.0f, propCY = 0.0f, propCZ = 0.0f;
static const float PROP_SPEED = 15.0f;

static const float PLANE_SCALE = 100.0f;

// average propeller shape vertices to find the spin centre
static void ComputePropellerCenter() {
    if (shapes.empty()) return;

    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    int count = 0;
    // propeller is shapes[0] in plane.obj
    const auto &shape = shapes[0];
    size_t indexOffset = 0;
    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
        int fv = shape.mesh.num_face_vertices[f];
        for (int v = 0; v < fv; v++) {
            tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
            sumX += attrib.vertices[3 * idx.vertex_index + 0];
            sumY += attrib.vertices[3 * idx.vertex_index + 1];
            sumZ += attrib.vertices[3 * idx.vertex_index + 2];
            count++;
        }
        indexOffset += fv;
    }
    if (count > 0) {
        propCX = sumX / count;
        propCY = sumY / count;
        propCZ = sumZ / count;
    }
}

// the shape's diffuse (or default) colour
static void SetShapeColor(const tinyobj::shape_t &shape) {
    int matId = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];
    if (matId >= 0 && matId < (int) materials.size()) {
        const auto &mat = materials[matId];
        glColor3f(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
    } else {
        glColor3f(0.8f, 0.8f, 0.8f);
    }
}

static void DrawShape(const tinyobj::shape_t &shape) {
    size_t indexOffset = 0;
    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
        int fv = shape.mesh.num_face_vertices[f];
        glBegin(GL_TRIANGLES);
        for (int v = 0; v < fv; v++) {
            tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
            if (idx.texcoord_index >= 0) {
                glTexCoord2f(attrib.texcoords[2 * idx.texcoord_index + 0],
                             attrib.texcoords[2 * idx.texcoord_index + 1]);
            }
            glNormal3f(attrib.normals[3 * idx.normal_index + 0],
                       attrib.normals[3 * idx.normal_index + 1],
                       attrib.normals[3 * idx.normal_index + 2]);
            glVertex3f(attrib.vertices[3 * idx.vertex_index + 0],
                       attrib.vertices[3 * idx.vertex_index + 1],
                       attrib.vertices[3 * idx.vertex_index + 2]);
        }
        glEnd();
        indexOffset += fv;
    }
}

void InitPlane() {
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                "models/plane.obj", "models/");
    if (!ret || !err.empty()) {
        printf("Error loading model: %s\n", err.c_str());
        exit(1);
    }
    if (!warn.empty()) {
        printf("Warning: %s\n", warn.c_str());
    }
    printf("Loaded %zu shapes, %zu materials\n", shapes.size(), materials.size());

    planeTexture = LoadTexture("models/plane.jpg");
    ComputePropellerCenter();
}

void DrawPlane() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, planeTexture);

    glPushMatrix();
    glTranslatef(planeX, planeY, planeZ);
    glRotatef(planeRotY, 0.0f, 1.0f, 0.0f);
    glRotatef(planePitch, 0.0f, 0.0f, 1.0f);
    glRotatef(planeRoll, 1.0f, 0.0f, 0.0f);
    glRotatef(planeRotX, 1.0f, 0.0f, 0.0f);
    glScalef(PLANE_SCALE, PLANE_SCALE, PLANE_SCALE);

    for (size_t s = 0; s < shapes.size(); s++) {
        SetShapeColor(shapes[s]);

        // shape 0 is the propeller: spin it about its own centre
        if (s == 0) {
            glPushMatrix();
            glTranslatef(propCX, propCY, propCZ);
            glRotatef(propAngle, 1.0f, 0.0f, 0.0f);
            glTranslatef(-propCX, -propCY, -propCZ);
        }

        DrawShape(shapes[s]);

        if (s == 0) {
            glPopMatrix();
        }
    }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void SpinPropeller() {
    propAngle += PROP_SPEED;
}

void UpdatePlane() {
    float yaw = planeRotY * DEG2RAD;
    float pitch = planePitch * DEG2RAD;
    float horiz = planeSpeed * cosf(pitch);
    planeX += horiz * cosf(yaw);
    planeZ -= horiz * sinf(yaw);
    planeY += planeSpeed * sinf(pitch);

    // bank back to level when done turning
    planeRoll *= PLANE_ROLL_DECAY;
}

void TurnPlane(float deg) {
    planeRotY += deg;
    planeRoll = (deg < 0.0f) ? PLANE_BANK_ANGLE : -PLANE_BANK_ANGLE;
}

void PitchPlane(float deg) {
    planePitch += deg;
    if (planePitch > PLANE_PITCH_LIMIT) planePitch = PLANE_PITCH_LIMIT;
    if (planePitch < -PLANE_PITCH_LIMIT) planePitch = -PLANE_PITCH_LIMIT;
}

void ChangePlaneSpeed(float delta) {
    planeSpeed += delta;
    if (planeSpeed < PLANE_SPEED_MIN) planeSpeed = PLANE_SPEED_MIN;
    if (planeSpeed > PLANE_SPEED_MAX) planeSpeed = PLANE_SPEED_MAX;
}

void GetPlanePosition(float* x, float* y, float* z) {
    *x = planeX;
    *y = planeY;
    *z = planeZ;
}

float GetPlaneHeading() {
    return planeRotY;
}
