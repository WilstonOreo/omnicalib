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

namespace omnic
{
  namespace gl
  {
    class Renderer OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS
    {
    public:
      Renderer() {}
      Renderer(CalibratedProjector& _proj)
      {
        init(_proj);
      }

      void initialize(CalibratedProjector const& _proj)
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

          glCompileShader(VertexShaderObject);
          glCompileShader(FragmentShaderObject);

          glAttachShader(programId_, VertexShaderObjecti);
          glAttachShader(programId_, FragmentShaderObject);

          glDeleteShader(_vertexShaderId);
          glDeleteShader(_fragmentShaderId);

          // Get uniforms
           colorCorrectionTexLoc_ = glGetUniformLocation(programId_,"colorcorrection");
           colorCorrectionTexWidthLoc_ = glGetUniformLocation(programId_,"colorcorrection_width");
           colorCorrectionTexHeightLoc_ = glGetUniformLocation(programId_,"colorcorrection_height");

           calibrationTexLoc_ = glGetUniformLocation(programId_,"calibration");
           calibrationTexWidthLoc_ = glGetUniformLocation(programId_,"calibration_width");
           calibrationTexHeightLoc_ = glGetUniformLocation(programId_,"calibration_height");

           inputTex2DLoc_ = glGetUniformLocation(programId_,"input_2d");
           inputTexRectLoc_ = glGetUniformLocation(programId_,"input_rect");
           inputWidthLoc_ = glGetUniformLocation(programId_,"input_width");
           inputHeightLoc_ = glGetUniformLocation(programId_,"input_height");
        }

        auto& _colorCorrectionLookUp = _proj.colorCorrectionLookUp();
        colorCorrectionTex_.initialize(_colorCorrectionLookUp.data(),GL_TEXTURE_1D);
        calibrationTex_.initialize(_proj.pixelData(),GL_TEXTURE_RECT);

      }

      inline bool isInitialized() const
      {
        return programId_ != 0;
      }

      inline void bindCalibration(
        GLuint _inputTexId,
        GLuint _inputWidth,
        GLuint _inputHeight,
        GLuint _target = GL_TEXTURE_RECT) const
      {
        glUseProgram(programId_);

        /// Different uniform location for different texture target
        switch(_target)
        {
        case GL_TEXTURE_RECT:
          glUniform1i(inputTex2DLoc_,0);
          break;
        case GL_TEXTURE_2D:
          glUniform1i(inputTexRectLoc_,0);
          break;
        }

        /// Bind input texture
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(_target, _inputTexId);

        /// Bind calibration texture
        glUniform1i(calibrationTexLoc_,1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_RECT, calibrationTex_.textureId());

        /// Bind colorcorrection lookup texture;
        glUniform1i(colorCorrectionTexLoc_,2);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_1D, colorCorrectionTex_.textureId());

        glUniform1i(inputWidthLoc_,_inputWidth);
        glUniform1i(inputHeightLoc_,_inputHeight);
      }

      inline void releaseCalibration() const
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D,0);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindTexture(GL_TEXTURE_RECT,0);
        glUseProgram(0);
      }

      void render(
        GLuint _inputTexId,
        GLuint _inputWidth,
        GLuint _inputHeight,
        GLuint _target = GL_TEXTURE_RECT) const
      {
        bindCalibration(_inputTexId,_inputWidth,_inputHeight);

        // Render rectangle

        releaseCalibration();
      }

    private:
      Rect viewportRect_;

      TextureRGBA32F colorCorrectionTex_;

      GLuint colorCorrectionTexId_;
      GLint colorCorrectionTexLoc_;
      GLint colorCorrectionTexWidth_;
      GLint colorCorrectionTexWidthLoc_;

      TextureRGBA16 calibrationTex_;
      GLint calibrationTexLoc_;

      GLint inputTex2DLoc_;
      GLint inputTexRectLoc_;
      GLint inputWidthLoc_;
      GLint inputHeightLoc_;

      GLint programId_ = 0;
    };
  }
}

#endif /* OMNIC_GL_RENDERER_H_ */
