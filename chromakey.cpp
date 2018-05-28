#include "chromakey.hpp"
#include <opencv2/imgproc.hpp>

using Pixel = uchar;
using Pixel3 = cv::Point3_<uint8_t>;

struct GreenFilter
{
    explicit GreenFilter(const cv::Mat &hsv) : hsv(hsv)  { }

    void operator ()(Pixel &pixel, const int * position) const
    {
        const Pixel3& pxHSV = *hsv.ptr<Pixel3>(position[0], position[1]);
        if (pxHSV.x >= 60/2 && pxHSV.x <= 130/2 &&
            pxHSV.y >= 0.4*255 &&
            pxHSV.z >= 0.3*255)
        {
            pixel = 0;
        }
    }

private:
    const cv::Mat& hsv;
};

struct ReduceGreenSpill
{
    explicit ReduceGreenSpill(const cv::Mat &hsv, cv::Mat &mask) : hsv(hsv), mask(mask) { }

    void operator ()(Pixel3 &pixel, const int * position) const
    {
        const Pixel3& pxHSV = *hsv.ptr<Pixel3>(position[0], position[1]);
        if (pxHSV.x >= 60/2 && pxHSV.x <= 130/2 &&
            pxHSV.y >= 0.15*255 &&
            pxHSV.z >= 0.15*255)
        {
            // BGR
            const auto r = pixel.z;
            const auto g = pixel.y;
            const auto b = pixel.x;
            Pixel& pxMask = *mask.ptr<Pixel>(position[0], position[1]);
            pxMask = 0;

            if (((r * b) != 0 )&& (((float)g * g) / (r * b) > 1.5))
            {
                pixel.z = static_cast<uint8_t>(r * 1.4f);
//                pixel.y = static_cast<uint8_t>(r * 1.4f);
                pixel.x = static_cast<uint8_t>(b * 1.4f);
            }
            else
            {
                pixel.z = static_cast<uint8_t>(r * 1.2f);
//                pixel.y = static_cast<uint8_t>(r * 1.4f);
                pixel.x = static_cast<uint8_t>(b * 1.2f);
            }
        }
    }

private:
    const cv::Mat& hsv;
    cv::Mat& mask;
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
    reduceGreenSpill(bgrImg, hsv, outImg, outMask);

    // add blur
    outMask = applyGaussianFilter(outMask, 1);

}




