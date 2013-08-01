/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2008-2012, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_PHOTO_HPP__
#define __OPENCV_PHOTO_HPP__

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

/*! \namespace cv
 Namespace where all the C++ OpenCV functionality resides
 */
namespace cv
{

//! the inpainting algorithm
enum
{
    INPAINT_NS    = 0, // Navier-Stokes algorithm
    INPAINT_TELEA = 1 // A. Telea algorithm
};

CV_EXPORTS_W bool initModule_photo();

//! restores the damaged image areas using one of the available intpainting algorithms
CV_EXPORTS_W void inpaint( InputArray src, InputArray inpaintMask,
                           OutputArray dst, double inpaintRadius, int flags );


CV_EXPORTS_W void fastNlMeansDenoising( InputArray src, OutputArray dst, float h = 3,
                                        int templateWindowSize = 7, int searchWindowSize = 21);

CV_EXPORTS_W void fastNlMeansDenoisingColored( InputArray src, OutputArray dst,
                                               float h = 3, float hColor = 3,
                                               int templateWindowSize = 7, int searchWindowSize = 21);

CV_EXPORTS_W void fastNlMeansDenoisingMulti( InputArrayOfArrays srcImgs, OutputArray dst,
                                             int imgToDenoiseIndex, int temporalWindowSize,
                                             float h = 3, int templateWindowSize = 7, int searchWindowSize = 21);

CV_EXPORTS_W void fastNlMeansDenoisingColoredMulti( InputArrayOfArrays srcImgs, OutputArray dst,
                                                    int imgToDenoiseIndex, int temporalWindowSize,
                                                    float h = 3, float hColor = 3,
                                                    int templateWindowSize = 7, int searchWindowSize = 21);

class CV_EXPORTS_W Tonemap : public Algorithm
{
public:
    CV_WRAP virtual void process(InputArray src, OutputArray dst) = 0;

	CV_WRAP virtual float getGamma() const = 0;
	CV_WRAP virtual void setGamma(float gamma) = 0;
};

class CV_EXPORTS_W TonemapLinear : public Tonemap
{
};

CV_EXPORTS_W Ptr<TonemapLinear> createTonemapLinear(float gamma = 1.0f);

// "Adaptive Logarithmic Mapping For Displaying HighContrast Scenes", Drago et al., 2003

class CV_EXPORTS_W TonemapDrago : public Tonemap
{
public:
	CV_WRAP virtual float getBias() const = 0;
	CV_WRAP virtual void setBias(float bias) = 0;
};

CV_EXPORTS_W Ptr<TonemapDrago> createTonemapDrago(float gamma = 1.0f, float bias = 0.85f);

// "Fast Bilateral Filtering for the Display of High-Dynamic-Range Images", Durand, Dorsey, 2002

class CV_EXPORTS_W TonemapDurand : public Tonemap
{
public:
	CV_WRAP virtual float getContrast() const = 0;
	CV_WRAP virtual void setContrast(float contrast) = 0;

	CV_WRAP virtual float getSigmaSpace() const = 0;
	CV_WRAP virtual void setSigmaSpace(float sigma_space) = 0;

	CV_WRAP virtual float getSigmaColor() const = 0;
	CV_WRAP virtual void setSigmaColor(float sigma_color) = 0;
};

CV_EXPORTS_W Ptr<TonemapDurand> 
createTonemapDurand(float gamma = 1.0f, float contrast = 4.0f, float sigma_space = 2.0f, float sigma_color = 2.0f);

// "Dynamic Range Reduction Inspired by Photoreceptor Physiology", Reinhard, Devlin, 2005

class CV_EXPORTS_W TonemapReinhardDevlin : public Tonemap
{
public:
	CV_WRAP virtual float getIntensity() const = 0;
	CV_WRAP virtual void setIntensity(float intensity) = 0;

	CV_WRAP virtual float getLightAdaptation() const = 0;
	CV_WRAP virtual void setLightAdaptation(float light_adapt) = 0;

