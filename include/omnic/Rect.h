/* Copyright (c) 2014-2016 "OmniCalibration" by cr8tr
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 * 
 * The MIT License (MIT)
 * Copyright (c) 2016 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef OMNIC_RECT_H_
#define OMNIC_RECT_H_

#include "util.h"

namespace omnic {
  /// Rectangle struct for screen and content rectangles
  struct Rect {
  public:
    /// Offset in X direction
    inline int32_t offsetX() const {
      return offsetX_;
    }

    /// Offset in Y direction
    inline int32_t offsetY() const {
      return offsetY_;
    }

    /// Width of rectangle
    inline uint32_t width() const {
      return width_;
    }

    /// Height of rectangle
    inline uint32_t height() const {
      return height_;
    }
    
    /// Load rectangle from stream
    template<typename STREAM>
    void load(STREAM& _is) {
      using namespace util;
      readBinary(_is,offsetX_);
      readBinary(_is,offsetY_);
      readBinary(_is,width_);
      readBinary(_is,height_);
    }

  private:
    int32_t offsetX_;
    int32_t offsetY_;
    uint32_t width_;
    uint32_t height_;
  };
}



#endif /* OMNIC_RECT_H_ */
