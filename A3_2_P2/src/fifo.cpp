#include "fifo.h"

Fifo::Fifo()
{
}

int Fifo::get()
{
    if (is_empty())
    {
        return 0; // Error, should not be requested
    }
    else
    {
        pCurrent_take -= 1;
        return *(pCurrent_take + 1);
    }
}

void Fifo::put(int item)
{
    if (is_full()) // if the buffer if full, *current_take must be interpreted as one lower to move up with the whole queue
    {
        pCurrent_take -= 1;
    }

    for (int k = 0; pBegin <= (pCurrent_take - k); k++)
    {
        *(pCurrent_take + 1 - k) = *(pCurrent_take - k); //Every item is shifted to the right
    }
    *pBegin = item; // The new value is placed at the front
    pCurrent_take += 1;   //One is added to the pointer value
}

bool Fifo::is_empty()
{
    if (pCurrent_take == (pBegin - 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Fifo::is_full()
{
    if (pCurrent_take == (pBegin + FIFO_SIZE - 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Fifo::reset()
{
    pCurrent_take = pBegin - 1;
}