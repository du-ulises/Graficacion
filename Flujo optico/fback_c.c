#undef _GLIBCXX_DEBUG

#include "cv.h"
#include "highgui.h"

void drawOptFlowMap(const CvMat* flow, CvMat* cflowmap, int step, double scale, CvScalar color)
{
    int x, y;
    for( y = 200; y < 400; y += step)
        for( x = 200; x < 400; x += step)
        {
            CvPoint2D32f fxy = CV_MAT_ELEM(*flow, CvPoint2D32f, y, x);
            cvLine(cflowmap, cvPoint(x,y), cvPoint(cvRound(x+fxy.x), cvRound(y+fxy.y)),color, 1, 8, 0);
            cvCircle(cflowmap, cvPoint(x,y), 2, color, -1, 8, 0);
        }
}

int main(int argc, char** argv)
{
    CvCapture* capture = cvCreateCameraCapture(0);
    CvMat* prevgray = 0, *gray = 0, *flow = 0, *cflow = 0;

    if( !capture )
        return -1;
    
    cvNamedWindow("flow", 1);
    
    for(;;)
    {
        int firstFrame = gray == 0;
        IplImage* frame = cvQueryFrame(capture);
        if(!frame)
            break;
        if(!gray)
        {
            gray = cvCreateMat(frame->height, frame->width, CV_8UC1);
            prevgray = cvCreateMat(gray->rows, gray->cols, gray->type);
            flow = cvCreateMat(gray->rows, gray->cols, CV_32FC2);
            cflow = cvCreateMat(gray->rows, gray->cols, CV_8UC3);
        }
        cvCvtColor(frame, gray, CV_BGR2GRAY);
        
        if( !firstFrame )
        {
           cvCalcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        //     cvCalcOpticalFlowLK(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
           
            cvCvtColor(prevgray, cflow, CV_GRAY2BGR);
            drawOptFlowMap(flow, cflow, 16, 1.5, CV_RGB(0, 255, 0));
            cvShowImage("flow", cflow);
        }
        if(cvWaitKey(30)>=0)
            break;
        {
        CvMat* temp;    
       CV_SWAP(prevgray, gray, temp);
        }
    }
    cvReleaseCapture(&capture);
    return 0;
}
