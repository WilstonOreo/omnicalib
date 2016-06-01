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
#ifndef OMNIC_CALIBRATION_H_
#define OMNIC_CALIBRATION_H_

#include <string>
#include <vector>
#include <iostream>
#include <omnic/CalibratedProjector.h>

namespace omnic {
  /// Calibration main struct. Holds a number of calibrated projectors. 
  class Calibration {
  public:
    /// Default, non-changable header size
    inline static constexpr size_t headerSize() {
      return 80;
    }

    Calibration() {
      // Set read-only header string
      std::string header_ = std::string("OMNIC_generated_by_omnidome_v" + std::string(OMNICALIBRATION_VERSION_STRING));
      header_.resize(headerSize(),' ');
    }

    /// Return 
    template<typename STREAM>
    inline void load(STREAM& _is, Version _version = Version::current()) {
      std::string _header(headerSize(),' ');
      _is.read(header_,headerSize());

      OMNIC_ASSERT(_header == header_);
      
      Version _readVersion;
      _is.read(_is,_readVersion);
      OMNIC_ASSERT(_readVersion < _version);

      uint32_t _numTunings = 0;
      readBinary(_is,_numTunings);
      projectors_.clear();
      for (uint32_t i = 0; i < _numTunings; ++i) {
        CalibratedProjector _proj;
        _proj.load(_is,_version);
        addCalibratedProjector(_proj);
      }
    }

    template<typename STREAM>
    void save(STREAM& _os, Version _version = Version::current()) const {
      _os.write(header_.c_str(),headerSize());
      _version.save(_os);
      _os << uint32_t(projectors_.size());
      
      for (auto& _projector : projectors_) {
        _projector.save(_os,_version);
      } 
    }

    inline std::vector<CalibratedProjector> const& projectors() const {
      return projectors_;
    }
    
    inline void addCalibratedProjector(CalibratedProjector const& _proj) {
      projectors_.push_back(_proj);
    }

    inline void setProjectors(std::vector<CalibratedProjector> const& _proj) {
      projectors_ = _proj;
    }

    inline std::string const& header() const {
      return header_;
    }


  private:
    /// Header string
    std::string header_;

    /// List of calibrated projectors
    std::vector<CalibratedProjector> projectors_;
  };
}

#endif /* OMNIC_CALIBRATION_H_ */
