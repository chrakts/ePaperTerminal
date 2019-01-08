#include "info.h"

info::info()
{
    //ctor
}

info::~info()
{
    //dtor
}

info& info::operator=(const info& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
