/*
 * main.cc
 *
 *  Created on: 24/mar/2015
 *      Author: nicola
 */

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <utility>
#include "Image.hpp"
#include "Utils.hpp"
#include "DA3D.hpp"

using std::cerr;
using std::endl;
using std::min;
using std::max;
using std::copy;
using std::ostream_iterator;
using std::istream_iterator;
using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::getline;
using std::move;
using std::pair;
using std::tie;

using utils::pick_option;
using utils::read_image;
using utils::save_image;

using da3d::Image;
using da3d::DA3D;


int main(int argc, char **argv) {
  bool usage = static_cast<bool>(pick_option(&argc, argv, "h", nullptr));
  if (usage || argc < 4) {
    cerr << "usage: " << argv[0] << " noisy guide sigma output" <<
        endl;
    return EXIT_FAILURE;
  }

#ifndef _OPENMP
  cerr << "Warning: OpenMP not available. The algorithm will run in a single" <<
      " thread." << endl;
#endif

  Image input = read_image(argv[1]);
  Image guide = read_image(argv[2]);
  float sigma = atof(argv[3]);
  Image output = DA3D(input, guide, sigma);
  save_image(output, argc > 4 ? argv[4] : "-");
}
