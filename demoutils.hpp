//
// Created by Nicola Pierazzo on 21/10/15.
//

#ifndef UTILS_DEMOUTILS_HPP
#define UTILS_DEMOUTILS_HPP

#include "Image.hpp"
#include <string>

namespace utils {

const char *pick_option(int *c, char **v, const char *o, const char *d);
da3d::Image read_image(const std::string& filename);
void save_image(const da3d::Image& image, const std::string& filename);

}  // namespace imgutils

#endif //UTILS_DEMOUTILS_HPP
