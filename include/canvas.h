#ifndef CANVAS_H
#define CANVAS_H

#include <cstdlib>
#include <color_rgba.h>
#include <vector2_int.h>

class Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        int* pixels = nullptr;

        Canvas(unsigned int _width, unsigned int _height)
        {
            width = _width;
            height = _height;
            pixels = new int[width * height];
        }

        ~Canvas()
        {
            delete []pixels;
        }

        void fillCanvas(ColorRGBA color = ColorRGBA())
        {
            for (int i = 0; i < width * height; i++) {
                pixels[i] = color.value;
            }
        }

        void setPixel(int x, int y, ColorRGBA color)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                pixels[x + y * width] = color.value;
            }
        }

        void setPixel(Vector2Int p, ColorRGBA color) { setPixel(p.x, p.y, color); }

        void drawLine(int x1, int y1, int x2, int y2, ColorRGBA color)
        {
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;

            if (dx > dy) {
                int y = y1;
                int d = dy * 2 - dx;
                for (int x = x1; x != x2; x += sx) {
                    setPixel(x, y, color);
                    if (d > 0) {
                        y += sy;
                        d -= dx * 2;
                    }
                    d += dy * 2;
                }
            } else {
                int x = x1;
                int d = dx * 2 - dy;
                for (int y = y1; y != y2; y += sy) {
                    setPixel(x, y, color);
                    if (d > 0) {
                        x += sx;
                        d -= dy * 2;
                    }
                    d += dx * 2;
                }
            }
        }

        void drawLine(Vector2Int p1, Vector2Int p2, ColorRGBA color) { drawLine(p1.x, p1.y, p2.x, p2.y, color); }

        void drawCircle(int x, int y, int r, ColorRGBA color)
        {
            int xx = -r;
            int yy = 0;
            int rr = 2 - 2 * r;
            do {
                setPixel(x - xx, y + yy, color);
                setPixel(x - yy, y - xx, color);
                setPixel(x + xx, y - yy, color);
                setPixel(x + yy, y + xx, color);
                r = rr;
                if (r <= yy) rr += ++yy * 2 + 1;
                if (r > xx || rr > yy) rr += ++xx * 2 + 1;
            } while (xx < 0);
        }

        void drawCircle(Vector2Int p, int r, ColorRGBA color) { drawCircle(p.x, p.y, r, color); }
};

#endif
