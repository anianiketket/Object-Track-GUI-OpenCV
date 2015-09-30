#include "opencv2/opencv.hpp"
#include <opencv2/core/ocl.hpp>
#include "set_value.h"

using namespace cv;

int main(int argc, char *argv[])
{
    Mat frame,binary;
    int Rmin,Rmax,Bmin,Bmax,Gmin,Gmax,brightness,gamma,struct_size,vidh,vidw;

    ocl::setUseOpenCL(false);   //Close OCL support

    VideoCapture cap(0);        //Videocapture class initialized

    if(!cap.isOpened())         //Check for successful load of camera
    {
        cout << "Camera could not be loaded.";
        return -1;
    }

    vid_h = cap.get(CAP_PROP_FRAME_HEIGHT); //Video height and video width
    vid_w = cap.get(CAP_PROP_FRAME_WIDTH);

    SetParameter(cap,&Rmin,&Rmax,&Bmin,&Bmax,&Gmin,&Gmax,&brightness,&gamma,&struct_size);   //Function to set parameters for image processing

    namedWindow("Binary",WINDOW_NORMAL);

    while(1)
    {
        cap >> frame;
        frame = EditImage(frame,&brightness,&gamma);        //Adjusts contrast and brightness according to factors provided
        binary = Threshold(frame,&Rmin,&Rmax,&Bmin,&Bmax,&Gmin,&Gmax);  //Thresholds and converts image to binary
        binary = Morph(binary,&struct_size);
        imshow("Binary",binary);
        if(waitKey(1)=='q')     //If q is pressed program is exitted
            break;
    }

}
