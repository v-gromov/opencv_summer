#ifndef FIND_FACE_AND_EYES_H
#define FIND_FACE_AND_EYES_H
//#include <iostream>
#include "mainwindow.h"

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"

#include <QVector>
#include <QImage>

using namespace std;
using namespace cv;

class face
{
public:
    Point get_coord_face() { return coord_face;}
    QVector<Point> get_coord_eyes() {return coord_eyes;}
    QVector<int> get_radius_eyes() {return radius_eyes;}
    void set_coord_face(Point set){coord_face = set;}
    void set_coord_eyes(QVector<Point> set){coord_eyes = set;}
    void set_radius_eyes(QVector<int> set){radius_eyes = set;}
    int number_eyes(){return coord_eyes.size();}
private:
    Point coord_face;
    QVector <Point> coord_eyes;
    QVector <int> radius_eyes;

};

QVector <face> center_faces(QImage, QImage* ResultImg, float);

Mat detect_Face_and_eyes( Mat& img, CascadeClassifier& cascade,
                           CascadeClassifier& nestedCascade,
                           double scale, QVector <face> &find_faces);
QImage convert_lpl_qimg(IplImage*);
IplImage convert_qimg_to_lpl(QImage );
Mat qimage2mat(QImage& qimage);
QImage Mat2QImage(cv::Mat const& src);

#endif // FIND_FACE_AND_EYES_H
