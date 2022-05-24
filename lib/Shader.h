//
// Created by danylo on 5/6/22.
//
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

string readFile(const char *filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);

    if (!fileStream.is_open()) {
        cerr << "Could not read file " << filePath << ". File does not exist." << endl;
        return "";
    }

    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


GLuint loadShaders(const char *vertex_path, const char *fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Read shaders

    string vertShaderStr = readFile(vertex_path);
    string fragShaderStr = readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex primitiveShader

    cout << "Compiling: " << vertex_path << endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex primitiveShader

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    if (vertShaderError[0] != '\0') {
        cerr << "Compiling: " << vertex_path << endl;

        cerr << "Errors: " << &vertShaderError[0] << endl;
        exit(-1);
    }

    // Compile fragment primitiveShader

    cout << "Compiling: " << fragment_path << endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment primitiveShader

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);

    if (fragShaderError[0] != '\0') {
        cerr << "Compiling: " << fragment_path << endl;

        cerr << "Errors: " << &fragShaderError[0] << endl;
        exit(-1);
    }

    cout << "Linking program (" << vertex_path << " + " << fragment_path << ")" << endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);

    if (programError[0] != '\0') {
        cerr << "Linking program (" << vertex_path << " + " << fragment_path << ")" << endl;

        cerr << "Errors: " << &programError[0] << endl;
        exit(-1);
    } else {
        cout << "Successful!\n" << endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}