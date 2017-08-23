#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d.hpp>
#include <iostream>


using namespace cv;
using namespace std;



int main(int argc, char** argv) {

    // Open the image
    String filename = "../data_in/test_01.JPG";
    Mat img = imread(filename, IMREAD_GRAYSCALE);

    // Make sure that we have an image
    if(img.empty()) {
        cout << "can not open " << filename << endl;
        return EXIT_FAILURE;
    }

    // Threshold the image
    Mat img_thresh;
    double thresh = 80;
    double maxValue = 255;
    threshold(img,img_thresh,thresh,maxValue, THRESH_BINARY_INV);

    // Display
    namedWindow("thresholded image", WINDOW_FREERATIO);
    imshow("thresholded image", img_thresh);
    waitKey(10);

    // Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img_thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

//    // Remove small and large contours
//    // https://stackoverflow.com/a/18108466/7718197
//    int contour_length_threshold = 10;
//    for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end(); ) {
//        if (it->size()<contour_length_threshold)
//            it=contours.erase(it);
//        else
//            ++it;
//    }

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly(contours.size());
    vector<Point2f> center( contours.size());
    vector<float> radius(contours.size());
    for( int i = 0; i < contours.size(); i++ ) {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true);
        minEnclosingCircle(contours_poly[i], center[i], radius[i]);
    }


    // Debug
    cout << "found " << contours.size() << " contours" << endl;



    // Draw contours
    RNG rng(12345);
    Mat drawing = Mat::zeros(img_thresh.size(), CV_8UC3);

    // Find the minimun enclosing circle
    for(int i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar(rng.uniform(100, 255), rng.uniform(100,255), rng.uniform(100,255));
        drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        circle(drawing, center[i], (int)radius[i], color, 2, 8, 0 );
    }


    // Display
    namedWindow("results", WINDOW_FREERATIO);
    imshow("results", drawing);
    waitKey(0);



    return EXIT_SUCCESS;
}
