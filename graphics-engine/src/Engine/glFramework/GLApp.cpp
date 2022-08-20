#include "GLApp.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GLWindow.h"
#include <Engine/Input.h>
#include <Engine/Time.h>
#include <Logging/Log.h>

GLApp::~GLApp()
{
    glfwTerminate();
}

int GLApp::run()
{
    // Initialize
    if (init() == false)
        return EXIT_FAILURE;

    // Run main loop
    mainLoop();

    quit();

    return EXIT_SUCCESS;
}

bool GLApp::init()
{
    log_reset();                                    // reset log file
    if (!initGlfw() || !window.init()) // intialize all components
        return false;

    inputInit(); // create sample input axes/actions

    return true;
}

bool GLApp::initGlfw()
{
    if (glfwInit() == GLFW_FALSE)
    {
        log_error("ERROR::GLFW::INITIALIZATION");
        return false;
    }
    return true;
}

void GLApp::quit()
{
    glfwTerminate();
}

void GLApp::inputInit()
{
    // Some inputs for testing
    Input::createAction("QUIT", GLFW_KEY_ESCAPE);
}

#include <glad/glad.h>

static const char* shaderCodeVertex = R"(
#version 460 core
layout (location=0) out vec3 color;
const vec2 pos[3] = vec2[3](
	vec2(-0.6, -0.4),
	vec2( 0.6, -0.4),
	vec2( 0.0,  0.6)
);
const vec3 col[3] = vec3[3](
	vec3( 1.0, 0.0, 0.0 ),
	vec3( 0.0, 1.0, 0.0 ),
	vec3( 0.0, 0.0, 1.0 )
);
void main()
{
	gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
	color = col[gl_VertexID];
}
)";

static const char* shaderCodeFragment = R"(
#version 460 core
layout (location=0) in vec3 color;
layout (location=0) out vec4 out_FragColor;
void main()
{
	out_FragColor = vec4(color, 1.0);
};
)";

void GLApp::mainLoop()
{
    const GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &shaderCodeVertex, nullptr);
    glCompileShader(shaderVertex);

    const GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &shaderCodeFragment, nullptr);
    glCompileShader(shaderFragment);

    const GLuint program = glCreateProgram();
    glAttachShader(program, shaderVertex);
    glAttachShader(program, shaderFragment);

    glLinkProgram(program);
    glUseProgram(program);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, window.getWidth(), window.getHeight());

    while (running)
    {
        if (Input::getAction("QUIT")) {
            running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        Input::resetMouse();
        Time::tick();
        window.tick();
    }
    glDeleteProgram(program);
    glDeleteShader(shaderFragment);
    glDeleteShader(shaderVertex);
    glDeleteVertexArrays(1, &vao);

    window.setShouldClose(true);
}