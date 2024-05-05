#include <open_gl.h>

#include <color_rgba.h>
#include <vector2_int.h>

#include <iostream>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const unsigned int PIXEL_SIZE = 3;

void processInput(GLFWwindow *window);
void processDisplay();
void fillCanvas(ColorRGBA color = ColorRGBA());
void setPixel(int x, int y, ColorRGBA color);
void setPixel(Vector2Int p, ColorRGBA color);
void drawBox(Vector2Int pos, Vector2Int size, ColorRGBA color);

Vector2Int pos = Vector2Int(200, 50);

int main()
{
    openGL_init(CANVAS_WIDTH, CANVAS_HEIGHT, PIXEL_SIZE);

    Shader canvasShader("shaders/canvas.vert", "shaders/canvas.frag");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        processDisplay();

        openGL_update(canvasShader);
    }

    openGL_terminate();
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
    drawBox(pos, Vector2Int(10, 15), ColorRGBA(0, 255, 255, 255));
    setPixel(pos, ColorRGBA(255, 255, 255, 255));
}

void fillCanvas(ColorRGBA color)
{
    for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
        pixels[i] = color.value;
    }
}

void setPixel(int x, int y, ColorRGBA color)
{
    if (x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT) {
        pixels[x + y * CANVAS_WIDTH] = color.value;
    }
}

void setPixel(Vector2Int p, ColorRGBA color)
{
    if (p.x >= 0 && p.x < CANVAS_WIDTH && p.y >= 0 && p.y < CANVAS_HEIGHT) {
        pixels[p.x + p.y * CANVAS_WIDTH] = color.value;
    }
}

void drawBox(Vector2Int pos, Vector2Int size, ColorRGBA color)
{
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            setPixel(pos.x + x, pos.y + y, color);
        }
    }
}
