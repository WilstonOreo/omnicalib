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

#include "sample.h"

#include <fstream>
#include <chrono>
#include <QTimer>
#include <QFile>

namespace omnic {
  Sample::Sample(QString const& _filename) : size_(2048,2048) {
    loadCalibration(_filename);
    startTime_ = now(); 
  }

  Sample::~Sample() {  
  }

  void Sample::loadCalibration(QString const& _filename) {
    omnic::Calibration _calibration;
    
    std::ifstream _if(_filename.toStdString());
    _calibration.load(_if);
    _if.close();

    output_.setCalibration(_calibration);  

    timer_.reset(new QTimer());
    connect(timer_.get(),SIGNAL(timeout()),this,SLOT(render()));
    timer_->start(20);
  }

  double Sample::now() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count()
               /
               1000000000.0;
      }
  
    /// Return content of file from a file name
    QString Sample::fileToStr(const QString& _filename)
    {
      QFile _f(_filename);
      _f.open(QIODevice::ReadOnly | QIODevice::Text);
      return _f.readAll();
    }


  
  void Sample::render() {
    auto* _context = QOpenGLContext::globalShareContext();
    if (!_context || !QOpenGLContext::currentContext()) return;
    
    int _w = size_.width();
    int _h = size_.height();

    if (!framebuffer_ || !shader_) {
      _context->makeCurrent(QOpenGLContext::currentContext()->surface());
      initializeOpenGLFunctions();
      QOpenGLFramebufferObjectFormat _format;
      _format.setMipmap(false);
      _format.setSamples(0);
      _format.setTextureTarget(GL_TEXTURE_RECTANGLE);
      _format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
      framebuffer_.reset(new QOpenGLFramebufferObject(
            _w,_h,_format));
      glBindTexture(GL_TEXTURE_RECTANGLE, framebuffer_->texture());
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_RECTANGLE, 0);
      
      shader_.reset(new QOpenGLShaderProgram);
      shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
          fileToStr(":/shader.vert"));
      shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
          fileToStr(":/shader.frag"));
      shader_->link();

      output_.setTexture(framebuffer_->texture(),_w,_h);
    }
 
    double _time = now() - startTime_;

    framebuffer_->bind();
    {
      glViewport(0, 0, framebuffer_->width(), framebuffer_->height());

      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
    
          
      glMatrixMode(GL_PROJECTION);
      {
        glLoadIdentity();
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      }

      glMatrixMode(GL_MODELVIEW);
      {
        glLoadIdentity();
        shader_->bind();
        {
          shader_->setUniformValue("iGlobalTime", GLfloat(_time));
          shader_->setUniformValue("iResolution", QVector2D(_w,_h));
          glBegin(GL_QUADS);
          {
            glTexCoord2f(0.0f, _h);
            glVertex2f(-0.5, -0.5);
            glTexCoord2f(_w, _h);
            glVertex2f(0.5, -0.5);
            glTexCoord2f(_w, 0.0f);
            glVertex2f(0.5, 0.5);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(-0.5, 0.5);
          }
          glEnd();
        }
        shader_->release();
      }
    }
    framebuffer_->release(); 
    glFlush();

    output_.update();
  }
}
