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
#ifndef OMNIC_CHANNELCORRECTION_H_
#define OMNIC_CHANNELCORRECTION_H_

#include "util.h"

namespace omnic
{
  /// Correction for a single color channel
  struct ChannelCorrection
  {
  public:
    /// Gamma correction from -1.0 to 1.0
    inline float gamma() const
    {
      return gamma_;
    }

    /// Brightness correction from -1.0 to 1.0
    inline float brightness() const
    {
      return brightness_;
    }

    /// Contrast correction from -1.0 to 1.0
    inline float contrast() const
    {
      return contrast_;
    }

    /// Correction multiplier from 0.0 to 1.0
    inline float multiplier() const
    {
      return multiplier_;
    }

    /// Load from stream
    template<typename STREAM>
    inline void load(STREAM& _is, Version _version = Version::latest())
    {
      using namespace util;
      readBinary(_is,gamma_);
      keepInRange(gamma_);
      readBinary(_is,brightness_);
      keepInRange(brightness_);
      readBinary(_is,contrast_);
      keepInRange(contrast_);
      readBinary(_is,multiplier_);
      keepInRange(multiplier_,0.0f,1.0f);
    }

    inline float brightness(float v, float brightness_value)
    {
      return std::max(v + brightness_value,0.0f);
    }

    /// Calculate contrast corrected value
    inline float contrast(float v, float contrast_value)
    {
      float _c = (contrast_value <= 0.0) ?
                 contrast_value + 1.0 :
                 (1.0 / (1.0 - contrast_value));
      return (v - 0.5) * std::max(_c, 0.0f) + 0.5;
    }

    /// Calculate gamma corrected value
    inline float gamma(float v, float gamma_value)
    {
      float g = 0.0;
      if (gamma_value >= 0.0)
      {
        g = 1.0 / (1.0 + gamma_value);
      }
      else
      {
        g = (1.0 - gamma_value);
      }
      return pow(v,g*g*g*g);
    }

    /// Calculate corrected pixel value from given pixel value
    inline float corrected(float v)
    {
      return brightness(contrast(gamma(v,
                                       gamma() * multiplier()),
                                 contrast() * multiplier()),
                        brightness ()* multiplier());
    }

    /// Appends lookup table data to given input vector of float
    template<typename T>
    inline void calculateLookUpTable(T& _t, uint32_t _quantization = 256) {
      size_t _start = _t.size();
      _t.resize(_t.size() + _quantization);
      size_t _end = _t.size();

      for (size_t i = 0; i < _quantization; ++i) {
        _t[i + _start] = corrected(i / float(_quantization));
      }
    }

  private:
    /// Clamps value of a member in min/max range
    inline static void keepInRange(float& _f, float _min = -1.0, float _max = 1.0)
    {
      _f = util::clamp(_f,_min,_max);
    }

    float gamma_ = 0.0;
    float brightness_ = 0.0;
    float contrast_ = 0.0;
    float multiplier_ = 0.0;
  }
};

}

#endif /* OMNIC_CHANNELCORRECTION_H_ */
