#ifndef set_value
#define set_value

#include "opencv2/opencv.hpp"
#include <opencv2/core/ocl.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int vid_h,vid_w;

vector<Mat> bgr;

Mat EditImage(Mat frame,int* slider_brightness,int* slider_gamma)
{
    frame.convertTo(frame,-1,float(*slider_gamma)/25,*slider_brightness);
    return frame;
}

Mat Threshold(Mat frame,int* slider_Rmin,int* slider_Rmax,int* slider_Bmin,int* slider_Bmax,int* slider_Gmin,int* slider_Gmax)
{
	Mat binary,temp;
	flip(frame,frame,1);
	split(frame,bgr);

	threshold(bgr[0],temp,*slider_Bmin,255,THRESH_BINARY);
	threshold(bgr[0],bgr[0],*slider_Bmax,255,THRESH_BINARY_INV);
	bitwise_and(bgr[0],temp,bgr[0]);

	temp = Mat::zeros(vid_h,vid_w,CV_8U);
	threshold(bgr[1],temp,*slider_Gmin,255,THRESH_BINARY);
	threshold(bgr[1],bgr[1],*slider_Gmax,255,THRESH_BINARY_INV);
	bitwise_and(bgr[1],temp,bgr[1]);

	temp = Mat::zeros(vid_h,vid_w,CV_8U);
	threshold(bgr[2],temp,*slider_Rmin,255,THRESH_BINARY);
	threshold(bgr[2],bgr[2],*slider_Rmax,255,THRESH_BINARY_INV);
	bitwise_and(bgr[2],temp,bgr[2]);

	bitwise_and(bgr[1],bgr[2],bgr[1]);
	bitwise_and(bgr[1],bgr[0],binary);
	return binary;
}

Mat Morph(Mat binary,int* slider_struct_size)
{
    Mat element = getStructuringElement(MORPH_ELLIPSE,Size(2*(*slider_struct_size)+1,2*(*slider_struct_size)+1));
    Mat eroded;
    erode(binary,eroded,element);
    return eroded;
}

void CreateTrackBars(int* slider_Rmin,int* slider_Rmax,int* slider_Bmin,int* slider_Bmax,int* slider_Gmin,int* slider_Gmax,int* slider_brightness,int* slider_gamma,int* slider_struct_size)
{
    createTrackbar("Rmin","GUI",slider_Rmin,255);
    createTrackbar("Rmax","GUI",slider_Rmax,255);
    createTrackbar("Gmin","GUI",slider_Gmin,255);
    createTrackbar("Gmax","GUI",slider_Gmax,255);
    createTrackbar("Bmin","GUI",slider_Bmin,255);
    createTrackbar("Bmax","GUI",slider_Bmax,255);
    createTrackbar("Brightness","GUI",slider_brightness,50);
    createTrackbar("Contrast","GUI",slider_gamma,150);
    createTrackbar("Struct element size","GUI",slider_struct_size,10);
}

void SetParameter(VideoCapture cap,int* Rmin_ref,int* Rmax_ref,int* Bmin_ref,int* Bmax_ref,int* Gmin_ref,int* Gmax_ref,int* brightness_ref,int* gamma_ref,int* struct_size_ref)
{
    Mat binary,frame;
    int slider_Rmin = 0,
        slider_Rmax = 0,
        slider_Gmin = 0,
        slider_Gmax = 0,
        slider_Bmin = 0,
        slider_Bmax = 0,
        slider_brightness = 1,
        slider_gamma = 25,
        slider_struct_size = 0;
    //Create a figure
    namedWindow("GUI",WINDOW_NORMAL);
    namedWindow("Original",WINDOW_NORMAL);

    //Create tarckbars on window
    CreateTrackBars(&slider_Rmin,&slider_Rmax,&slider_Bmin,&slider_Bmax,&slider_Gmin,&slider_Gmax,&slider_brightness,&slider_gamma,&slider_struct_size);

    vid_h = cap.get(CAP_PROP_FRAME_HEIGHT);
    vid_w = cap.get(CAP_PROP_FRAME_WIDTH);

    //Infinite loop of program
    while(1)
    {
        cap >> frame;
        frame = EditImage(frame,&slider_brightness,&slider_gamma);
        binary = Threshold(frame,&slider_Rmin,&slider_Rmax,&slider_Bmin,&slider_Bmax,&slider_Gmin,&slider_Gmax);
        binary = Morph(binary,&slider_struct_size);
        imshow("GUI",binary);
        imshow("Original",frame);
        if(waitKey(30)=='q')
        {
            *Rmin_ref = slider_Rmin;
            *Rmax_ref = slider_Rmax;
            *Bmin_ref = slider_Bmin;
            *Bmax_ref = slider_Bmax;
            *Gmin_ref = slider_Gmin;
            *Gmax_ref = slider_Gmax;
            *brightness_ref = slider_brightness;
            *gamma_ref = slider_gamma;
            *struct_size_ref = slider_struct_size;
            break;
        }
    }
    destroyWindow("GUI");
    destroyWindow("Original");
}

#endif
