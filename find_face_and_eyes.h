#ifndef FIND_FACE_AND_EYES_H
#define FIND_FACE_AND_EYES_H
#include <iostream>
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
    Point get_coord_face();
    QVector<Point> get_coord_eyes();
    QVector<int> get_radius_eyes();
    void set_coord_face(Point);
    void set_coord_eyes(QVector<Point>);
    void set_radius_eyes(QVector<int>);
    int number_eyes();
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
Mat qimage2mat(const QImage& qimage);
QImage Mat2QImage(cv::Mat const& src);

#endif // FIND_FACE_AND_EYES_H
