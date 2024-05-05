#ifndef COLOR_RGBA_H
#define COLOR_RGBA_H

struct ColorRGBA
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
    unsigned int value;

    ColorRGBA()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        value = 0;
    }

    ColorRGBA(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a)
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
