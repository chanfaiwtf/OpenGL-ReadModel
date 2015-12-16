// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void my_mode();
void setupLight(Shader &shader);
void lightControl(Shader &shader);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
// handle of light
int ison[4] = { -1, -1, -1, -1 };

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader shader("model_loading.vs", "model_loading.frag");

	// Load models
	//Model ourModel("The City/The City.obj");
	Model ourModel("nanosuit/nanosuit.obj");


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();
		my_mode();
		lightControl(shader);

		// Clear the colorbuffer
		glClearColor(0.05f, 0.55f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();   // <-- Don't forget this one!
		setupLight(shader);

		// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(shader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region "User input"

void my_mode()
{
	if (keys[GLFW_KEY_Z])
	{
		glPointSize(2.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (keys[GLFW_KEY_X])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (keys[GLFW_KEY_C])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void lightControl(Shader &shader)
{
	if (keys[GLFW_KEY_1])
	{
		if (ison[0] > 0)
		{
			ison[0] = -1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[0].on"), -1);
		}
		else
		{
			ison[0] = 1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[0].on"), 1);
		}
		
	}
	if (keys[GLFW_KEY_2])
	{
		if (ison[1] > 0)
		{
			ison[1] = -1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[1].on"), -1);
		}
		else
		{
			ison[1] = 1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[1].on"), 1);
		}
	}
	if (keys[GLFW_KEY_3])
	{
		if (ison[2] > 0)
		{
			ison[2] = -1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[2].on"), -1);
		}
		else
		{
			ison[0] = 1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[2].on"), 1);
		}
	}
	if (keys[GLFW_KEY_4])
	{
		if (ison[3] > 0)
		{
			ison[3] = -1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[3].on"), -1);
		}
		else
		{
			ison[3] = 1;
			glUniform1i(glGetUniformLocation(shader.Program, "pointLights[3].on"), 1);
		}
	}
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void setupLight(Shader &shader)
{
	// == ==========================
	// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
	// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
	// == ==========================
	// Directional light
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	// Point light 1
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].quadratic"), 0.032);
	// Point light 2
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].quadratic"), 0.032);
	// Point light 3
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[2].quadratic"), 0.032);
	// Point light 4
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(shader.Program, "pointLights[3].quadratic"), 0.032);
	// SpotLight
	//glUniform3f(glGetUniformLocation(shader.Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	//glUniform3f(glGetUniformLocation(shader.Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
	//glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	//glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	//glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), 1.0f);
	//glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), 0.09);
	//glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), 0.032);
	//glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	//glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
}
#pragma endregion