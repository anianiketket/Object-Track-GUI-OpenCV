#include <iostream>

#include "opencv2/opencv.hpp"
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;

int slider_Rmin,slider_Rmax,slider_Gmin,slider_Gmax,slider_Bmin,slider_Bmax,slider_brightness = 1,slider_gamma = 25;

int vid_h,vid_w;
Mat frame;
vector<Mat> bgr;

void EditImage(void)
{
    frame.convertTo(frame,-1,float(slider_gamma)/25,slider_brightness);
}

void Threshold(void)
{
    EditImage();
	Mat binary,temp;
	flip(frame,frame,1);
	split(frame,bgr);

	threshold(bgr[0],temp,slider_Bmin,255,THRESH_BINARY);
	threshold(bgr[0],bgr[0],slider_Bmax,255,THRESH_BINARY_INV);
	bitwise_and(bgr[0],temp,bgr[0]);

	temp = Mat::zeros(vid_h,vid_w,CV_8U);
	threshold(bgr[1],temp,slider_Gmin,255,THRESH_BINARY);
	threshold(bgr[1],bgr[1],slider_Gmax,255,THRESH_BINARY_INV);
	bitwise_and(bgr[1],temp,bgr[1]);

	temp = Mat::zeros(vid_h,vid_w,CV_8U);
	threshold(bgr[2],temp,slider_Rmin,255,THRESH_BINARY);
	threshold(bgr[2],bgr[2],slider_Rmax,255,THRESH_BINARY_INV);
	bitwise_and(bgr[2],temp,bgr[2]);

	bitwise_and(bgr[1],bgr[2],bgr[1]);
	bitwise_and(bgr[1],bgr[0],binary);
	imshow("GUI",binary);
}

void CreateTrackBars(void)
{
    createTrackbar("Rmin","GUI",&slider_Rmin,255);
    createTrackbar("Rmax","GUI",&slider_Rmax,255);
    createTrackbar("Gmin","GUI",&slider_Gmin,255);
    createTrackbar("Gmax","GUI",&slider_Gmax,255);
    createTrackbar("Bmin","GUI",&slider_Bmin,255);
    createTrackbar("Bmax","GUI",&slider_Bmax,255);
    createTrackbar("Brightness","GUI",&slider_brightness,50);
    createTrackbar("Contrast","GUI",&slider_gamma,150);
}

int* SetParameter(void)
{
    int return_values[8];
    int *point;
    ocl::setUseOpenCL(false);

    //Initialise webcam
    VideoCapture cap(0);

    //Create a figure
    namedWindow("GUI",WINDOW_NORMAL);

    //Create tarckbars on window
    CreateTrackBars();

    //Check if webcam is ready
    if(!cap.isOpened())
    {
        *point = -1;
        return point;
    }

    vid_h = cap.get(CAP_PROP_FRAME_HEIGHT);
    vid_w = cap.get(CAP_PROP_FRAME_WIDTH);

    //Infinite loop of program
    while(1)
    {
        cap >> frame;
        Threshold();
        if(waitKey(30)=='q')
            break;
    }

    //Close the video capture class
    cap.release();

    //Fill the array with all the parameters
    return_values[0]=slider_Rmin;
    return_values[1]=slider_Rmax;
    return_values[2]=slider_Gmin;
    return_values[3]=slider_Gmax;
    return_values[4]=slider_Bmin;
    return_values[5]=slider_Bmax;
    return_values[6]=slider_brightness;
    return_values[7]=slider_gamma;
    point = &return_values[0];
    return point;
}

int main(int argc, char *argv[])
{
    int *returned_pointer;

    returned_pointer = SetParameter();
    cout << "The first value is" << *returned_pointer << "\n";
}
