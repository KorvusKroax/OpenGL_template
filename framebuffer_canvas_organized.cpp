#include <canvas.h>
#include <open_gl.h>
#include <iostream>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const float PIXEL_SCALE = 3;

Canvas canvas = Canvas(CANVAS_WIDTH, CANVAS_HEIGHT);

void processInput(GLFWwindow *window);
void processDisplay();

Vector2Int pos = Vector2Int(200, 50);

int main()
{
    openGL_init(canvas.width, canvas.height, canvas.pixels, PIXEL_SCALE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        processDisplay();

        openGL_update();
    }

    openGL_terminate();
    return 0;
}

void processInput(GLFWwindow* window)
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
    canvas.fillCanvas();
    canvas.setPixel(pos, ColorRGBA(255, 255, 255, 255));
    canvas.drawCircle(pos, 20, ColorRGBA(0, 255, 255, 255));
    canvas.drawLine(pos, Vector2Int(canvas.width * .5f, canvas.height * .5f), ColorRGBA(255, 255, 0, 255));
}
