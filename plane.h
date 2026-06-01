#ifndef PLANE_H
#define PLANE_H

void InitPlane();
void DrawPlane();
void SpinPropeller();

void UpdatePlane();
void TurnPlane(float deg);
void PitchPlane(float deg);
void ChangePlaneSpeed(float delta);

void GetPlanePosition(float* x, float* y, float* z);
float GetPlaneHeading();

#endif
