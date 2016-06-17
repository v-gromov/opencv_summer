#ifndef FIND_FACE_AND_EYES_H
#define FIND_FACE_AND_EYES_H
#include "mainwindow.h"
using namespace std;
using namespace cv;

class face
{
public:
    Point get_coord_face();
    QVector<Point> get_coord_eyes();
    void set_coord_face(Point);
    void set_coord_eyes(QVector<Point>);
    int number_eyes();
private:
    Point coord_face;
    QVector <Point> coord_eyes;
};

face center_faces(QString image_name);

void detect_Face_and_eyes( Mat& img, CascadeClassifier& cascade,
                           CascadeClassifier& nestedCascade,
                           double scale, QVector <face> &find_faces);

#endif // FIND_FACE_AND_EYES_H
