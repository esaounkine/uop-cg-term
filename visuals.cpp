#define TINYOBJLOADER_IMPLEMENTATION

#include <cstdio>
#include <cstdlib>
#include <vector>
#include "glutwrapper.h"
#include "visuals.h"
#include "deps/tiny_obj_loader.h"

tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

float planeX = 0.0f;
float planeY = 0.0f;
float planeRotZ = 0.0f;
float planeRotY = 0.0f;

const float MOVE_STEP = 5.0f;
const float ROT_STEP = 5.0f;

void DrawModel() {
    for (const auto &shape: shapes) {
        size_t indexOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            glBegin(GL_TRIANGLES);
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

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
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(200.0, 150.0, 300.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glTranslatef(planeX, planeY, 0.0f);
    glRotatef(planeRotY, 0.0f, 1.0f, 0.0f);
    glRotatef(planeRotZ, 0.0f, 0.0f, 1.0f);
    glScalef(100.0f, 100.0f, 100.0f);

    DrawModel();

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

    switch (key) {
        case GLUT_KEY_UP:
            if (shift)
                planeRotZ += ROT_STEP;
            else
                planeY += MOVE_STEP;
            break;
        case GLUT_KEY_DOWN:
            if (shift)
                planeRotZ -= ROT_STEP;
            else
                planeY -= MOVE_STEP;
            break;
        case GLUT_KEY_LEFT:
            if (shift)
                planeRotY += ROT_STEP;
            else
                planeX -= MOVE_STEP;
            break;
        case GLUT_KEY_RIGHT:
            if (shift)
                planeRotY -= ROT_STEP;
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
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "models/plane.obj");

    if (!ret || !err.empty()) {
        printf("Error loading model: %s\n", err.c_str());
        exit(1);
    }
    if (!warn.empty()) {
        printf("Warning: %s\n", warn.c_str());
    }

    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_position[] = {0.0, 50.0, 50.0, 0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
