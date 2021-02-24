#include "GLWindow.h"

GLWindow::GLWindow()
{
	width = 800;
	height = 600;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

int GLWindow::Initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW initialization failed!");
        glfwTerminate();
        return 1;
    }


    // Setup GLDW window properties
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile so no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Enable forward compatibility 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed.");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle key and mouse inputs
    createCallbacks();

    // Hide and disable the cursor
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed.");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
}

void GLWindow::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat GLWindow::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;

    return theChange;
}

GLfloat GLWindow::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;

    return theChange;
}

void GLWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void GLWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    // opposite calculations for change because otherwise Y movement would be inverted
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}