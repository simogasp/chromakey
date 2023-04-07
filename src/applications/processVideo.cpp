#include "../chromakey/chromakey.hpp"

#include <iostream>

#include <opencv2/videoio.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"

/**
 * @brief Check if a string is a positive integer
 * @param[in] s the input string
 * @return true if the string is a positive integer, false otherwise
 */
bool isPositiveInteger(const std::string& s)
{
    return !s.empty() &&
            std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void usage(const char* argv)
{
    std::cout << "Usage:\n " << argv << " <videofile or cameraIndex> [<background>] " << "\n";
}

int main(int argc, char* argv[] )
{

    if(argc != 2 && argc != 3)
    {
      usage(argv[0]);
      return EXIT_FAILURE;
    }

    const auto inputFeed = std::string(argv[1]);
    const bool isVideo = isPositiveInteger(inputFeed);

    const std::string backgroundFile = (argc == 3) ? std::string(argv[2]) : "";


    cv::VideoCapture cap;

    if(isVideo)
    {
        // open the camera at given index
        const int deviceNumber = std::stoi(inputFeed);
        std::cout << "Opening camera device " << deviceNumber << std::endl;
        cap.open(deviceNumber);
    }
    else
    {
        // open the video file
        std::cout << "Opening video file " << inputFeed << std::endl;
        cap.open(inputFeed);
    }


    if(!cap.isOpened())
    {
        // check if we succeeded
        std::cerr << "unable to open the camera!" << std::endl;
        return EXIT_FAILURE;
    }

    const bool withBackground = (argc == 3);

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
            std::cout << "reading background file "<< backgroundFile << std::endl;
            background = cv::imread(backgroundFile, cv::IMREAD_COLOR);

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