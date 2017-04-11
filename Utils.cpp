/*
 * Utils.cpp
 *
 *  Created on: 12/feb/2015
 *      Author: nicola
 */

#include <cmath>
#include "Utils.hpp"

using std::vector;
using std::pair;
using std::move;
using std::max;
using std::min;
using da3d::Image;
using da3d::WeightMap;

namespace utils {

/*! \brief Compute best tiling of image in at most ntiles
 *
 *  Returns the pair: rows, columns
 */
pair<int, int> ComputeTiling(int rows, int columns, int ntiles) {
  // The objective is extract ntiles square-ish tiles
  // For a square image the optimal number of rows is sqrt(ntiles)
  // The ratio rows/columns permits to handle rectangular images
  float best_r = sqrt(static_cast<float>(ntiles * rows) / columns);
  int r_low = static_cast<int>(best_r);
  int r_up = r_low + 1;
  if (r_low < 1) return {1, ntiles};      // single row
  if (r_up > ntiles) return {ntiles, 1};  // single column
  // look for the nearest integer divisors of ntiles
  while (ntiles % r_low != 0) --r_low;
  while (ntiles % r_up != 0) ++r_up;
  // this criterion selects between r_low and r_up rows
  // if   r_up * r_low > best_r^2
  // then r_up is too large so r_low is selected
  // else r_low is too small and r_up is selected
  if (r_up * r_low * columns > ntiles * rows) {
    return {r_low, ntiles / r_low};
  } else {
    return {r_up, ntiles / r_up};
  }
}

/*! \brief Split image in tiles
 *
 *  Returns a vector containing tiling.first x tiling.sencond images
 *  each padded by pad_* pixels. Tiles are stored in lexicographic order.
 *  Padding outside the image is done by symmetrization
 */
vector<Image> SplitTiles(const Image &src,
                         int pad_before,
                         int pad_after,
                         pair<int, int> tiling) {
  vector<Image> result;
  for (int tr = 0; tr < tiling.first; ++tr) {
    int rstart = src.rows() * tr / tiling.first - pad_before;
    int rend = src.rows() * (tr + 1) / tiling.first + pad_after;
    for (int tc = 0; tc < tiling.second; ++tc) {
      int cstart = src.columns() * tc / tiling.second - pad_before;
      int cend = src.columns() * (tc + 1) / tiling.second + pad_after;
      // copy image to tile using the above computed limits
      Image tile(rend - rstart, cend - cstart, src.channels());
      for (int row = rstart; row < rend; ++row) {
        for (int col = cstart; col < cend; ++col) {
          for (int ch = 0; ch < src.channels(); ++ch) {
            tile.val(col - cstart, row - rstart, ch) = src.val(
                SymmetricCoordinate(col, src.columns()),
                SymmetricCoordinate(row, src.rows()),
                ch);
          }
        }
      }
      result.push_back(move(tile));
    }
  }
  return result;
}

/*! \brief Recompose tiles produced by SplitTiles
 *
 *  Returns an image resulting of recomposing the tiling
 *  padded margins are averaged in the result
 */
Image MergeTiles(const vector<pair<Image, Image>> &src,
                 pair<int, int> shape,
                 int pad_before,
                 int pad_after,
                 pair<int, int> tiling) {
  int channels = src[0].first.channels();
  Image result(shape.first, shape.second, channels);
  Image weights(shape.first, shape.second);
  auto tile = src.begin();
  for (int tr = 0; tr < tiling.first; ++tr) {
    int rstart = shape.first * tr / tiling.first - pad_before;
    int rend = shape.first * (tr + 1) / tiling.first + pad_after;
    for (int tc = 0; tc < tiling.second; ++tc) {
      int cstart = shape.second * tc / tiling.second - pad_before;
      int cend = shape.second * (tc + 1) / tiling.second + pad_after;
      // copy tile to image using the above computed limits
      for (int row = max(0, rstart); row < min(shape.first, rend); ++row) {
        for (int col = max(0, cstart); col < min(shape.second, cend); ++col) {
          for (int ch = 0; ch < channels; ++ch) {
            result.val(col, row, ch) +=
                tile->first.val(col - cstart, row - rstart, ch);
          }
          weights.val(col, row) += tile->second.val(col - cstart, row - rstart);
        }
      }
      ++tile;
    }
  }
  // normalize by the weight
  for (int row = 0; row < shape.first; ++row) {
    for (int col = 0; col < shape.second; ++col) {
      for (int ch = 0; ch < channels; ++ch) {
        result.val(col, row, ch) /= weights.val(col, row);
      }
    }
  }
  return result;
}

bool isMonochrome (const Image &u) {
  for (int row = 0; row < u.rows(); ++row) {
    for (int col = 0; col < u.columns(); ++col) {
      float v = u.val(col, row, 0);
      for (int ch = 1; ch < u.channels(); ++ch) {
        if (u.val(col, row, ch) != v)  {
           return false;
        }
      }
    }
  }
  return true;
}

Image makeMonochrome (const Image &u) {
  Image result(u.columns(), u.rows());
  for (int row = 0; row < u.rows(); ++row) {
    for (int col = 0; col < u.columns(); ++col) {
      double v = 0;
      for (int ch = 1; ch < u.channels(); ++ch) {
        v += u.val(col, row, ch);
      }
      result.val(col,row) = v / u.channels();
    }
  }
  return result;
}

}  // namespace utils
