#include "../chromakey/chromakey.hpp"
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.hpp>

void usage(const char* argv)
{
    std::cout << "Usage:\n " << argv[0] << " <image> [<background>] " << "\n";
}

int main(int argc, char* argv[] )
{
    if(argc <= 2 || argc > 3)
    {
        usage(argv[0]);
    }

    // load image
    cv::Mat rgbImg = cv::imread(std::string(argv[1]), cv::IMREAD_COLOR);

    cv::Mat outMask;
    cv::Mat outImage;

    computeChromaMask(rgbImg, outImage, outMask);

    cv::Mat transparent = applyTransparencyMask(outImage, outMask);

    const std::string ORIGINAL_IMG = "original image";
    const std::string OUTPUT_IMG = "mask image";
    const std::string FINAL_IMG = "final image";
    const std::string BLENDED_IMG = "blended image";

    cvNamedWindow(ORIGINAL_IMG.c_str());
    cv::imshow(ORIGINAL_IMG, rgbImg);
    cv::waitKey(-1);

    cvNamedWindow(OUTPUT_IMG.c_str());
    cv::imshow(OUTPUT_IMG, outMask);
    cv::waitKey(-1);

    cvNamedWindow(FINAL_IMG.c_str());
    cv::imshow(FINAL_IMG, transparent);
    cv::waitKey(-1);

    cv::imwrite("result.png", transparent);

    if(argc != 3)
    {
        return EXIT_SUCCESS;
    }

    cv::Mat background = cv::imread(std::string(argv[2]), cv::IMREAD_COLOR);

    // resize if the background has not the same size
    if(background.rows != outImage.rows && background.cols != outImage.cols)
    {
        cv::Mat temp(outImage.rows, outImage.cols, background.type());
        cv::resize(background, temp, cv::Size(outImage.cols, outImage.rows));
        background = temp;
    }

    cv::Mat blended = alphaBlending(outImage, background, outMask);

    cvNamedWindow(BLENDED_IMG.c_str());
    cv::imshow(BLENDED_IMG, blended);
    cv::waitKey(-1);

    cv::imwrite("blended.png", blended);

    return EXIT_SUCCESS;
}