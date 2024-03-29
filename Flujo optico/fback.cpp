#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;


static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,double, const Scalar& color){
  for(int y = 0; y < cflowmap.rows; y += step)
    for(int x = 0; x < cflowmap.cols; x += step){
	const Point2f& fxy = flow.at<Point2f>(y, x);
	//line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),color);
	//line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y+15)),color);
	//line(cflowmap, Point(cvRound(x+fxy.x+15), cvRound(y+fxy.y)), Point(x,y),color);


	line(cflowmap, Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), Point(x+step,y),color);
	line(cflowmap, Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), Point(x,y+step),color);
    
        
      }
}

int main(int, char**)
{
    VideoCapture cap(0);
    //help();
    if( !cap.isOpened() )
        return -1;

    Mat flow, cflow, frame;
    Mat gray, prevgray, uflow;
    namedWindow("flow", 1);

    for(;;)
      {
        cap >> frame;
        flip(frame,frame, 1);
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( !prevgray.empty() )
	  {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 5, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            imshow("flow", cflow);
	  }
        if(waitKey(10)>=0)
	  break;
        std::swap(prevgray, gray);
      }
    return 0;
}
