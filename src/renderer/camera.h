#if !defined(CAMERA_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Andre Rogers $
   $Notice: (C) Copyright 2020 by Dre Codes, Inc. All Rights Reserved. $
   ======================================================================== */

#define CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

class Camera {
public:
    glm::vec3 m_position;
    glm::vec3 m_orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_cameraMatrix = glm::mat4(1.0f);

    // Prevents the camera from jumping
    // around when first clicking left click
    bool m_firstClick = false;

    int m_width;
    int m_height;

    float m_speed = 1.0f;
    float m_sensitivity = 100.0f;

    Camera(int _width, int _height, glm::vec3 _position);
    void UpdateMatrix(float _fov, float _near, float _far);
    void SetMatrix(Shader &_shader, const char *_uniform);
    void Inputs(GLFWwindow *_window);
};
#endif
