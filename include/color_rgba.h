#ifndef COLOR_RGBA_H
#define COLOR_RGBA_H

struct ColorRGBA
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
    unsigned int value;

    ColorRGBA(unsigned int _r = 0, unsigned int _g = 0, unsigned int _b = 0, unsigned int _a = 0)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
        updateValue();
    }

    void updateValue()
    {
        value = r|(g<<8)|(b<<16)|(a<<24);
    }
};

#endif
