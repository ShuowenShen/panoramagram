#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;


/** Global variables */
String face_cascade_name = "/Users/sysc1234/macports/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
Mat para = imread("/Users/sysc1234/Desktop/008.jpg",CV_LOAD_IMAGE_COLOR);
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
string window_name2 = "Para";
int previous_facesmax_x_save, previous_facesmax_x;
int n = 1;
int a;
RNG rng(12345);

/** @function main */
int main( int argc, const char** argv )
{
    CvCapture* capture;
    Mat frame;
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    
    //-- 2. Read the video stream
    capture = cvCaptureFromCAM( -1 );
    
    if( capture )
    {
        while( true )
        {
            frame = cvQueryFrame( capture );
            
            //-- 3. Apply the classifier to the frame
            if( !frame.empty())
            {
                std::vector<Rect> faces;
                Mat frame_gray;
                Rect facesmax(0,0,0,0);
                cvtColor( frame, frame_gray, CV_BGR2GRAY );
                equalizeHist( frame_gray, frame_gray );
                
                //-- Detect faces
                face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
                for( size_t i = 0; i < faces.size(); i++ )
                {
                    if(faces[i].width* faces[i].height > facesmax.width * facesmax.height)
                        facesmax = faces[i];
                    //   Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                    //   ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
                }
                for(;n>=0;n--)
                {
                    previous_facesmax_x_save = facesmax.x;
                }
                if(n == 0)
                {
                    previous_facesmax_x = previous_facesmax_x_save;
                    previous_facesmax_x_save = facesmax.x;
                }
                Point center( facesmax.x + facesmax.width*0.5, facesmax.y + facesmax.height*0.5 );
                ellipse( frame, center, Size( facesmax.width*0.5, facesmax.height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 4, 8, 0 );
                //imshow( window_name, frame );
                if(para.rows < 0 || para.cols < 0)
                    return -1;
                
                if(previous_facesmax_x - facesmax.x > 10)
                {
                  a = (previous_facesmax_x + facesmax.width*0.5)/frame.cols * para.cols;
                }
                else
                {
                  a = (facesmax.x + facesmax.width*0.5)/frame.cols * para.cols;
                }
               // cout<<facesmax.x+facesmax.width*0.5<<"   "<<frame.cols<< "  "<< a <<"   "<<para.cols <<" "<<para.rows<<"  "<<facesmax.width<<endl;
                int red = para.at<Vec3b>(10,15).val[2];
                cout << red <<endl;
                if(a > abs(para.cols - para.rows))
                {
                    a = abs(para.cols - para.rows);
                }
                else if(a < min(para.cols,para.rows))
                {
                    a = 0;
                }
                if(facesmax.width > 150)
                {
                Mat result = para(Range(0, para.rows), Range(a, a+para.rows));
                namedWindow("Paranoma",WINDOW_AUTOSIZE);
                imshow("Paranoma", result);
                }
              
                //-- Show what you got
                imshow("video",frame);
              
            }
            else
            { printf(" --(!) No captured frame -- Break!"); break; }
            
            int c = waitKey(1);
            if( (char)c == 'c' ) { break; }
        }
    }

    
    return 0;
}
