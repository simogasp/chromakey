#include "chromakey.hpp"
#include <opencv2/imgproc.hpp>

using Pixel = uchar;
using Pixel3 = cv::Point3_<uint8_t>;

struct GreenFilter
{
    struct Parameters
    {
    public:
        Parameters() = default;
        Parameters(Pixel hMax, Pixel hMin, float sThreshold, float vThreshold)
                : _hMax(hMax),
                  _hMin(hMin),
                  _sThreshold(sThreshold),
                  _vThreshold(vThreshold) {}
        /**
         * The max angle in deg for the Hue value
         */
        Pixel _hMax{130};
        /**
         * The min angle in deg for the Hue value
         */
        Pixel _hMin{60};
        /**
         * The threshold (min value in [0, 1]) for the saturation
         */
        float _sThreshold{0.4f};
        /**
         * The threshold (min value in [0, 1]) for the value
         */
        float _vThreshold{0.3f};
    };

    explicit GreenFilter(const cv::Mat &hsv)
            : _hsv(hsv) { }

    GreenFilter(const cv::Mat &hsv, const Parameters& param)
            : _hsv(hsv),
              _param(param) { }

    void operator ()(Pixel &pixel, const int * position) const
    {
        const Pixel3& pxHSV = *_hsv.ptr<Pixel3>(position[0], position[1]);
        // divide Hue by 2 because opencv stores halved values
        if (pxHSV.x >= _param._hMin/2 && pxHSV.x <= _param._hMax/2 &&
            pxHSV.y >= _param._vThreshold*255 &&
            pxHSV.z >= _param._sThreshold*255)
        {
            pixel = 0;
        }
    }

private:
    const cv::Mat& _hsv;
    const Parameters _param{ };
};

struct ReduceGreenSpill
{
    struct Parameters
    {
    public:
        Parameters() = default;

        Parameters(Pixel hMax,
                   Pixel hMin,
                   float sThreshold,
                   float vThreshold,
                   float spillThreshold,
                   float coeff1,
                   float coeff2)
                : _hMax(hMax),
                  _hMin(hMin),
                  _sThreshold(sThreshold),
                  _vThreshold(vThreshold),
                  _spillThreshold(spillThreshold),
                  _multCoeff1(coeff1),
                  _multCoeff2(coeff2) {}
        /**
         * The max angle in deg for the Hue value
         */
        Pixel _hMax{130};
        /**
         * The min angle in deg for the Hue value
         */
        Pixel _hMin{60};
        /**
         * The threshold (min value in [0, 1]) for the saturation
         */
        float _sThreshold{0.15f};
        /**
         * The threshold (min value in [0, 1]) for the value
         */
        float _vThreshold{0.15f};

        float _spillThreshold{1.5f};

        float _multCoeff1{1.4f};

        float _multCoeff2{1.2f};
    };

    ReduceGreenSpill(const cv::Mat &hsv, cv::Mat &mask) : _hsv(hsv), _mask(mask) { }

    ReduceGreenSpill(const cv::Mat &hsv, cv::Mat &mask, const Parameters& param)
            : _hsv(hsv),
              _mask(mask),
              _param(param) { }

    void operator ()(Pixel3 &pixel, const int * position) const
    {
        const Pixel3& pxHSV = *_hsv.ptr<Pixel3>(position[0], position[1]);

        // divide Hue by 2 because opencv stores halved values
        if (pxHSV.x >= _param._hMin/2 && pxHSV.x <= _param._hMax/2 &&
            pxHSV.y >= 0.15*255 &&
            pxHSV.z >= 0.15*255)
        {
            // BGR
            const auto r = pixel.z;
            const auto g = pixel.y;
            const auto b = pixel.x;
            Pixel& pxMask = *_mask.ptr<Pixel>(position[0], position[1]);
            pxMask = 0;

            if (((r * b) != 0 )&& (((float)g * g) / (r * b) > _param._spillThreshold))
            {
                pixel.z = static_cast<uint8_t>(r * _param._multCoeff1);
//                pixel.y = static_cast<uint8_t>(r * _param._multCoeff1);
                pixel.x = static_cast<uint8_t>(b * _param._multCoeff1);
            }
            else
            {
                pixel.z = static_cast<uint8_t>(r * _param._multCoeff2);
//                pixel.y = static_cast<uint8_t>(r * _param._multCoeff2);
                pixel.x = static_cast<uint8_t>(b * _param._multCoeff2);
            }
        }
    }

private:
    const cv::Mat& _hsv;
    cv::Mat& _mask;
    Parameters _param{ };
};


