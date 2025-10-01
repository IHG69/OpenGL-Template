// Includes math library
#include <math.h>

// Includes and defined GLAD implementatioin
#define GLAD_GL_IMPLEMENTATION
#include <glad/GL.h>

// Includes glfw3 and tell it to not include OpenGL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Vertex Shader Source
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader Source
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// Data structure of the program state
typedef struct AppState {
	GLFWwindow *window;
	struct {
		GLuint vertex;
		GLuint fragment;
		GLuint programID;
	} shaders;
	GLuint VBO, VAO;
} AppState;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Triangle's vertices
static const GLfloat vertices[] = {
	-0.5f, -0.5f * (float)sqrt(3) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * (float)sqrt(3) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * (float)sqrt(3) * 2 / 3, 0.0f // Upper corner
};

int main() {
	// Initializes the program state and glfw3
	AppState app = {0};
	if (!glfwInit()) {
		return 1;
	}

	// Tells glfw3 to use OpenGL Core 4.6
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates a window with glfw3
	app.window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL 4.6", NULL, NULL);
	if (!app.window) goto terminate;
	glfwMakeContextCurrent(app.window); // Sets OpenGL context as current

	// Tell GLAD to load OpenGL
	gladLoadGL(glfwGetProcAddress);

	// Creates a vertex shader and compiles
	app.shaders.vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(app.shaders.vertex, 1, &vertexShaderSource, NULL);
	glCompileShader(app.shaders.vertex);

	// Creates a fragment shaders and compiles
	app.shaders.fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(app.shaders.fragment, 1, &fragmentShaderSource, NULL);
	glCompileShader(app.shaders.fragment);
	
	// Creates a shader program and links
	app.shaders.programID = glCreateProgram();
	glAttachShader(app.shaders.programID, app.shaders.vertex);
	glAttachShader(app.shaders.programID, app.shaders.fragment);
	glLinkProgram(app.shaders.programID);

	// Deletes the shaders 'cos they're already linked
	glDeleteShader(app.shaders.vertex);
	glDeleteShader(app.shaders.fragment);

	// Generates VAO and VBO buffers
	glGenVertexArrays(1, &app.VAO);
	glGenBuffers(1, &app.VBO);
	
	// Bind both buffers
	glBindVertexArray(app.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, app.VBO);
	
	// Sends the data 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	while (!glfwWindowShouldClose(app.window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(app.shaders.programID);
		glBindVertexArray(app.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(app.window);
		glfwPollEvents();
	}

exit:
	glDeleteVertexArrays(1, &app.VAO);
	glDeleteBuffers(1, &app.VBO);
	glDeleteProgram(app.shaders.programID);

	glfwDestroyWindow(app.window);

terminate:
	glfwTerminate();
	return 0;
}
