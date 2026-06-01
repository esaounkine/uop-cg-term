//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the
// OpenGL context associated to a window.

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();
// Idle function

void Keyboard(unsigned char key, int x, int y);
// Function for handling keyboard events.

void SpecialKeyboard(int key, int x, int y);
// Function for handling special keys (arrows, PageUp/PageDown).

void MouseMotion(int x, int y);
// Mouse movement (camera orbit).

void MouseEntry(int state);
// Cursor entering/leaving the window (gate mouse capture).
