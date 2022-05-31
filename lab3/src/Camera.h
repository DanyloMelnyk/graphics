//
// Created by danylo on 5/31/22.
//

#ifndef GRAPHICS1_CAMERA_H
#define GRAPHICS1_CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>


using namespace glm;

class Camera {
    vec3 pos = vec3(0.0, 0.0, 5.0);

    vec3 target = vec3(0.0, 0.0, 0.0);

    vec3 up = vec3(0.0f, 1.0f, 0.0f);

    mat4 view = lookAt(pos, target, up);

    mat4 projection;

    double size = 2.0;
    mat4 orthoProjection = glm::ortho(-size, size, -size, size, 0.1, 100.0);


    bool normSmoothing = true; // FIXME
public:

    bool showNorm = false;

    void changePos(vec3 d) {
        vec3 direction = normalize(pos - target);
//
        vec3 right = normalize(cross(up, direction));

//        cout << "Move camera (player system): (" << d.x << "; " << d.y << "; " << d.z << ")" << endl;
        vec3 t = d.x * right + d.y * up - d.z * direction;

        target = target + t;

        size -= d.z * 0.3;

//        cout << "Move camera (world system): (" << t.x << "; " << t.y << "; " << t.z << ")" << endl;

        setPos(pos + t);
    }

    void moveTarget(vec3 d) {
        vec3 direction = normalize(pos - target);
//
        vec3 right = normalize(cross(up, direction));


//        cout << "Move camera (player system): (" << d.x << "; " << d.y << "; " << d.z << ")" << endl;
        vec3 t = d.x * right + d.y * up - d.z * direction;

        target = target + t;

        setPos(pos);
    }

    void setPos(vec3 newPos) {
        pos = newPos;
        view = lookAt(pos, target, vec3(0.0, 1.0, 0.0));

        std::cout << "New camera pos: (" << pos.x << "; " << pos.y << "; " << pos.z << ")" << std::endl;
    }

    const mat4 &getView() const {
        return view;
    }

    const vec3 &getPos() const {
        return pos;
    }

    void setProjection(const mat4 &projection) {

        Camera::projection = projection;
    }

    const mat4 &getProjection() {
        if (perspective) {
            return projection;
        } else {
            orthoProjection = glm::ortho(-size, size, -size, size, 0.1, 100.0);
            return orthoProjection;
        }
    }

    void setUniform(GLuint shader, bool viewPos = true) {
        if (viewPos)
            glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &pos[0]);

        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &getProjection()[0][0]);

        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
    }

    bool isNormSmoothing() const {
        return normSmoothing;
    }

    void toggleNormSmoothing() {
        normSmoothing = !normSmoothing;
    }

    void toggleNorm() {
        showNorm = !showNorm;
    }

    bool showTextures = true;
    bool perspective = true;

    void toggleTextures() {
        showTextures = !showTextures;
    }
};


#endif //GRAPHICS1_CAMERA_H
