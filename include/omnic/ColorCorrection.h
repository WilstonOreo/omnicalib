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
#ifndef OMNIC_COLORCORRECTION_H_
#define OMNIC_COLORCORRECTION_H_

#include "ChannelCorrection.h"

namespace omnic {
  /// Color Correction with correction for each channel and overall color correction
  struct ColorCorrection {

    /// Load from stream
    template<typename T>
    inline void load(STREAM& _is, Version _version = Version::latest()) {
      red_.load(_is,_version);
      green_.load(_is,_version);
      blue_.load(_is,_version);
      all_.load(_is,_version);
    }

    /// Return correction for red channel
    inline ChannelCorrection const& red() const {
      return red_;
    }

    /// Return correction for green channel
    inline ChannelCorrection const& green() const {
      return green_;
    }
    
    /// Return correction for blue channel
    inline ChannelCorrection const& blue() const {
      return blue_;
    }
    
    /// Return correction for all channel
    inline ChannelCorrection const& all() const {
      return all_;
    }

    template<typename T>
    inline void calculateLookUpTable(T& _t, uint32_t _quantization = 256) {
      red_.calculateLookUpTable(_t,_quantization);
      green_.calculateLookUpTable(_t,_quantization);
      blue_.calculateLookUpTable(_t,_quantization);
      all_.calculateLookUpTable(_t,_quantization);
    }


  private:
    ChannelCorrection red_, green_, blue_, all_;
  };
}

#endif /* OMNIC_COLORCORRECTION_H_ */
