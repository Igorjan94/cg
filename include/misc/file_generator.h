#pragma once

#include <boost/filesystem.hpp>
#include <string>
#include <stdexcept>
#include <fstream>

namespace util {

    template<class T>
    void generate_files(std::string const& root,
                   T const& generator_function,
                   int count,
                   std::string const& prefix = "")
    {
        using namespace boost::filesystem;
        path p(root);
        if (!is_directory(p)) {
            throw std::invalid_argument("Given path " + p.string() + " is not a directory");
        }

        for (int i = 1; i <= count; ++i)
        {
            path file = p / (prefix + std::to_string(i));
            std::ofstream ofs(file.string());
            if (!generator_function(ofs, i)) {
                std::cerr << "Error generating file " << file;
            }
        }
    }

}
