#include <sydevs/core/scale.h>
#include <array>
#include <string>
#include <ostream>

namespace sydevs {


scale& scale::operator++()
{
    ++level_;
    return *this;
}


scale scale::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}


scale& scale::operator--()
{
    --level_;
    return *this;
}


scale scale::operator--(int)
{
    auto old = *this;
    --(*this);
    return old;
}


scale& scale::operator+=(int64 rhs)
{
    level_ += rhs;
    return *this;
}


scale& scale::operator-=(int64 rhs)
{
    level_ -= rhs;
    return *this;
}


char scale::symbol() const
{
    static const std::array<char, 17> symbols = {{
        'y',
        'z',
        'a',
        'f',
        'p',
        'n',
        'u',
        'm',
        0,
        'k',
        'M',
        'G',
        'T',
        'P',
        'E',
        'Z',
        'Y',
    }};
    if (*this >= yocto && *this <= yotta) {
        return symbols[level_ + 8];
    }
    else {
        return 0;
    }
}
    
    
std::ostream& operator<<(std::ostream& os, const scale& rhs)
{
    static const std::array<std::string, 17> names = {{
        "yocto",
        "zepto",
        "atto",
        "femto",
        "pico",
        "nano",
        "micro",
        "milli",
        "unit",
        "kilo",
        "mega",
        "giga",
        "tera",
        "peta",
        "exa",
        "zetta",
        "yotta",
    }};
    if (rhs >= yocto && rhs <= yotta) {
        os << names[rhs.level() + 8];
    }
    else {
        os << "scale(" << rhs.level() << ")";
    }
    
    return os;
}


}  // namespace
