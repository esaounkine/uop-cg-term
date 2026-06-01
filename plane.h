#ifndef PLANE_H
#define PLANE_H

void InitPlane();
void DrawPlane();
void SpinPropeller();

void MovePlane(float dx, float dy);
void RotatePlane(float dx, float dy, float dz);

void GetPlanePosition(float* x, float* y, float* z);
float GetPlaneHeading();

#endif
