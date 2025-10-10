// Includes math library and IO library
#include <math.h>
#include <stdio.h>

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

#define RESOLUTION 4
#define SW 160*RESOLUTION
#define SH 120*RESOLUTION
#define PIXEL_SCALE 4/RESOLUTION
#define GLSW (SW*PIXEL_SCALE)
#define GLSH (SH*PIXEL_SCALE)

#define UNUSED(obj) ((void)obj) // Marks an object as unused

static const GLfloat vertices[] = {
	// first triangle
	-0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
    // second triangle
	0.0f, -0.5f, 0.0f,  // left
	0.9f, -0.5f, 0.0f,  // right
	0.45f, 0.5f, 0.0f   // top
};
int main() {
	// Initializes the program state and glfw3
	AppState app = {0};
	if (!glfwInit()) {
		const char *buf; glfwGetError(&buf);
		fprintf(stderr, "Couldn't initialize GLFW: %s\n", buf);
		return 1;
	}

	// Tells glfw3 to use OpenGL Core 4.6
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__ // Make OpenGL compatible in Apple (ig)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Creates a window with glfw3
	app.window = glfwCreateWindow(SW, SH, "OpenGL 4.6", NULL, NULL);
	if (!app.window) {
		const char *buf; glfwGetError(&buf);
		fprintf(stderr, "Couldn't create window: %s\n", buf);
		goto terminate;
	}
	glfwMakeContextCurrent(app.window); // Sets OpenGL context as current
	
	// Hides the window, will show when everything else is set up
	glfwHideWindow(app.window);

	// Tell GLAD to load OpenGL
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		fprintf(stderr, "Couldn't initialize GLAD\n");
		goto terminate;
	}

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
	
	// We unbind the VAO and VBO to prevent making changes to them
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// We set OpenGL viewport size to be the same as
	// out window's size
	glViewport(0, 0, GLSW, GLSH);

	// Show the window
	glfwShowWindow(app.window);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// We loop until the window wants to close
	while (!glfwWindowShouldClose(app.window)) {
		// If the user presses 'Q' we close the window
		if (glfwGetKey(app.window, GLFW_KEY_Q) == GLFW_PRESS) {
			goto exit;
		}

		// Clears the background with a bluish color
		// and clears any old graphic
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// We start our shader program and render some triangle
		glUseProgram(app.shaders.programID);
		glBindVertexArray(app.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// GLFW updates the buffers and polls other events
		glfwSwapBuffers(app.window);
		glfwPollEvents();
	}

exit:
	// Destroys the VAO and VBO alongside the program shader
	glDeleteVertexArrays(1, &app.VAO);
	glDeleteBuffers(1, &app.VBO);
	glDeleteProgram(app.shaders.programID);

	// Destroys the window
	glfwDestroyWindow(app.window);

terminate:
	// Quits GLFW and cleans this up
	glfwTerminate();
	return 0;
}
