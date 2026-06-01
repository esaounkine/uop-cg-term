#ifndef CAMERA_H
#define CAMERA_H

// place the view behind + above the plane (call at the start of Render)
void ApplyCamera();

// current eye position
void GetCameraEye(float* x, float* y, float* z);

// mouse cursor orbits the camera
void CameraMouseMotion(int x, int y);
void CameraMouseEntry(int state); // track whether the cursor is in the window

// negative = zoom in, positive = zoom out (bound to +/- keys)
void CameraZoom(float delta);

#endif
