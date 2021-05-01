#include "camera.h"

Camera::Camera(int _width, int _height, glm::vec3 _position)
    : m_width(_width), m_height(_height), m_position(_position) {}

void Camera::SetUniform(Shader &_shader, const char *_uniform) {
    glUniformMatrix4fv(glGetUniformLocation(_shader.ID, _uniform), 1, GL_FALSE, glm::value_ptr(m_cameraMatrix));
}

void Camera::UpdateMatrix(float _fov, float _near, float _far) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(m_position, m_position + m_orientation, m_up);
    projection = glm::perspective(glm::radians(_fov), (float)(m_width / m_height), _near, _far);

    m_cameraMatrix = projection * view;
}

void Camera::Inputs(GLFWwindow *_window) {
    if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
        m_position += m_speed * m_orientation;
    }

    if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
        m_position += m_speed * -glm::normalize(glm::cross(m_orientation, m_up));
    }

    if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
        m_position += m_speed * -m_orientation;
    }

    if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
        m_position += m_speed * glm::normalize(glm::cross(m_orientation, m_up));
    }

    if(glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_position += m_speed * m_up;
    }

    if(glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        m_position += m_speed * -m_up;
    }

    if(glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        m_speed = 0.04f;
    } else if(glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        m_speed = 0.0f;
    }


	// Handles mouse inputs
	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (m_firstClick)
		{
			glfwSetCursorPos(_window, (m_width / 2), (m_height / 2));
			m_firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(_window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they being in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = m_sensitivity * (float)(mouseY - (m_height / 2)) / m_height;
		float rotY = m_sensitivity * (float)(mouseX - (m_height / 2)) / m_height;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(m_orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_orientation, m_up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		m_orientation = glm::rotate(m_orientation, glm::radians(-rotY), m_up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(_window, (m_width / 2), (m_height / 2));
	}
	else if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		m_firstClick = true;
	}
}
