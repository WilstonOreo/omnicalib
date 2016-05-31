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

#ifndef OMNIC_CALIBRATEDPROJECTOR_H_
#define OMNIC_CALIBRATEDPROJECTOR_H_

#include "Rect.h"
#include "PixelData.h"
#include "ColorCorrectionLOT.h"

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

    inline ColorCorrectionLOT const& colorCorrection() const {
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
    
    /// Save to stream
    template<typename STREAM>
    inline void save(STREAM& _is, Version _version = Version::latest()) const
    {
      using namespace util;
      uint32_t screenInfo_ = 0;
      writeBinary(_is,screenInfo_);
      screenGeometry_.save(_is,_version);
      contentGeometry_.save(_is,_version);
      colorCorrection_.save(_is,_version);
      pixelData_.save(_is,_version);
    }

  private:
    uint32_t screenInfo_ = 0;
    Rect screenGeometry_;
    Rect contentGeometry_;
    ColorCorrectionLOT colorCorrection_;
    PixelData pixelData_;
  };
}

#endif /* OMNIC_CALIBRATEDPROJECTOR_H_ */


