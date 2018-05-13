//
//  SoftwareRenderer
//

#include "Size2.hpp"
#include "Size3.hpp"

namespace sr
{
    Size2::Size2(const Size3& s):
        width(s.width), height(s.height)
    {
    }

    Size2& Size2::operator=(const Size3& s)
    {
        width = s.width;
        height = s.height;
        return *this;
    }
}
