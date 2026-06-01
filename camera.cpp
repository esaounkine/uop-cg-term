#include "camera.h"
#include "glutwrapper.h"
#include "constants.h"
#include "plane.h"
#include <cmath>

static float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 0.0f;

void ApplyCamera() {
    float px, py, pz;
    GetPlanePosition(&px, &py, &pz);
    float yaw = GetPlaneHeading() * 3.14159265f / 180.0f;

    // direction of the nose
    float fx = cosf(yaw);
    float fz = -sinf(yaw);

    eyeX = px - CAM_DIST * fx;
    eyeY = py + CAM_HEIGHT;
    eyeZ = pz - CAM_DIST * fz;

    gluLookAt(eyeX, eyeY, eyeZ,
              px, py, pz,
              0.0, 1.0, 0.0);
}

void GetCameraEye(float* x, float* y, float* z) {
    *x = eyeX;
    *y = eyeY;
    *z = eyeZ;
}
