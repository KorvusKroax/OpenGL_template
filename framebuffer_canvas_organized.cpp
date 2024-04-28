#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>

#include <canvas.h>

const unsigned int WIDTH = CANVAS_WIDTH * PIXEL_SIZE;
const unsigned int HEIGHT = CANVAS_HEIGHT * PIXEL_SIZE;

void processInput(GLFWwindow *window);
void processDisplay();

Vector2Int pos = Vector2Int(200, 50);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "title", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfwSetWindowPos(window, 100, 100);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader canvasShader("shaders/canvas.vert", "shaders/canvas.frag");

    float canvasVertices[] = {
        // positions    // texCoords
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f
    };

    unsigned int canvasVAO;
    glGenVertexArrays(1, &canvasVAO);
    glBindVertexArray(canvasVAO);

    unsigned int canvasVBO;
    glGenBuffers(1, &canvasVBO);
    glBindBuffer(GL_ARRAY_BUFFER, canvasVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(canvasVertices), &canvasVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CANVAS_WIDTH, CANVAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, CANVAS_WIDTH, CANVAS_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        processDisplay();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        canvasShader.use();
        glBindVertexArray(canvasVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CANVAS_WIDTH, CANVAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &canvasVAO);
    glDeleteBuffers(1, &canvasVBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbuffer);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (pos.y < CANVAS_HEIGHT - 1) pos.y++;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (pos.x < CANVAS_WIDTH - 1) pos.x++;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (pos.y > 0) pos.y--;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (pos.x > 0) pos.x--;
    }
}

void processDisplay()
{
    fillCanvas();
    setPixel(pos, ColorRGBA(255, 255, 100, 255));
}