struct AlphaBlending
{
    AlphaBlending(const cv::Mat &foreground, const cv::Mat &background, const cv::Mat &alpha)
            : _foreground(foreground),
              _background(background),
              _alpha(alpha) { }

    void operator ()(Pixel3 &pixel, const int * position) const
    {
        const Pixel3 &pxFore = *_foreground.ptr<Pixel3>(position[0], position[1]);
        const Pixel3 &pxBack = *_background.ptr<Pixel3>(position[0], position[1]);
        const Pixel &pxAlpha = *_alpha.ptr<Pixel>(position[0], position[1]);

        pixel.x = static_cast<uint8_t>(pxFore.x * pxAlpha / 255.f + (1.f - pxAlpha / 255.f) * pxBack.x);
        pixel.y = static_cast<uint8_t>(pxFore.y * pxAlpha / 255.f + (1.f - pxAlpha / 255.f) * pxBack.y);
        pixel.z = static_cast<uint8_t>(pxFore.z * pxAlpha / 255.f + (1.f - pxAlpha / 255.f) * pxBack.z);
    }

private:
    const cv::Mat& _foreground;
    const cv::Mat& _background;
    const cv::Mat& _alpha;
};

void greenToTransparency(const cv::Mat& imageIn, const cv::Mat& hsv, cv::Mat& imageMask)
{
    imageMask = cv::Mat(imageIn.rows, imageIn.cols, CV_8UC1, cv::Scalar(255));
    imageMask.forEach<Pixel>(GreenFilter(hsv));
}

void reduceGreenSpill(const cv::Mat& imageIn, const cv::Mat& hsv, cv::Mat& imageOut, cv::Mat& imageMask)
{
    imageIn.copyTo(imageOut);
    imageMask = cv::Mat(imageIn.rows, imageIn.cols, CV_8UC1, cv::Scalar(255));
    imageOut.forEach<Pixel3>(ReduceGreenSpill(hsv, imageMask));
}

cv::Mat applyGaussianFilter(const cv::Mat& imageIn, double sigma)
{
    cv::Mat output;
    auto size = static_cast<int>(2 * std::ceil(2 * sigma) + 1);
    GaussianBlur(imageIn, output, cv::Size(size,size), sigma, 0, cv::BORDER_ISOLATED);
    return output;
}

cv::Mat applyTransparencyMask(const cv::Mat& imageIn, const cv::Mat& imageMask)
{
    cv::Mat output(imageIn.rows, imageIn.cols, CV_8UC4);
    std::vector<cv::Mat> matChannels;
    cv::split(imageIn, matChannels);

    // create alpha channel
    const auto channels = matChannels.size();
    if(channels == 3)
    {
        matChannels.push_back(imageMask);
    }
    else if(channels == 4)
    {
        imageMask.copyTo(matChannels[3]);
    }
    else
    {
        throw std::invalid_argument("The input image must be a color image!");
    }

    cv::merge(matChannels, output);
    return output;
}


void computeChromaMask(const cv::Mat &bgrImg, cv::Mat& outImg, cv::Mat &outMask)
{
    // create the hsv
    cv::Mat hsv;
    cv::cvtColor(bgrImg, hsv, CV_BGR2HSV);

    // determine the transparency mask
    greenToTransparency(bgrImg, hsv, outMask);

    // reduce remaining green pixels
    cv::Mat tmp;
    reduceGreenSpill(bgrImg, hsv, outImg, tmp);

    // add blur
    outMask = applyGaussianFilter(outMask, 1);

}


cv::Mat alphaBlending(const cv::Mat& foreground, const cv::Mat& background, const cv::Mat& alpha)
{
    cv::Mat blended = foreground.clone();
    blended.forEach<Pixel3>(AlphaBlending(foreground, background, alpha));
    return blended;
}