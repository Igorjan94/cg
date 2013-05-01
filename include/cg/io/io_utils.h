#pragma once

#include <iostream>

namespace cg
{
    namespace io
    {

        inline std::istream& skip_char(std::istream& in, char ch)
        {
            char c;

            while ((in >> c) && (c != ch));

            return in;
        }


        class Printer
        {
            std::ostream& _stream;

        public:
            template<class T>
            Printer(T& stream)
                : _stream(stream)
            {}

            void operator() () {}

            template<class U, class ... Args>
            void operator() (U const& u, Args const& ... args)
            {
                _stream << u;
                (*this)(args...);
            }
        };

    }
}
