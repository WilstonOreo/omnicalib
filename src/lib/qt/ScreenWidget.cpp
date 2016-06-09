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
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
*/

#include <omnic/qt/ScreenWidget.h>

#include <omnic/qt/wrapper.h>

#include <QMatrix4x4>
#include <QOpenGLFramebufferObject>

namespace omnic {
  namespace qt {
    ScreenWidget::ScreenWidget(QWidget* _parent) :
      QOpenGLWidget(_parent) {
      removeProjectors();

    }

    ScreenWidget::~ScreenWidget() {
      removeProjectors();
    }
    
    QRect const& ScreenWidget::screenGeometry() const {
      return screenGeometry_;
    }
    
    bool ScreenWidget::isVirtual() const {
      return isVirtual_;
    }

    void ScreenWidget::addCalibratedProjector(omnic::CalibratedProjector const& _proj) {
      QRect _screenGeometry;
      wrap(_proj.screenGeometry(),_screenGeometry);
      
      /// Initialize widget geometry if this widget has no projectors assigned yet
      if (renderers_.empty()) {
        screenGeometry_ = _screenGeometry;
        // Setup window 
        if (!_proj.virtualScreen()) {
          setStyleSheet("* { background:black; }");
          setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
#ifdef Q_OS_LINUX
          setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
#endif
        } else { // Projector is virtual, so set the output size only
          setWindowFlags(Qt::Widget);
        }
        setGeometry(screenGeometry_);

        this->show();
      }

      if (screenGeometry_ == _screenGeometry) {
        projectors_.emplace_back(_proj);
        update();
      }
    }
    
    void ScreenWidget::removeProjectors() {
      renderers_.clear();
      projectors_.clear();
      this->hide();
    }

    void ScreenWidget::resizeGL(int _w, int _h) {
      makeCurrent();
      int _d = devicePixelRatio();
      int _width = screenGeometry_.width();
      int _height = screenGeometry_.height();
      glViewport(0, 0, _width * _d, _height * _d);
    }


    void ScreenWidget::initializeGL() {
      QOpenGLContext::globalShareContext()->makeCurrent(context()->surface());
      initializeOpenGLFunctions();
      resetOpenGLState();

      gl::Renderer::shader().initialize();
      QOpenGLContext::globalShareContext()->doneCurrent();
    }
    
    void ScreenWidget::setTexture(gl::TextureRef const& _tex) {
      tex_=_tex;
      update();
    }
    
    gl::TextureRef const& ScreenWidget::texture() const {
      return tex_;
    }

    void ScreenWidget::paintGL() {
      makeCurrent();
       
      int _w = screenGeometry_.width();
      int _h = screenGeometry_.height();
       
      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
      if (tex_.id() == 0) return;
      
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      QMatrix4x4 _m;
      _m.ortho(QRect(0,0,_w,_h));
      glMultMatrixf(_m.constData());
 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
     
      /// Work around for Qt OpenGL bug:
      // renderers need to be initialized so calibration texture is uploaded properly
      if (renderers_.size() != projectors_.size() || initializations_ < 2) {
        renderers_.clear();
        for (auto& _proj : projectors_) {
          renderers_.emplace_back(_proj);
        }
        ++initializations_;
      }

      for (auto& _renderer : renderers_) {   
        _renderer.render(tex_);
      }
    }

    void ScreenWidget::resetOpenGLState() {
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

          if (QOpenGLContext::currentContext()->isOpenGLES() ||
              (openGLFeatures() & QOpenGLFunctions::FixedFunctionPipeline)) {
            int maxAttribs;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

            for (int i = 0; i < maxAttribs; ++i) {
              glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 0, 0);
              glDisableVertexAttribArray(i);
            }
          }
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, 0);
          glDisable(GL_DEPTH_TEST);
          glDisable(GL_STENCIL_TEST);
          glDisable(GL_SCISSOR_TEST);
          glDisable(GL_CULL_FACE);
          glColorMask(true, true, true, true);
          glClearColor(0, 0, 0, 1);

          glDepthMask(true);
          glDepthFunc(GL_LESS);
          glClearDepthf(1);

          glStencilMask(0xff);
          glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
          glStencilFunc(GL_ALWAYS, 0, 0xff);
          glUseProgram(0);
          glDisable(GL_DEPTH_TEST);
          glDepthFunc(GL_LEQUAL);
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glEnable(GL_LINE_SMOOTH);
          glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
          glEnable(GL_POINT_SMOOTH);
          glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
          glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
          glEnable(GL_NORMALIZE);

          // fix outlines z-fighting with quads
          glPolygonOffset(1, 1);
          QOpenGLFramebufferObject::bindDefault();
    }
  }
}
