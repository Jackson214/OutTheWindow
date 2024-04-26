#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Define the size of the terrain
const int TERRAIN_SIZE = 65536; // 2^16
const int screenFontSize = 18;

// Camera parameters
float camX = 0.0f;
float camY = 50.0f;
float camZ = 0.0f;
float camYaw = 0.0f;
float camPitch = 0.0f;

enum TextHorizontalAlign {
    TEXT_HALIGN_LEFT,
    TEXT_HALIGN_CENTER,
    TEXT_HALIGN_RIGHT
};
enum TextVerticalAlign {
    TEXT_VALIGN_TOP,
    TEXT_VALIGN_CENTER,
    TEXT_VALIGN_BOTTOM
};

// Function to initialize OpenGL settings
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
}

// x and y are in pixel coordinates, with (0, 0) at the bottom-left corner of the window
void renderScreenText(float x, float y, const char* text, TextHorizontalAlign hAlign = TEXT_HALIGN_LEFT, TextVerticalAlign vAlign = TEXT_VALIGN_BOTTOM) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Calculate text width
    int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text);
    int textHeight = screenFontSize;

    // Adjust x position based on alignment
    switch (hAlign) {
        case TEXT_HALIGN_LEFT:
            // Left-aligned, no adjustment needed
            break;
        case TEXT_HALIGN_CENTER:
            x -= textWidth / 2;
            break;
        case TEXT_HALIGN_RIGHT:
            x -= textWidth;
            break;
    }

    switch (vAlign) {
        case TEXT_VALIGN_TOP:
            y -= textHeight;
            break;
        case TEXT_VALIGN_CENTER:
            y -= textHeight / 2;
            break;
        case TEXT_VALIGN_BOTTOM:
            // Bottom-aligned, no adjustment needed
            break;
    }

    glRasterPos2f(x, y);

    // Loop through each character in the string and render it
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Use a 12-point Helvetica font
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void renderWorldText(float x, float y, float z, const char* text) {
    glRasterPos3f(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white

    // Loop through each character in the string and render it
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c); // Use a 12-point Helvetica font
    }
}

// Function to render the terrain
void renderTerrain() {
    // Render the terrain here using a heightmap or other method
    // For simplicity, we'll just render a flat quad

    glColor3f(0.3f, 0.6f, 0.3f); // Green color for terrain
    glBegin(GL_QUADS);
    glVertex3f(-TERRAIN_SIZE / 2, 0, -TERRAIN_SIZE / 2);
    glVertex3f(-TERRAIN_SIZE / 2, 0, TERRAIN_SIZE / 2);
    glVertex3f(TERRAIN_SIZE / 2, 0, TERRAIN_SIZE / 2);
    glVertex3f(TERRAIN_SIZE / 2, 0, -TERRAIN_SIZE / 2);
    glEnd();
}

// Function to render the skybox
void renderSky() {
    // Render the sky here using a skybox or other method
    // For simplicity, we'll just render a blue gradient
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Function to handle rendering
void render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-camPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-camX, -camY, -camZ);

    // Render the sky
    renderSky();

    // Render the terrain
    renderTerrain();

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    glColor3f(0.0f, 0.0f, 0.0f);
    std::string text = "x: " + std::to_string(camX) + "\ny: " + std::to_string(camY) + "\nz: " + std::to_string(camZ);
    renderScreenText(0, windowHeight, text.c_str(), TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP);
    text = "yaw: " + std::to_string(camYaw) + "\npitch: " + std::to_string(camPitch);
    renderScreenText(0, windowHeight - screenFontSize, text.c_str(), TEXT_HALIGN_LEFT, TEXT_VALIGN_TOP);
    renderWorldText(0.0f, 10.0f, 0.0f, "Origin");

    // Update the display
    glutSwapBuffers();
}

// Function to handle window resizing
void reshape(int w, int h) {
    // Set the viewport
    glViewport(0, 0, w, h);

    // Update projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 45 degree fov, ratio, clipping render near plane, clipping render far plane
    gluPerspective(60.0f, (float)w / (float)h, 0.1f, 10000.0f); 
}

// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    // Handle keyboard input (e.g., camera controls)
    switch (key) {
        case 'w':
            camX -= sin(camYaw * M_PI / 180.0f);
            camZ -= cos(camYaw * M_PI / 180.0f);
            break;
        case 's':
            camX += sin(camYaw * M_PI / 180.0f);
            camZ += cos(camYaw * M_PI / 180.0f);
            break;
        case 'd':
            camX += sin((camYaw + 90) * M_PI / 180.0f);
            camZ += cos((camYaw + 90) * M_PI / 180.0f);
            break;
        case 'a':
            camX -= sin((camYaw + 90) * M_PI / 180.0f);
            camZ -= cos((camYaw + 90) * M_PI / 180.0f);
            break;
        case ' ':
            camY += 1.0f;
            break;
        case 'c':
            camY -= 1.0f;
            break;
    }
    glutPostRedisplay();
}

// Function to handle special key input
void specialKeys(int key, int x, int y) {
    // Handle special key input (e.g., camera rotation)
    switch (key) {
        case GLUT_KEY_LEFT:
            camYaw += 1.0f;
            if (camYaw > 360.0f)
                camYaw -= 360.0f;
            if (camYaw < -360.0f)  
                camYaw += 360.0f;
            break;
        case GLUT_KEY_RIGHT:
            camYaw -= 1.0f;
            if (camYaw > 360.0f)
                camYaw -= 360.0f;
            if (camYaw < -360.0f)  
                camYaw += 360.0f;
            break;
        case GLUT_KEY_UP:
            camPitch += 1.0f;
            if (camPitch > 360.0f)
                camPitch -= 360.0f;
            if (camPitch < -360.0f)
                camPitch += 360.0f;
            break;
        case GLUT_KEY_DOWN:
            camPitch -= 1.0f;
            if (camPitch > 360.0f)
                camPitch -= 360.0f;
            if (camPitch < -360.0f)
                camPitch += 360.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Out-The-Window View");

    // Initialize OpenGL settings
    initOpenGL();

    // Register callback functions
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Enter the GLUT event processing loop
    glutMainLoop();

    return 0;
}
