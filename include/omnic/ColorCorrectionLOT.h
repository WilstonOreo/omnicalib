/* Copyright (c) 2014-2016 "OmniCalibration" by cr8tr
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Simplified BSD license
 * Copyright (c) 2016
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OMNIC_COLORCORRECTIONLOT_H_
#define OMNIC_COLORCORRECTIONLOT_H_

#include <cassert>
#include <vector>
#include <omnic/util.h>

namespace omnic {
  /// Color correction information
  struct ColorCorrectionInfo {
    typedef float value_type;

    ColorCorrectionInfo() {}

    ColorCorrectionInfo(
        value_type _red,
        value_type _green,
        value_type _blue,
        value_type _all) :
      red_(_red),
      green_(_green),
      blue_(_blue),
      all_(_all) {}

    /// Red value
    inline value_type red() const {
      return red_;
    }

    /// Green value
    inline value_type green() const {
      return green_;
    }

    /// Blue value
    inline value_type blue() const {
      return blue_;
    }
    /// All value
    inline value_type all() const {
      return all_;
    }

  private:
    float red_;
    float green_;
    float blue_;
    float all_;
  };

  /// Look up table for color correction
  struct ColorCorrectionLOT {

    inline static constexpr size_t maxSize() { return 65536; }
    inline static constexpr size_t minSize() { return 256; }
    inline static constexpr size_t defaultSize() { return 1024; }

    inline uint32_t size() const {
      return data_.size();
    }

    inline const void* ptr() const {
      return (void const*)data_.data();
    }

    inline std::vector<ColorCorrectionInfo> const& data() const {
      return data_;
    }

    template<typename T>
    void setData(T const& _data) {
      data_ = _data;
    }

    template<typename STREAM>
    void save(STREAM& _os, Version = Version::latest()) const {
      util::writeBinary(_os,uint32_t(data_.size()));
      _os.write((char const*)(data_.data()),data_.size() * sizeof(ColorCorrectionInfo));
    }

    template<typename STREAM>
    void load(STREAM& _is, Version = Version::latest()) {
      using namespace util;
      uint32_t size_ = 0;
      readBinary(_is,size_);
      assert(size_ <= maxSize());
      data_.resize(size_);
      _is.read((char*)(data_.data()),data_.size() * sizeof(ColorCorrectionInfo));
    }

  private:
    std::vector<ColorCorrectionInfo> data_;
  };
}

#endif /* OMNIC_COLORCORRECTIONLOT_H_ */
