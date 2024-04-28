#include <Vector2Int.h>
#include <ColorRGBA.h>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const unsigned int PIXEL_SIZE = 3;

static unsigned int canvas[CANVAS_WIDTH * CANVAS_HEIGHT];

void fillCanvas(ColorRGBA color = ColorRGBA())
{
    for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
        canvas[i] = color.value;
    }
}

void setPixel(Vector2Int p, ColorRGBA color)
{
    if (p.x >= 0 && p.x < CANVAS_WIDTH && p.y >= 0 && p.y < CANVAS_HEIGHT) {
        canvas[p.x + p.y * CANVAS_WIDTH] = color.value;
    }
}
