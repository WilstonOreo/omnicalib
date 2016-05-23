/********************************************************************************
 * This file is part of cm8kr.
 *
 * Copyright (c) 2016, CR8TR GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ********************************************************************************/

#ifndef OMNIC_VERSION_H_
#define OMNIC_VERSION_H_

namespace omnic {
  /// Simple struct for holding version numbers
  struct Version {
    /// Construct with latest version by default
    Version(
        uint32_t _major = OMNICALIBRATION_MAJOR_VERSION,
        uint32_t _minor = OMNICALIBRATION_MINOR_VERSION,
        uint32_t _patch = OMNICALIBRATION_PATCH_VERSION) : 
      major_(_major),
      minor_(_minor),
      patch_(_patch) {}

    inline static Version latest() {
      return Version();
    }
    
    inline uint32_t id() const {
      return major_ * 1000000 + minor_ * 1000 + patch_;
    }

    friend bool operator <(const Version& lhs, const Version& rhs) //friend claim has to be here
    {
      return lhs.id() < rhs.id();
    }

    inline uint32_t major() const {
      return major_;
    }
    
    inline uint32_t minor() const {
      return minor_;
    }
    
    inline uint32_t patch() const {
      return patch_;
    }

  private:
    uint32_t major_, minor_, patch_;
  };
}

#endif /* OMNIC_VERSION_H_ */
