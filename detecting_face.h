#ifndef DETECTING_FACE_H
#define DETECTING_FACE_H

#include "opencv2/face.hpp"

//#include "opencv2"
#include <QString>
#include <QImage>

using namespace cv;
//using namespace cv::face;
using namespace std;

class face_model
{
public:
    face_model();
    int get_numb_people(QImage);
private:
    void read_csv(const string&, vector<Mat>&, vector<int>&);
    Mat norm_0_255(InputArray);
    Ptr <BasicFaceRecognizer> model;
    Mat qimage2mat(const QImage& );
};

#endif // DETECTING_FACE_H
