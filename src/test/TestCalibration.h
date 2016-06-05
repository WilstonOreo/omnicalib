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

#include <omnic/Calibration.h>

namespace omnic {
  /// Generate artificial sample calibration for testing
  static Calibration testCalibration() {
    Calibration _calib;

    auto _generateProjector = [&_calib](
        Rect const& _screenGeometry,
        Rect const& _contentGeometry) -> CalibratedProjector {
      CalibratedProjector _proj;
      _proj.setVirtualScreen(true);

      uint32_t _w = _contentGeometry.width();
      uint32_t _h = _contentGeometry.height();

      _proj.setContentGeometry(_contentGeometry);
      _proj.setScreenGeometry(_screenGeometry);

      PixelData::data_type _data(_w*_h);
      for (uint32_t y = 0; y < _h; ++y)
        for (uint32_t x = 0; x < _w; ++x) {
          _data[y*_w + x] = UVDBPixel(x * 65536.0 / _w, y * 65536.0 / _h,0,x * 65536.0 / _w);
        }
      _proj.setPixelData(PixelData(_w,_h,_data));

      ColorCorrectionLOT _colorCorrection;

      /// Generate Color Correction Data
      ColorCorrectionLOT::data_type _ccData;

      size_t _size = ColorCorrectionLOT::defaultSize();
      for (size_t i = 0; i < _size; ++i) {
        float _v = i / float(_size);
        _ccData.emplace_back(_v,_v,_v,_v);
      }

      _colorCorrection.setData(_ccData);
      _proj.setColorCorrection(_colorCorrection);

      return _proj;
    };

    _calib.addCalibratedProjector(_generateProjector(
          Rect(0,0,1920,1080),
          Rect(0,0,1920,1080)));

    _calib.addCalibratedProjector(_generateProjector(
          Rect(1920,0,1920,1080),
          Rect(0,0,960,1080)));

    _calib.addCalibratedProjector(_generateProjector(
          Rect(1920,0,1920,1080),
          Rect(960,0,960,1080)));

    return _calib;
  }
}
