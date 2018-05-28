#include "chromakey.hpp"
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.hpp>

void usage(const char* argv)
{
    std::cout << "Usage:\n " << argv[0] << " <filename> " << "\n";
}

int main(int argc, char* argv[] )
{

    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())
    {
        // check if we succeeded
        std::cerr << "unable to open the camera!" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string ORIGINAL_IMG = "original image";
    const std::string OUTPUT_IMG = "mask image";
    const std::string FINAL_IMG = "final image";

    cv::namedWindow(FINAL_IMG,1);
    for(;;)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera

        cv::Mat outMask;
        cv::Mat outImage;

        computeChromaMask(frame, outImage, outMask);

        outImage = applyTransparencyMask(outImage, outMask);

        cv::imshow(FINAL_IMG, outImage);
        if(cv::waitKey(10) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return EXIT_SUCCESS;
}