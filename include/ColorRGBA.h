struct ColorRGBA
{
    int r;
    int g;
    int b;
    int a;
    int value;

    ColorRGBA()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        value = 0;
    }

    ColorRGBA(int _r, int _g, int _b, int _a)
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
