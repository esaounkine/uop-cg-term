#ifndef CAMERA_H
#define CAMERA_H

// place the view behind + above the plane (call at the start of Render)
void ApplyCamera();

// current eye position
void GetCameraEye(float* x, float* y, float* z);

#endif
