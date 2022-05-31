//
// Created by danylo on 5/31/22.
//

#include <GL/glew.h>

#include "Figure.h"
#include "Camera.h"


glm::vec3 normal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    const auto &vec = glm::normalize(glm::cross(b - a, c - a));

    return vec;
}

void setVert(GLfloat *vertArray, int verInd, glm::vec3 pos, glm::vec3 norm, glm::vec2 texturePos) {
    int offset = verInd * 8;

    vertArray[offset] = pos.x;
    vertArray[offset + 1] = pos.y;
    vertArray[offset + 2] = pos.z;
    vertArray[offset + 3] = norm.x;
    vertArray[offset + 4] = norm.y;
    vertArray[offset + 5] = norm.z;
    vertArray[offset + 6] = texturePos.x;
    vertArray[offset + 7] = texturePos.y;
}

void Figure::loadBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * verticesCount * sizeof(float), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    ///////////////

    glGenVertexArrays(1, &VAO_VECTORS);
    glGenBuffers(1, &VBO_VECTORS);

    glBindVertexArray(VAO_VECTORS);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_VECTORS);
    glBufferData(GL_ARRAY_BUFFER, 3 * normsVerticesCount * sizeof(float), normVert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    /////////////

    glUseProgram(shader);

    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &camera.getProjection()[0][0]);
    GLenum error = glGetError();
    if (error != 0) {
        cout << "Init Error: " << error << endl;
    }

}

void Figure::loadTextures() {
    texture = SOIL_load_OGL_texture(DIR"/textures/oak_planks.png", 3, 0, SOIL_FLAG_MIPMAPS);
    cout << "Texture id " << texture << endl;
    if (texture == 0) {
        std::cout << "Failed to load texture" << std::endl;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "texture1"), 0);
}

void Figure::render() {
    if (camera.isNormSmoothing() != normSmoothing) {
        vector<vec3> vertPos = calcVertPos();
        unsigned long traceLen = trace.trace.size();

        verticesCount = 36 * (traceLen - 1);
        prepareVerArray(traceLen, vertPos);

        loadBuffers();
    }

    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    material.setUniform(shader);
    light.setUniform(shader);
    camera.setUniform(shader);

    glBindVertexArray(VAO);

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &modelTransform[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader, "normal"), 1, GL_FALSE, &normTransform[0][0]);

    if (camera.showTextures) {
        glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, verticesCount);
    }

    if (camera.showNorm) {
        glUseProgram(vectorShader);

        camera.setUniform(vectorShader, false);
        glUniformMatrix4fv(glGetUniformLocation(vectorShader, "model"), 1, GL_FALSE, &modelTransform[0][0]);
        glUniformMatrix3fv(glGetUniformLocation(vectorShader, "normal"), 1, GL_FALSE, &normTransform[0][0]);

        glBindVertexArray(VAO_VECTORS);
        glDrawArrays(GL_LINES, 0, normsVerticesCount);
    }
}

void Figure::setModelTransform(const mat4 &modelTransform) {
    Figure::modelTransform = modelTransform;
    normTransform = glm::mat3(glm::transpose(glm::inverse(modelTransform)));
}

Figure::Figure(Trace &trace, CrossSection &crossSection, GLuint shader, GLuint vectorShader, Camera &camera,
               Light &light)
        : shader(shader), camera(camera), light(light), vectorShader(vectorShader), trace(trace),
          crossSection(crossSection) {

    auto modelTransform = glm::mat4(1.0f);
    setModelTransform(modelTransform);

    vector<vec3> vertPos = calcVertPos();

//    cout << "\n\nPoints:" << endl;
//    for (int i = 0; i < 4 * traceLen; i++) {
//        cout << "[" << i << "]" << "(" << vertPos[i].x << "; " << vertPos[i].y << "; " << vertPos[i].z << ")"
//             << endl;
//    }
//    cout << "\n\n" << endl;

    unsigned long traceLen = trace.trace.size();
    verticesCount = 36 * (traceLen - 1);
    vertices = new GLfloat[verticesCount * 8];
    prepareVerArray(traceLen, vertPos);

    loadBuffers();
    loadTextures();
}

