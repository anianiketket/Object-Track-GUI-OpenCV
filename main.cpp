#include "opencv2/opencv.hpp"
#include <opencv2/core/ocl.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    //Initialise webcam
    VideoCapture cap(0);

    namedWindow("GUI",1);
    //Check if webcam is ready
    if(!cap.isOpened())
    {
        return -1;
    }

    //Infinite loop of program
    while(1)
    {
        Mat frame,binary;
        cap >> frame;
        threshold(frame,binary,100,255,THRESH_BINARY);
        imshow("GUI",binary);
        if(waitKey(30)=='q')
            break;
    }
    return 0;
}




