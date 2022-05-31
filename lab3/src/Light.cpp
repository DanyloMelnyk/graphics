//
// Created by danylo on 5/31/22.
//

#include <GL/glew.h>
#include <iostream>
#include <SOIL/SOIL.h>

#include "Light.h"

#define DIR "/home/danylo/CLionProjects/graphics/lab3"

using namespace std;

void Light::init(GLuint shader) {
    cout << "Init light" << endl;

    Light::shader = shader;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    texture = SOIL_load_OGL_texture(DIR"/textures/redstone_lamp_on.png", 3, 0, SOIL_FLAG_MIPMAPS);
    cout << "Texture id " << texture << endl;
    if (texture == 0) {
        std::cout << "Failed to load texture" << std::endl;
        exit(-1);
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "texture"), 0);

    GLenum error = glGetError();
    if (error != 0) {
        cout << "Init Error: " << error << endl;
    }

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // -----------------------------------------------------------------------------------------------------------

    error = glGetError();
    if (error != 0) {
        cout << "Init Error: " << error << endl;
    }
}

void Light::render(Camera camera) {
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &camera.getProjection()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &camera.getView()[0][0]);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    GLenum error = glGetError();
    if (error != 0) {
        cout << "Render Error: " << error << endl;
    }
}

void Light::setUniform(GLuint shader) {
    glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, &position[0]);

    glUniform3fv(glGetUniformLocation(shader, "light.ambient"), 1, &ambient[0]);
    glUniform3fv(glGetUniformLocation(shader, "light.diffuse"), 1, &diffuse[0]);
    glUniform3fv(glGetUniformLocation(shader, "light.specular"), 1, &specular[0]);
}
