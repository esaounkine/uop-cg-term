#include "camera.h"
#include "deps/glutwrapper.h"
#include "constants.h"
#include "plane.h"
#include <cmath>

static float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 0.0f;

static float orbitRoll = 0.0f;
static float orbitYaw = 0.0f;
static float orbitPitch = 0.0f;
static float zoom = 2.0f;

static int lastMouseX = -1;
static int lastMouseY = -1;
static bool mouseInWindow = false;

void ApplyCamera() {
    float px, py, pz;
    GetPlanePosition(&px, &py, &pz);

    float yaw = (GetPlaneHeading() + orbitYaw) * DEG2RAD;
    float pitch = orbitPitch * DEG2RAD;

    float dist = CAM_DIST * zoom;
    float fx = cosf(yaw);
    float fz = -sinf(yaw);

    eyeX = px - dist * fx * cosf(pitch);
    eyeY = py + CAM_HEIGHT + dist * sinf(pitch);
    eyeZ = pz - dist * fz * cosf(pitch);

    gluLookAt(eyeX, eyeY, eyeZ,
              px, py, pz,
              0.0, 1.0, 0.0);
}

void GetCameraEye(float *x, float *y, float *z) {
    *x = eyeX;
    *y = eyeY;
    *z = eyeZ;
}

void CameraMouseEntry(int state) {
    // start fresh on re-entry so the view doesn't jump
    mouseInWindow = (state == GLUT_ENTERED);
    lastMouseX = -1;
}

void CameraMouseMotion(int x, int y) {
    if (!mouseInWindow) return;

    if (lastMouseX < 0) {
        // first sample, just remember it
        lastMouseX = x;
        lastMouseY = y;
        return;
    }

    orbitYaw += (x - lastMouseX) * CAM_MOUSE_SENS;
    orbitPitch += (y - lastMouseY) * CAM_MOUSE_SENS;

    // keep the pitch from flipping over the top or bottom
    if (orbitPitch > CAM_PITCH_LIMIT) orbitPitch = CAM_PITCH_LIMIT;
    if (orbitPitch < -CAM_PITCH_LIMIT) orbitPitch = -CAM_PITCH_LIMIT;

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void CameraZoom(float delta) {
    zoom += delta;
    if (zoom < CAM_ZOOM_MIN) zoom = CAM_ZOOM_MIN;
    if (zoom > CAM_ZOOM_MAX) zoom = CAM_ZOOM_MAX;
    glutPostRedisplay();
}
