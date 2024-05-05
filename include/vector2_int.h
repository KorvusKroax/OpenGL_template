#ifndef VECTOR2_INT_H
#define VECTOR2_INT_H

struct Vector2Int
{
    int x;
    int y;

    Vector2Int()
    {
        x = 0;
        y = 0;
    }

    Vector2Int(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};

#endif
