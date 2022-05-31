//
// Created by danylo on 5/24/22.
//

#ifndef GRAPHICS2_GLUTILLS_H
#define GRAPHICS2_GLUTILLS_H

#include "Shader.h"

void glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                       const void *userParam) {
    if (severity == GL_DEBUG_SEVERITY_HIGH_AMD) {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
    } else {
#ifdef DEBUG
        fprintf(stdout, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
#endif
    }

}

void initGl(int &argc, char **argv, int width, int height, const char *title) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow(title);

    glewExperimental = GL_TRUE;
    GLenum glfwInitState = glewInit();
    if (glfwInitState != GLEW_OK) {
        cerr << "Can't initialize GLFW!" << endl;
        cerr << "Error: " << glewGetErrorString(glfwInitState) << endl;

        exit(-1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glMessageCallback, nullptr);
}

void printGlVersions() {
    cout << "OpenGl version: " << (char *) glGetString(GL_VERSION) << endl;
    cout << "FreeGLUT version: " << glutGet(GLUT_VERSION) << endl;
    cout << "GLEW version: " <<
         GLEW_VERSION << "." << GLEW_VERSION_MAJOR << "." <<
         GLEW_VERSION_MINOR << "." << GLEW_VERSION_MICRO << endl;
}

#endif //GRAPHICS2_GLUTILLS_H
