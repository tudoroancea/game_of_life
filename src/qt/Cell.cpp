#include "Cell.hpp"
#include <Qt>

Qt::GlobalColor Cell_::color() const
{
    if (state_) {return Qt::white;}
    else {return Qt::black;}
}
