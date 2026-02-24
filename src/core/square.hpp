#pragma once

struct Square
{
    int row, col;

    Square() : row(-1), col(-1)
    {
    }
    Square(int r, int c) : row(r), col(c)
    {
    }
};