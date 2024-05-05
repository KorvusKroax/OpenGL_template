#ifndef CANVAS_H
#define CANVAS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <color_rgba.h>
#include <vector2_int.h>

class Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        float pixelSize;

        unsigned int* pixels;

        unsigned int windowWidth;
        unsigned int windowHeight;
        GLFWwindow* window;

        Shader quadShader = Shader("shaders/canvas.vert", "shaders/canvas.frag");

        Canvas(unsigned int _width, unsigned int _height, float _pixelSize)
        {
            width = _width;
            height = _height;
            pixelSize = _pixelSize;

            pixels = new unsigned int[width * height];

            OpenGL_init();
            OpenGL_window(width * pixelSize, height * pixelSize);
            OpenGL_GLAD();
            OpenGL_quad();
        }

        ~Canvas()
        {
            OpenGL_terminate();
            delete [] pixels;
        }

        void update()
        {
            OpenGL_update();
        }

        void fillCanvas()
        {
            for (unsigned int i = 0; i < width * height; i++) {
                pixels[i] = 0;
            }
        }

        void fillCanvas(ColorRGBA color)
        {
            for (unsigned int i = 0; i < width * height; i++) {
                pixels[i] = color.value;
            }
        }

        void setPixel(Vector2Int p, ColorRGBA color)
        {
            if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
                pixels[p.x + p.y * width] = color.value;
            }
        }



    private:
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int renderbuffer;

        void OpenGL_init()
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        }

        int OpenGL_window(unsigned int _windowWidth, unsigned int _windowHeight)
        {
            windowWidth = _windowWidth;
            windowHeight = _windowHeight;
            window = glfwCreateWindow(windowWidth, windowHeight, "title", NULL, NULL);
            if (window == NULL) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
            }
            glfwMakeContextCurrent(window);
            return 0;
        }

        int OpenGL_GLAD()
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "Failed to initialize GLAD" << std::endl;
                return -1;
            }
            return 0;
        }

        void OpenGL_quad()
        {
            float quadVertices[] = {
                // positions    // texCoords
                -1.0f,  1.0f,   0.0f, 1.0f,
                -1.0f, -1.0f,   0.0f, 0.0f,
                1.0f, -1.0f,   1.0f, 0.0f,

                -1.0f,  1.0f,   0.0f, 1.0f,
                1.0f, -1.0f,   1.0f, 0.0f,
                1.0f,  1.0f,   1.0f, 1.0f
            };

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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

            glGenRenderbuffers(1, &renderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void OpenGL_update()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);

            quadShader.use();
            glBindVertexArray(quadVAO);
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        void OpenGL_terminate()
        {
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &quadVBO);
            glDeleteFramebuffers(1, &framebuffer);
            glDeleteRenderbuffers(1, &renderbuffer);
            glfwTerminate();
        }
};

#endif
