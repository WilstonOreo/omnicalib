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
#ifndef OMNIC_GL_RENDERER_H_
#define OMNIC_GL_RENDERER_H_

#include <omnic/gl/Texture.h>
#include <omnic/gl/TextureRef.h>
#include <omnic/gl/shader.h>
#include <omnic/CalibratedProjector.h>

namespace omnic
{
  namespace gl
  {
    class Renderer OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS
    {
    public:
      Renderer() {}
      Renderer(CalibratedProjector const& _proj)
      {
        initialize(_proj);
      }

      inline void initialize(CalibratedProjector const& _proj)
      {
#if OMNIC_USE_QT_GL
        initializeOpenGLFunctions();
#endif
        destroy();

        if (programId_ == 0)
        {
          programId_ = glCreateProgram();
          GLint _vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
          GLint _fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

          const char* _vertexSrc = std::string(CalibrationFragmentShader_120).c_str();
          const char* _fragmentSrc = std::string(CalibrationFragmentShader_120).c_str();

          glShaderSource(_vertexShaderId,1,&_vertexSrc,NULL);
          glShaderSource(_fragmentShaderId,1,&_fragmentSrc,NULL);

          glCompileShader(_vertexShaderId);
          glCompileShader(_fragmentShaderId);

          glAttachShader(programId_, _vertexShaderId);
          glAttachShader(programId_, _fragmentShaderId);
          
          glLinkProgram(programId_);

          glDeleteShader(_vertexShaderId);
          glDeleteShader(_fragmentShaderId);

          // Get uniforms
          colorCorrectionTexLoc_ = glGetUniformLocation(programId_,"colorcorrection");

          pixeldataTexLoc_ = glGetUniformLocation(programId_,"pixeldata");
          pixeldataTexWidthLoc_ = glGetUniformLocation(programId_,"pixeldata_width");
          pixeldataTexHeightLoc_ = glGetUniformLocation(programId_,"pixeldata_height");

          inputTex2DLoc_ = glGetUniformLocation(programId_,"input_2d");
          inputUseRectLoc_ = glGetUniformLocation(programId_,"input_use_rect");
          inputTexRectLoc_ = glGetUniformLocation(programId_,"input_rect");
          inputWidthLoc_ = glGetUniformLocation(programId_,"input_width");
          inputHeightLoc_ = glGetUniformLocation(programId_,"input_height");
        }

        auto& _colorCorrectionLookUp = _proj.colorCorrection();
        colorCorrectionTex_.initialize(_colorCorrectionLookUp.data(),GL_TEXTURE_1D);

        pixeldataTex_.initialize(_proj.pixelData(),GL_TEXTURE_RECTANGLE);
      }

      inline bool isInitialized() const
      {
        return programId_ != 0;
      }

      inline void bindCalibration(
        GLuint _inputTexId,
        GLuint _inputWidth,
        GLuint _inputHeight,
        GLuint _target = GL_TEXTURE_RECTANGLE) {
        bindCalibration(gl::TextureRef(_inputTexId,_inputWidth,_inputHeight,_target));
      }

      inline void bindCalibration(gl::TextureRef const& _input)
      {
        glUseProgram(programId_);

        /// Different uniform location for different texture target
        switch(_input.target())
        {
        case GL_TEXTURE_RECTANGLE:
          glUniform1i(inputUseRectLoc_,GLint(true));
          glUniform1i(inputTex2DLoc_,0);
          break;
        case GL_TEXTURE_2D:
          glUniform1i(inputUseRectLoc_,GLint(false));
          glUniform1i(inputTexRectLoc_,0);
          break;
        }

        /// Bind input texture
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(_input.target(), _input.id());
        glUniform1i(inputWidthLoc_,_input.width());
        glUniform1i(inputHeightLoc_,_input.height());

        /// Bind pixeldata texture
        glUniform1i(pixeldataTexLoc_,1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_RECTANGLE, pixeldataTex_.textureId());

        /// Bind colorcorrection lookup texture;
        glUniform1i(colorCorrectionTexLoc_,2);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_1D, colorCorrectionTex_.textureId());
      }

      inline void releaseCalibration() 
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D,0);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindTexture(GL_TEXTURE_RECTANGLE,0);
        glUseProgram(0);
      }

      inline void render(gl::TextureRef const& _tex) {
        bindCalibration(_tex);

        auto& _r = contentRect_;

        glBegin(GL_TRIANGLE_STRIP);
        glVertex2i(_r.offsetX()             ,_r.offsetY());
        glVertex2i(_r.offsetX() + _r.width(),_r.offsetY());
        glVertex2i(_r.offsetX()             ,_r.offsetY() + _r.height());
        glVertex2i(_r.offsetX() + _r.width(),_r.offsetY() + _r.height());
        glEnd();

        releaseCalibration();
      }

      inline void render(
        GLuint _inputTexId,
        GLuint _inputWidth,
        GLuint _inputHeight,
        GLuint _target = GL_TEXTURE_RECTANGLE) 
      {
        render(gl::TextureRef(_inputTexId,_inputWidth,_inputHeight,_target));
      }

      inline void destroy() {
        colorCorrectionTex_.destroy();
        pixeldataTex_.destroy();
      }

    private:
      Rect contentRect_;

      TextureRGBA32F colorCorrectionTex_;

      GLint colorCorrectionTexLoc_;

      TextureRGBA16 pixeldataTex_;
      GLint pixeldataTexLoc_;
      GLint pixeldataTexWidthLoc_;
      GLint pixeldataTexHeightLoc_;

      GLint inputTex2DLoc_;
      GLint inputTexRectLoc_;
      GLint inputUseRectLoc_;
      GLint inputWidthLoc_;
      GLint inputHeightLoc_;

      GLint programId_ = 0;
    };
  }
}

#endif /* OMNIC_GL_RENDERER_H_ */
