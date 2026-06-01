#include <cstdio>
#include "glutwrapper.h"
#include "visuals.h"

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(480, 480);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Flight sim");

    Setup();

    glutDisplayFunc(Render);
    glutReshapeFunc(Resize);
    glutIdleFunc(Idle);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);
    glutPassiveMotionFunc(MouseMotion);
    glutMotionFunc(MouseMotion);
    glutEntryFunc(MouseEntry);

    glutMainLoop();
    return 0;
}
