#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


double lastx = 400.0f;
double lasty = 300.0f;
bool startup = true;

class Camera {
public:
	//variables
	glm::vec3 campos;
	glm::vec3 camfront;
	glm::vec3 camup;
	float yaw;
	float pitch;
	float fov;
	float sensitivity;

	//constructor
	Camera(float posX, float posY, float posZ, float frontX, float frontY, float frontZ, float YAW, float PITCH, float FOV, float SENSITIVITY)
	{
		campos = glm::vec3(posX, posY, posZ);
		camfront = glm::vec3(frontX, frontY, frontZ);
		camup = glm::vec3(0.0f, 1.0f, 0.0f);
		yaw = YAW;
		pitch = PITCH;
		fov = FOV;
		sensitivity = SENSITIVITY;
	}

	void input(GLFWwindow* window, float deltatime) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {    //escape
			glfwSetWindowShouldClose(window, true);
		}

		float camspeed = 2.5f * deltatime;                          //wasd
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			campos += camspeed * camfront;
		}if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			campos -= camspeed * camfront;
		}if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			campos -= glm::normalize(glm::cross(camfront, camup)) * camspeed;
		}if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			campos += glm::normalize(glm::cross(camfront, camup)) * camspeed;
		}if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			campos += camspeed * camup;
		}if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			campos -= camspeed * camup;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			fov += 10.0f * deltatime;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			fov -= 10.0f * deltatime;
		}
	}
	//handle mouse movement
	void mouse(double xpos, double ypos) {
		if (startup) {
			lastx = xpos;
			lasty = ypos;
			startup = false;
		}
		float xoffset = (xpos - lastx) * sensitivity;
		float yoffset = -(ypos - lasty) * sensitivity;
		lastx = xpos;
		lasty = ypos;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f) { pitch = 89.0f; }
		if (pitch < -89.0f) { pitch = -89.0f; }
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		camfront = glm::normalize(direction);
	}
};
#endif