#include <cstdlib>
#include "glutwrapper.h"
#include "visuals.h"
#include "constants.h"
#include "terrain.h"
#include "plane.h"
#include "camera.h"
#include "fog.h"

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ApplyCamera();

    UpdateTerrain();
    DrawTerrain();
    DrawPlane();

    glutSwapBuffers();
}

void Resize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) w / (float) h, 1.0, 1000.0);
}

void Idle() {
    UpdatePlane();
    SpinPropeller();
    glutPostRedisplay();
}

void MouseMotion(int x, int y) {
    CameraMouseMotion(x, y);
}

void MouseEntry(int state) {
    CameraMouseEntry(state);
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(0);
        case 'w':
            ChangePlaneSpeed(PLANE_SPEED_STEP);
            break;
        case 's':
            ChangePlaneSpeed(-PLANE_SPEED_STEP);
            break;
        case '=':
        case '+':
            CameraZoom(-CAM_ZOOM_STEP);
            break;
        case '-':
        case '_':
            CameraZoom(CAM_ZOOM_STEP);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y) {
    // left/right yaw the heading; up/down pitch the nose
    switch (key) {
        case GLUT_KEY_LEFT:
            TurnPlane(PLANE_TURN_STEP);
            break;
        case GLUT_KEY_RIGHT:
            TurnPlane(-PLANE_TURN_STEP);
            break;
        case GLUT_KEY_UP:
            PitchPlane(PLANE_PITCH_STEP);
            break;
        case GLUT_KEY_DOWN:
            PitchPlane(-PLANE_PITCH_STEP);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void Setup() {
    InitPlane();
    InitTerrain();

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

    DrawFog();
}
