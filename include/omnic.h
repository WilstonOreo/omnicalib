/* Copyright (c) 2014-2016 "OmniCalibration" by cr8tr
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 * 
 * The MIT License (MIT)
 * Copyright (c) 2016 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef OMNIC_H_
#define OMNIC_H_

/// Error codes
enum omnic_error_code {
    OMNIC_OK = 0,
    OMNIC_ERROR_READ = 1,
    OMNIC_ERROR_WRITE = 2
};

/// Calibrated pixel as UVW texture coordinates
typedef struct omnic_uvw_pixel {
    float u,v,w; // Texture coordinates
    float b; // Blend mask value
};

/// Correction for a single color channel
typedef struct omnic_channelcorrection {
    /// Gamma correction from -1.0 to 1.0
    float gamma;

    /// Brightness correction from -1.0 to 1.0
    float brightness;

    /// Contrast correction from -1.0 to 1.0
    float contrast;

    /// Correction multiplier from 0.0 to 1.0
    float multiplier;
};

/// Color correction for red, green and blue channel
typedef struct omnic_colorcorrection {
    /// Color correction for red channel
    omnic_colorcorrection red;

    /// Color correction for green channel
    omnic_colorcorrection green;

    /// Color correction for blue channel
    omnic_colorcorrection blue;

    /// Overall color correction for all three channels
    omnic_colorcorrection all;
};

/// Rectangle struct
typedef struct omnic_rect {
    /// Offset in X direction
    int32_t offset_x;

    /// Offset in Y direction
    int32_t offset_y;

    /// Width of rectangle
    uint32_t width;

    /// Height of rectangle
    uint32_t height;
};


/**@brief A calibration for a single projector
 **/
typedef struct omnic_calibrated_projector {
    /// Screen info flags. First bit: virtual screen
    uint32_t screen_info;

    /// Rectangle representing the screen geometry
    omnic_rect screen_geometry;

    /// Rectangle representing the content placement inside the screen
    omnic_rect content_geometry;

    /// Three channel color correction
    omnic_colorcorrection colorcorrection;

    /// Buffer width
    uint32_t buffer_width;

    /// Buffer Height
    uint32_t buffer_height;

    /// Buffer with calibrated pixels
    omni_uvw_pixel* buffer;

    /// Brightness correction buffer width
    uint32_t brightness_correction_width;

    /// Brightness correction buffer height
    uint32_t brightness_correction_height;

    /// Buffer with brightness corrected pixels
    omnic_channelcorrection* brightness_correction;
};

/// Calibration for several projectors
typedef struct omnic_calibration {
    /// Header string
    char header[80];

    /**@brief Calibration content type
     **@detail 0 = UV coords, 1 = UVW coords
     **/
    uint32_t content_type;

    /// Number of projectors
    uint32_t num_projectors;

    /// List of calibrated projectors
    omnic_calibrated_projector* projectors;
};

/// Read calibration from file
omnic_error_code omnic_read(FILE*,omni_calibration*);

/// Read calibration from file with name
omnic_error_code omnic_read(const char*,omni_calibration*);

/// Write calibration to file
omnic_error_code omnic_write(FILE*,omni_calibration*);

/// Write calibration to file with name
omnic_error_code omnic_write(const char*,omni_calibration*);

/// Free memory of calibration
void omnic_free(omni_calibration*);

#endif /* OMNIC_CALIBRATION_H_ */
