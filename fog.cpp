#include "fog.h"
#include "deps/glutwrapper.h"
#include "constants.h"

void DrawFog() {
    GLfloat fogColor[] = {FOG_R, FOG_G, FOG_B, 1.0f};
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, FOG_START);
    glFogf(GL_FOG_END, FOG_END);
    glEnable(GL_FOG);

    glClearColor(FOG_R, FOG_G, FOG_B, 1.0f);
}
