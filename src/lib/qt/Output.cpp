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

#include <omnic/qt/Output.h>

#include <omnic/qt/wrapper.h>

namespace omnic {
  namespace qt {
    Output::Output() : texture_(0,0,0,GL_TEXTURE_RECTANGLE) {
    }

    Output::~Output() {
    }
    
    void Output::showScreens() {
      for (auto& _widget : widgets_) {
        _widget->show();
      }
    }

    void Output::hideScreens() {
      for (auto& _widget : widgets_) {
        _widget->hide();
      }
    }

    void Output::showVirtualScreens() {
      for (auto& _widget : widgets_) {
        if (_widget->isVirtual()) _widget->show();
      }
    }
    
    void Output::hideVirtualScreens() {
      for (auto& _widget : widgets_) {
        if (_widget->isVirtual()) _widget->hide();
      }
    }
        
    void Output::setCalibration(omnic::Calibration const& _calib) {
      removeCalibration();

      for (auto& _proj : _calib.projectors()) {
        auto* _widget = getWidgetFromScreen(_proj.screenGeometry());

        if (!_widget) {
          widgets_.emplace_back(new ScreenWidget());
          _widget = widgets_.back().get();
        }

        _widget->addCalibratedProjector(_proj);
      }
    }

    omnic::Calibration const& Output::calibration() const {
      return calibration_;
    }
    
    void Output::removeCalibration() {
      calibration_ = omnic::Calibration();
      widgets_.clear();
    }

    void Output::setTexture(
        GLuint _id,
        uint32_t _width,
        uint32_t _height,
        GLuint _target) {   
      setTexture(gl::TextureRef(_id,_width,_height,_target));
    }

    void Output::setTexture(gl::TextureRef const& _texture) {
      texture_=_texture;
      for (auto& _widget : widgets_) {
        _widget->setTexture(texture_);
      }
    }

    gl::TextureRef const& Output::texture() const {
      return texture_;
    }

    void Output::update() {
      for (auto& _widget : widgets_) {
        _widget->update();
      } 
    }
    
    ScreenWidget* Output::getWidgetFromScreen(omnic::Rect const& _rect) {
      for (auto& _widget : widgets_) {
        if (wrap<QRect>(_rect) == _widget->screenGeometry()) {
          return _widget.get();
        }
      }

      return nullptr;    
    }
  }
}


