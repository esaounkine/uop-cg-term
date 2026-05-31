#define TINYOBJLOADER_IMPLEMENTATION

#include <vector>
#include "glutwrapper.h"
#include "visuals.h"
#include "constants.h"
#include "deps/tiny_obj_loader.h"
#include "texture.h"
#include "terrain.h"

tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

float planeX = 0.0f;
float planeY = 0.0f;
float planeRotX = -90.0f;
float planeRotY = 5.0f;
float planeRotZ = 0.0f;
float propAngle = 0.0f;
const float PROP_SPEED = 15.0f;

float propCX = 0.0f, propCY = 0.0f, propCZ = 0.0f;

const float MOVE_STEP = 5.0f;
const float ROT_STEP = 5.0f;

unsigned int planeTexture = 0;

void DrawModel() {
    for (size_t s = 0; s < shapes.size(); s++) {
        const auto &shape = shapes[s];

        // Use material colors if available, otherwise gray
        int matId = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];
        if (matId >= 0 && matId < materials.size()) {
            const auto &mat = materials[matId];
            glColor3f(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
        } else {
            glColor3f(0.8f, 0.8f, 0.8f);
        }

        if (s == 0) {
            glPushMatrix();
            glTranslatef(propCX, propCY, propCZ);
            glRotatef(propAngle, 1.0f, 0.0f, 0.0f);
            glTranslatef(-propCX, -propCY, -propCZ);
        }

        size_t indexOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            glBegin(GL_TRIANGLES);
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                if (idx.texcoord_index >= 0) {
                    float u = attrib.texcoords[2 * idx.texcoord_index + 0];
                    float vcoord = attrib.texcoords[2 * idx.texcoord_index + 1];
                    glTexCoord2f(u, vcoord);
                }

                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];

                float nx = attrib.normals[3 * idx.normal_index + 0];
                float ny = attrib.normals[3 * idx.normal_index + 1];
                float nz = attrib.normals[3 * idx.normal_index + 2];

                glNormal3f(nx, ny, nz);
                glVertex3f(vx, vy, vz);
            }
            glEnd();
            indexOffset += fv;
        }

        if (s == 0) {
            glPopMatrix();
        }
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(CAM_X, CAM_Y, CAM_Z,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    DrawTerrain();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, planeTexture);

    glPushMatrix();
    glTranslatef(planeX, planeY, 0.0f);
    glRotatef(planeRotX, 1.0f, 0.0f, 0.0f);
    glRotatef(planeRotY, 0.0f, 1.0f, 0.0f);
    glRotatef(planeRotZ, 0.0f, 0.0f, 1.0f);
    glScalef(100.0f, 100.0f, 100.0f);

    DrawModel();
    glPopMatrix();

    glutSwapBuffers();
}

void Resize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (h == 0) h = 1;
    gluPerspective(60.0, (float) w / (float) h, 1.0, 1000.0);
}

void Idle() {
    propAngle += PROP_SPEED;
    glutPostRedisplay();
}

void Keyboard(const unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(0);
        default:
            break;
    }
}

void SpecialKeyboard(int key, int x, int y) {
    int mod = glutGetModifiers();
    bool shift = (mod & GLUT_ACTIVE_SHIFT) != 0;
    bool alt = (mod & GLUT_ACTIVE_ALT) != 0;

    switch (key) {
        case GLUT_KEY_UP:
            if (alt)
                planeRotX += ROT_STEP;
            else if (shift)
                planeRotZ += ROT_STEP;
            else
                planeY += MOVE_STEP;
            break;
        case GLUT_KEY_DOWN:
            if (alt)
                planeRotX -= ROT_STEP;
            else if (shift)
                planeRotZ -= ROT_STEP;
            else {
                planeY -= MOVE_STEP;
                if (planeY < -20.0f)
                    planeY = -20.0f;
            }
            break;
        case GLUT_KEY_LEFT:
            if (shift)
                planeRotY -= ROT_STEP;
            else
                planeX -= MOVE_STEP;
            break;
        case GLUT_KEY_RIGHT:
            if (shift)
                planeRotY += ROT_STEP;
            else
                planeX += MOVE_STEP;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void Setup() {
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "models/plane.obj", "models/");

    if (!ret || !err.empty()) {
        printf("Error loading model: %s\n", err.c_str());
        exit(1);
    }
    if (!warn.empty()) {
        printf("Warning: %s\n", warn.c_str());
    }

    printf("Loaded %zu shapes, %zu materials\n", shapes.size(), materials.size());

    planeTexture = LoadTexture("models/plane.jpg");

    InitTerrain();

    // Compute the propeller (shape[0]) center for rotation
    // it was off initially, was spinning around a random axis
    if (!shapes.empty()) {
        float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
        int count = 0;
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
            printf("Propeller center: (%.3f, %.3f, %.3f)\n", propCX, propCY, propCZ);
        }
    }

    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light_position[] = {200.0f, 150.0f, 300.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
}
