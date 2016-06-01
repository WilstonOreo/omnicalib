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

#ifndef OMNIC_GL_TEXTURE_H_
#define OMNIC_GL_TEXTURE_H_

#include <omnic/gl/functions.h>

namespace omnic {
  namespace gl {
    /**@brief Template class for holding a GL_TEXTURE_2D with arbitrary data type
     * @tparam 
     **/
    template<GLuint FORMAT, typename T> 
    class Texture OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS {
    public:
      /// Our channel type 
      typedef T type;

      /// GLuint of format id
      inline static constexpr GLuint format() { return FORMAT; }

      /// Default constructor 
      Texture() {
      }
      
      /// Construct from buffer type
      template<typename BUF>
      Texture(BUF const& _buf, GLuint _target = GL_TEXTURE_2D) {
        initialize(_buf,_target);
      }

      /// Destruct and destroy texture
      ~Texture() {
        destroy();
      }

      /// Initialize texture data from buffer
      template<typename BUF>
      void initialize(BUF const& _buf, GLuint _target = GL_TEXTURE_2D) {
#if OMNIC_USE_QT_GL 
          initializeOpenGLFunctions();
#endif
        destroy();
        target_ = _target;
        width_ = _buf.width();
        height_ = _buf.height();

        glGenTextures(1, &texId_);
        bind();
        {
          glTexParameteri(target_, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
          glTexParameteri(target_, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
          glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          gl::texImage2D<format(),type>(target_,size_,_buf.ptr());
        }
        release();
      }

      /// Destroy texture and set internal tex ID to 0
      void destroy() {
        if (texId_ != 0) {
          glDeleteTextures(1, &texId_);
          texId_ = 0;
        } 
      }

      /// Bind texture with glBindTexture(target,texId)
      void bind() {
        glBindTexture(target_, texId_);
      }

      /// Release texture with glBindTexture(target,0)
      void release() {
        glBindTexture(target_, 0);
      }

      /// Return GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE
      GLuint target() const {
        return target_;
      }

      /// Return id of this texture
      GLuint textureId() const {
        return texId_;
      }

      /// Return width of texture
      int width() const {
        return size_;
      }
      
      /// Return width of texture
      int height() const {
        return size_;
      }

    private:
      int width_ = 0 ;
      int height_ = 0 ;
      GLuint target_ = GL_TEXTURE_2D;
      GLuint texId_ = 0;
    };

    typedef Texture<GL_RGBA,GLfloat> TextureRGBA32F;
    typedef Texture<GL_RGBA,GLushort> TextureRGBA16;
  }
}

#endif /* OMNIC_GL_TEXTURE_H_ */
