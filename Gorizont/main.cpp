

#include <QCoreApplication>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <iomanip>
using namespace cv;
using namespace std;
Mat src_gray,edges;
int thresh = 100;
RNG rng(12345);
//Mat src = imread( "D:\\test2.jpg" );
Mat src = imread( "D:\\841203449.jpeg" );
Mat frame, frame2,frame3,frame_HSV, frame_threshold;
//Point Txt_Point;
char Txt_Point[50] = { 0 };

struct point_t
{
    int x_, y_;
};

double distance_(const point_t &point)
{
    return sqrtf(point.x_*point.x_ + point.y_*point.y_);
}
int main(int argc, char *argv[])
{

    // Declare the output variables
     Mat dst, cdst, cdstP,dstBlur;
     // Loads an image
     Mat src = imread( "D:\\test4.jpg", IMREAD_GRAYSCALE );
     Mat src2 = imread( "D:\\test4.jpg");

  //   Mat frameBlur,frame_threshold,frame2;
    medianBlur(src, dstBlur, 9);  //фильтр сглаживания
imshow("medianBlur", dstBlur);
        // Edge detection
     Canny(dstBlur, dst, 130, 200, 3);

     imshow("Canny", dst);
     cvtColor(dst, cdst, COLOR_GRAY2BGR);
     cdstP = cdst.clone();
     // Standard Hough Line Transform
     vector<Vec2f> lines; // will hold the results of the detection
     HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
     // Draw the lines
     for( size_t i = 0; i < lines.size(); i++ )
     {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
     }
     // Probabilistic Line Transform
     vector<Vec4i> linesP; // will hold the results of the detection
     HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
     // Draw the lines
     Point ptBegin,ptEnd;
     for( size_t i = 0; i < linesP.size(); i++ )
     {
     Vec4i l = linesP[i];
     line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
      line( src2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);

     }


     //сортировка по Х
     vector<point_t>  sourceBegin;
    vector<point_t>  sourceEnd;
         int N = 0;
         double x_ = 0, y_ = 0;
         for (int i = 0; i < linesP.size(); ++i)
         {
            Vec4i l = linesP[i];
             sourceBegin.push_back(point_t{ l[0], l[1] });
             sourceEnd.push_back(point_t{ l[2], l[3] });
         }
         sort(sourceBegin.begin(), sourceBegin.end(), [](point_t lhs, point_t rhs) {return distance_(lhs) < distance_(rhs); });
         sort(sourceEnd.begin(), sourceEnd.end(), [](point_t lhs, point_t rhs) {return distance_(lhs) < distance_(rhs); });
         for (point_t & point_ : sourceBegin)
         {
             cout << "point with x =" << point_.x_ << " y = " << point_.y_ << endl;
         }
         for (point_t & point_ : sourceEnd)
         {
             cout << "point with x =" << point_.x_ << " y = " << point_.y_ << endl;
         }


// поиск масимального разрыва между линиями
int temp=0;int ind=0;
         for (int i = 0; i < sourceBegin.size()-1; ++i)
         {
            if (temp<abs(sourceBegin[i+1].x_-sourceEnd[i].x_) )
            {
            temp=abs(sourceBegin[i+1].x_-sourceEnd[i].x_) ;
            ind=i;
            }
    cout << "distance =" << ind << endl;

         }
int tsel;
tsel=(((sourceBegin[ind+1].x_-sourceEnd[ind].x_)/2)+sourceEnd[ind].x_-10);


putText(cdstP, "X", Point(tsel, sourceBegin[ind+1].y_), FONT_HERSHEY_DUPLEX, 1, Scalar(0,255,255), 2);
  cout << "distance =" << tsel << endl;

     // Show results
     imshow("Source", src);
     imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
     imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

     putText(src2, "X", Point(tsel, sourceBegin[ind+1].y_), FONT_HERSHEY_DUPLEX, 1, Scalar(255,255,255), 2);

     imshow("Detected Lines in original", src2);
        waitKey(0);
               return 0;
}
