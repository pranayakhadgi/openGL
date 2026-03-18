#include <glad.h>   // GLAD manages function pointers for OpenGL so we don't have to resolve them manually.
#include <glfw3.h>  // GLFW handles window creation, contexts, and input (interaction with the OS).
#include <iostream> // Standard input-output stream for printing errors and logs to the console.
#include "shaderClass.h" // Custom shader class to simplify shader compilation and usage.

// Function prototypes to let the compiler know these exist before they are called in main.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings for window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// VERTEX SHADER SOURCE
// This runs on the GPU for every vertex we pass in.
// #version 330 core: We are using OpenGL 3.3 Core profile.
// layout (location = 0): Matches the pointer setup in C++ (glVertexAttribPointer).
// out vec3 outColor: Output variable to pass color data to the fragment shader.
// gl_Position: Built-in output variable that sets the final position of the vertex in clip space.


// FRAGMENT SHADER 1 (Orange)
// This runs for every pixel (fragment) that the geometry covers on screen.
// out vec4 FragColor: The final color of the pixel (Red, Green, Blue, Alpha).
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // Hardcoded orange color.
"}\n\0";

// FRAGMENT SHADER 2 (Yellow)
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n" // Hardcoded yellow color.
"}\n\0";

int main() {
	// 1. GLFW Init & Hints
	// ------------------------------------------------------------------------
	glfwInit(); // Initialize the GLFW library.
	
	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// Explicitly use the Core Profile (removes legacy functionality for cleaner code).
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	// Required for MacOS functionality
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 2. Create Window
	// ------------------------------------------------------------------------
	// Create a window object of width 800, height 600, named "Hell2077".
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hell2077", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // Clean up if window fails.
		return -1;
	}
	
	// Make the window's context the main context on the current thread.
	glfwMakeContextCurrent(window);
	
	// Register the callback function to handle window resizing events.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 3. Initialize GLAD
	// ------------------------------------------------------------------------
	// We pass GLAD the function to load the address of the OpenGL function pointers 
	// which is OS-specific (glfwGetProcAddress handles this).
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 4. Build and compile our shader program
	// ------------------------------------------------------------------------
	// Update filenames to match the files open in your workspace: "orange.frag" and "yellow.frag"
	Shader shaderOrange("default.vert", "orange.frag");
	Shader shaderYellow("default.vert", "yellow.frag");

	// 5. Set up vertex data (buffers and attribute pointers)
	// ------------------------------------------------------------------------

	// Vertices for first triangle (X, Y, Z coordinates)
	// OpenGL coordinates are usually between -1.0 and 1.0 (Normalized Device Coordinates).
	float vertices1[] = {
		-0.9f, -0.5f, 0.0f, // left
		-0.0f, -0.5f, 0.0f, // right 
		-0.45f, 0.5f, 0.0f, // top
	};

	// Vertices for second triangle
	float vertices2[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top
	};

	// Generate Identifiers for Buffers
	// VAO (Vertex Array Object): Stores the configuration of the attributes (pointers).
	// VBO (Vertex Buffer Object): Stores the raw data (vertices) in GPU memory.
	unsigned int VBOs[2], VAOs[2];
	
	// Ask OpenGL to give us 2 distinct IDs for VAOs and 2 for VBOs.
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs); 
	
	// --- SETUP FIRST TRIANGLE ---
	// 1. Bind the VAO first. Any subsequent VBO/Attrib configurations execute on this VAO.
	glBindVertexArray(VAOs[0]);
	
	// 2. Bind the VBO type (GL_ARRAY_BUFFER).
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	
	// 3. Copy our actual vertex data into the bound buffer's memory.
	// GL_STATIC_DRAW: Data won't change often, used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	
	// 4. Tell OpenGL how to interpret the vertex data.
	// Index 0: Matches layout(location = 0) in vertex shader.
	// 3: Size (vec3 has 3 floats).
	// GL_FLOAT: Type of data.
	// GL_FALSE: Don't normalize.
	// 3 * sizeof(float): Stride (byte offset between consecutive generic vertex attributes).
	// (void*)0: Offset of where the position data begins in the buffer.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// 5. Enable the attribute at location 0 (it's disabled by default).
	glEnableVertexAttribArray(0);

	// --- SETUP SECOND TRIANGLE ---
	// Bind the second VAO and VBO to configure the second shape separately.
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	
	// Configure pointer for the second buffer.
	// Stride is 0 here: OpenGL determines stride automatically because arrays are tightly packed.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind options (good practice but optional in simple programs).
	// glBindVertexArray(0); 

	// Uncomment to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 


	// 6. THE HOLY RENDER LOOP
	// ------------------------------------------------------------------------
	// Runs every frame until the user closes the window.
	while (!glfwWindowShouldClose(window))
	{
		// Input Processing
		processInput(window);

		// Rendering Commands
		// Set the color created when clearing the screen (State setting).
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Actually clear the color buffer using the color above (State using).
		glClear(GL_COLOR_BUFFER_BIT);

		// --- DRAW FIRST TRIANGLE ---
		shaderOrange.use(); // Activate the shader program for orange.
		
		// Bind the specific VAO containing the first triangle's data and config.
		glBindVertexArray(VAOs[0]); 
		
		// Draw the data.
		// GL_TRIANGLES: Primitive type.
		// 0: Start index in the enabled arrays.
		// 3: Number of vertices to draw.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// --- DRAW SECOND TRIANGLE ---
		shaderYellow.use(); // Switch to the yellow shader program.
		glBindVertexArray(VAOs[1]);        // Switch to the second triangle's VAO.
		glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the second triangle.

		// Swap Buffers
		// OpenGL draws to a back buffer (hidden) then swaps it to the front (visible).
		// This prevents flickering.
		glfwSwapBuffers(window);
		
		// Check for events (keyboard, mouse, window close) and updates state.
		glfwPollEvents();
	}

	// 7. De-allocation
	// ------------------------------------------------------------------------
	// Clean up resources that were allocated on the GPU.
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	// Clean up GLFW resources (destroys window).
	glfwTerminate();
	return 0;
}

// Helper: Process all input: query GLFW whether relevant keys are pressed/released.
void processInput(GLFWwindow* window)
{
	// If ESC is pressed, set the window close flag to true (breaking the while loop in main).
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Helper: Whenever the window size changes, this callback executes.
// width/height are the new dimensions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Tell OpenGL the size of the rendering window so it knows how we want to display the data and coordinates.
	// Arguments: lower-left corner (x,y) and width, height.
	glViewport(0, 0, width, height);
}

































