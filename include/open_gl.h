#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

unsigned int canvasWidth;
unsigned int canvasHeight;
float pixelScale;

static int* pixels;

unsigned int windowWidth;
unsigned int windowHeight;
GLFWwindow* window;

float quadVertices[] = {
    // positions    // texCoords
    -1.0f,  1.0f,   0.0f, 1.0f,
    -1.0f, -1.0f,   0.0f, 0.0f,
     1.0f, -1.0f,   1.0f, 0.0f,

    -1.0f,  1.0f,   0.0f, 1.0f,
     1.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  1.0f,   1.0f, 1.0f
};

unsigned int quadVAO;
unsigned int quadVBO;
unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int renderbuffer;

void openGL_init(unsigned int _canvasWidth, unsigned int _canvasHeight, float _pixelScale)
{
    canvasWidth = _canvasWidth;
    canvasHeight = _canvasHeight;
    pixelScale = _pixelScale;

    pixels = new int[canvasWidth * canvasHeight];

    windowWidth = canvasWidth * pixelScale;
    windowHeight =  canvasHeight * pixelScale;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);



    window = glfwCreateWindow(windowWidth, windowHeight, "title", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    // glfwSetWindowPos(window, 100, 100);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvasWidth, canvasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, canvasWidth, canvasHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void openGL_update(Shader quadShader)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    quadShader.use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvasWidth, canvasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void openGL_terminate()
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbuffer);

    glfwTerminate();
}
