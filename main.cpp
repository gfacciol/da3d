/*
 * main.cpp
 *
 *  Created on: 24/mar/2015
 *      Author: nicola pierazzo <nicola.pierazzo@cmla.ens-cachan.fr>
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
#include "DemoUtils.hpp"
#include "DA3D.hpp"

using std::cerr;
using std::endl;

using utils::pick_option;
using utils::read_image;
using utils::save_image;
using utils::isMonochrome;
using utils::makeMonochrome;

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
  // DA3D doesn't work if a color image has monochromatic noise
  if (input.channels()>1 && isMonochrome(input)) {
     cerr << "Warning: input color image has monochromatic noise! " << 
        "Converting to monochrome." << endl;
     input = makeMonochrome(input);
     guide = makeMonochrome(guide);
  }
  Image output = DA3D(input, guide, sigma);
  save_image(output, argc > 4 ? argv[4] : "-");
  return EXIT_SUCCESS;
}
