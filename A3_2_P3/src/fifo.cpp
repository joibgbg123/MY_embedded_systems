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
        if (is_full())
        {
            cought_up = false;
        }

        int curr_val = *pCurrent_take;
        if (pCurrent_take == (pBegin + FIFO_SIZE - 1))
        {
            pCurrent_take = pBegin;
        }
        else
        {
            pCurrent_take += 1;
        }
        return curr_val;
    }
}

void Fifo::put(int item)
{

    *pCurrent_add = item;

    if (pCurrent_add == (pBegin + FIFO_SIZE - 1))
    {
        if (is_full())
        {
            pCurrent_take = pBegin;
        }
        pCurrent_add = pBegin;
    }
    else
    {
        if (is_full())
        {
            pCurrent_take += 1;
        }
        pCurrent_add += 1;
    }

    if (pCurrent_take == pCurrent_add)
    {
        cought_up = true;
    }
}

bool Fifo::is_empty()
{
    if ((pCurrent_take == pCurrent_add) & (!cought_up))
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
    if ((pCurrent_take == pCurrent_add) & (cought_up))
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
    pCurrent_take = pCurrent_add;
    cought_up = false;
}