#include "chromakey.hpp"

#include <iostream>

#include <opencv/cv.h>
#include <opencv2/videoio.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"

void usage(const char* argv)
{
    std::cout << "Usage:\n " << argv[0] << " <filename> [<background>] " << "\n";
}

int main(int argc, char* argv[] )
{
    // open the default camera
    cv::VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened())
    {
        // check if we succeeded
        std::cerr << "unable to open the camera!" << std::endl;
        return EXIT_FAILURE;
    }

    bool withBackground = false;
    if(argc == 2)
    {
        withBackground = true;
    }

    cv::Mat background;


    const std::string ORIGINAL_IMG = "original image";
    const std::string OUTPUT_IMG = "mask image";
    const std::string FINAL_IMG = "final image";

    int delay = 10;

    cv::namedWindow(FINAL_IMG, cv::WINDOW_NORMAL);
    while(true)
    {
        cv::Mat frame;
        // get a new frame from camera
        cap >> frame;
        if(frame.empty())
            break;

        if(withBackground && background.empty())
        {
            background = cv::imread(std::string(argv[1]), cv::IMREAD_COLOR);

            // resize if the background has not the same size
            if(background.rows != frame.rows && background.cols != frame.cols)
            {
                cv::Mat temp(frame.rows, frame.cols, background.type());
                cv::resize(background, temp, cv::Size(frame.cols, frame.rows));
                background = temp;
            }
        }

        cv::Mat outMask;
        cv::Mat outImage;

        computeChromaMask(frame, outImage, outMask);

//        outImage = applyTransparencyMask(outImage, outMask);

        if(withBackground)
        {
            assert(!background.empty());
            outImage = alphaBlending(outImage, background, outMask);
        }
        cv::imshow(FINAL_IMG, outImage);

        if( cv::waitKey(delay) == 27 )
            break;
        const char key = (char) cv::waitKey(delay);
        // stop capturing by pressing ESC
        if(key == 27)
            break;
        if(key == 'l' || key == 'L')
            delay = 10;
        // delay = 0 will wait for a key to be pressed
        if(key == 'd' || key == 'D')
            delay = 0;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return EXIT_SUCCESS;
}