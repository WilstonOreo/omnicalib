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

#ifndef OMNIC_CALIBRATEDPROJECTOR_H_
#define OMNIC_CALIBRATEDPROJECTOR_H_

#include "Rect.h"
#include "PixelData.h"
#include "ColorCorrection.h"

namespace omnic {
  class CalibratedProjector {
  public:
    inline bool virtualScreen() const {
      return screenInfo_ & 1; // Virtual screen is first bit
    }

    /// Rectangle representing the screen geometry
    inline Rect const& screenGeometry() const {
      return screenGeometry_;
    }
    
    /// Rectangle representing the content placement inside the screen
    inline Rect const& contentGeometry() const {
      return contentGeometry_;
    }

    inline PixelData const& pixelData() const {
      return pixelData_;
    }

    inline ColorCorrection const& colorCorrection() const {
      return colorCorrection_;
    }
    
    /// Load from stream
    template<typename STREAM>
    inline void load(STREAM& _is, Version _version = Version::latest())
    {
      using namespace util;
      uint32_t screenInfo_ = 0;
      readBinary(_is,screenInfo_);
      screenGeometry_.load(_is,_version);
      contentGeometry_.load(_is,_version);
      colorCorrection_.load(_is,_version);
      pixelData_.load(_is,_version);
    }

  private:
    uint32_t screenInfo_ = 0;
    Rect screenGeometry_;
    Rect contentGeometry_;
    ColorCorrection colorCorrection_;
    PixelData pixelData_;
  };
}

#endif /* OMNIC_CALIBRATEDPROJECTOR_H_ */