void Figure::prepareVerArray(unsigned long traceLen, const vector<vec3> &vertPos) const {
    glm::vec3 a, b, c;

    int offset;

    for (int i = 0; i < traceLen - 1; i++) {
        offset = 4 * i;

        GLfloat *vertArrray = vertices + 8 * 36 * i;

        for (int j = 0; j < 6; j++) {
            a = vertPos[indices[6 * j] + offset];
            b = vertPos[indices[6 * j + 1] + offset];
            c = vertPos[indices[6 * j + 2] + offset];

            setVert(vertArrray, j * 6, a, normal(a, b, c), vec2(0, 0));
            setVert(vertArrray, j * 6 + 1, b, normal(b, c, a), vec2(0, 1));
            setVert(vertArrray, j * 6 + 2, c, normal(c, a, b), vec2(1, 1));

            a = vertPos[indices[6 * j + 3] + offset];
            b = vertPos[indices[6 * j + 4] + offset];
            c = vertPos[indices[6 * j + 5] + offset];

            setVert(vertArrray, j * 6 + 3, a, normal(a, b, c), vec2(0, 0));
            setVert(vertArrray, j * 6 + 4, b, normal(b, c, a), vec2(1, 1));
            setVert(vertArrray, j * 6 + 5, c, normal(c, a, b), vec2(1, 0));
        }
    }
}

vector<vec3>
Figure::calcVertPos() {
    normSmoothing = camera.isNormSmoothing();
    unsigned long traceLen = trace.trace.size();

    vector<vec3> vertPos(4 * traceLen);

    vec3 prevNorm = normalize(vec3(trace.trace[1]) - vec3(trace.trace[0]));

    normsVerticesCount = traceLen * 2;
    normVert = new GLfloat[normsVerticesCount * 3];

    for (int i = 0; i < traceLen - 1; i++) {
        auto pos = vec3(trace.trace[i]);
        double scale = trace.trace[i].w;

        auto norm = normalize(vec3(trace.trace[i + 1]) - pos);

        if (!normSmoothing) {
            prevNorm = norm;
        }

        const vec3 &smothedNorm = (norm + prevNorm) / vec3(2);
        auto pVec = crossSection.getVert(smothedNorm, pos, scale);
        prevNorm = norm;

        vertPos[i * 4] = pVec[0];
        vertPos[i * 4 + 1] = pVec[1];
        vertPos[i * 4 + 2] = pVec[2];
        vertPos[i * 4 + 3] = pVec[3];

        auto t = pos - smothedNorm;

        normVert[i * 6] = pos.x;
        normVert[i * 6 + 1] = pos.y;
        normVert[i * 6 + 2] = pos.z;
        normVert[i * 6 + 3] = t.x;
        normVert[i * 6 + 4] = t.y;
        normVert[i * 6 + 5] = t.z;

        delete pVec;
    }

    auto pos = vec3(trace.trace[traceLen - 1]);
    double scale = trace.trace[traceLen - 1].w;

    auto norm = normalize(pos - vec3(trace.trace[traceLen - 2]));

    auto pVec = crossSection.getVert(norm, pos, scale);

    int offset = 4 * (traceLen - 1);
    vertPos[offset] = pVec[0];
    vertPos[offset + 1] = pVec[1];
    vertPos[offset + 2] = pVec[2];
    vertPos[offset + 3] = pVec[3];

    auto t = pos + norm;

    normVert[(traceLen - 1) * 6] = pos.x;
    normVert[(traceLen - 1) * 6 + 1] = pos.y;
    normVert[(traceLen - 1) * 6 + 2] = pos.z;
    normVert[(traceLen - 1) * 6 + 3] = t.x;
    normVert[(traceLen - 1) * 6 + 4] = t.y;
    normVert[(traceLen - 1) * 6 + 5] = t.z;

    delete pVec;
    return vertPos;
}
