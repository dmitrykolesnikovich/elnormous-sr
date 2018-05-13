//
//  SoftwareRenderer
//

#include "Size3.hpp"
#include "Size2.hpp"

namespace sr
{
    Size3::Size3(const Size2& s):
        width(s.width), height(s.height)
    {
    }

    Size3& Size3::operator=(const Size2& s)
    {
        width = s.width;
        height = s.height;
        depth = 0.0F;
        return *this;
    }
}
