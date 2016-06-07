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

#ifndef OMNIC_SAMPLE_H_
#define OMNIC_SAMPLE_H_

#include <omnic/qt/Output.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>

class QTimer;

namespace omnic {
  class Sample : public QObject, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    Sample(QString const& _filename);
    ~Sample();

  public slots:
    void render();

  private:
    /// Load calibration from file
    void loadCalibration(QString const&  _filename);

    static double now();
    static QString fileToStr(QString const& _filename);

    double startTime_;
    QSize size_;
    std::unique_ptr<QOpenGLFramebufferObject> framebuffer_;
    qt::QUniquePtr<QOpenGLShaderProgram> shader_;
    qt::QUniquePtr<QTimer> timer_;
    qt::Output output_;
  };
}

#endif /* OMNIC_SAMPLE_H_ */