	CV_WRAP virtual float getColorAdaptation() const = 0;
	CV_WRAP virtual void setColorAdaptation(float color_adapt) = 0;
};

CV_EXPORTS_W Ptr<TonemapReinhardDevlin> 
createTonemapReinhardDevlin(float gamma = 1.0f, float intensity = 0.0f, float light_adapt = 1.0f, float color_adapt = 0.0f);

class CV_EXPORTS_W ExposureAlign : public Algorithm
{
public:
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArrayOfArrays dst,
								 const std::vector<float>& times, InputArray response) = 0;
};

class CV_EXPORTS_W AlignMTB : public ExposureAlign
{
public:
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArrayOfArrays dst,
								 const std::vector<float>& times, InputArray response) = 0;

	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArrayOfArrays dst) = 0;

	CV_WRAP virtual void calculateShift(InputArray img0, InputArray img1, Point& shift) = 0;
	CV_WRAP virtual void shiftMat(InputArray src, OutputArray dst, const Point shift) = 0;

	CV_WRAP virtual int getMaxBits() const = 0;
	CV_WRAP virtual void setMaxBits(int max_bits) = 0;

	CV_WRAP virtual int getExcludeRange() const = 0;
	CV_WRAP virtual void setExcludeRange(int exclude_range) = 0;
};

// "Fast, Robust Image Registration for Compositing High Dynamic Range Photographs from Handheld Exposures", Ward, 2003

CV_EXPORTS_W Ptr<AlignMTB> createAlignMTB(int max_bits = 6, int exclude_range = 4);

class CV_EXPORTS_W ExposureCalibrate : public Algorithm
{
public:
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArray dst, std::vector<float>& times) = 0;
};

// "Recovering High Dynamic Range Radiance Maps from Photographs", Debevec, Malik, 1997

class CV_EXPORTS_W CalibrateDebevec : public ExposureCalibrate
{
public:
	CV_WRAP virtual float getLambda() const = 0;
    CV_WRAP virtual void setLambda(float lambda) = 0;
	
	CV_WRAP virtual int getSamples() const = 0;
	CV_WRAP virtual void setSamples(int samples) = 0;
};

CV_EXPORTS_W Ptr<CalibrateDebevec> createCalibrateDebevec(int samples = 50, float lambda = 10.0f);

class CV_EXPORTS_W ExposureMerge : public Algorithm
{
public:
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArray dst,
								 const std::vector<float>& times, InputArray response) = 0;
};

// "Recovering High Dynamic Range Radiance Maps from Photographs", Debevec, Malik, 1997

class CV_EXPORTS_W MergeDebevec : public ExposureMerge
{
public:
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArray dst,
								 const std::vector<float>& times, InputArray response) = 0;
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArray dst, const std::vector<float>& times) = 0;
};

CV_EXPORTS_W Ptr<MergeDebevec> createMergeDebevec();

// "Exposure Fusion", Mertens et al., 2007

class CV_EXPORTS_W MergeMertens : public ExposureMerge
{
public:
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArray dst,
								 const std::vector<float>& times, InputArray response) = 0;
	CV_WRAP virtual void process(InputArrayOfArrays src, OutputArray dst) = 0;

	CV_WRAP virtual float getContrastWeight() const = 0;
	CV_WRAP virtual void setContrastWeight(float contrast_weiht) = 0;

	CV_WRAP virtual float getSaturationWeight() const = 0;
	CV_WRAP virtual void setSaturationWeight(float saturation_weight) = 0;

	CV_WRAP virtual float getExposureWeight() const = 0;
	CV_WRAP virtual void setExposureWeight(float exposure_weight) = 0;
};

CV_EXPORTS_W Ptr<MergeMertens> 
createMergeMertens(float contrast_weight = 1.0f, float saturation_weight = 1.0f, float exposure_weight = 0.0f);

} // cv

#endif
