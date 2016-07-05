#include "find_face_and_eyes.h"

QVector <face> center_faces(QImage frame, QImage* ResultImg, float scale)
{
    //scale 0.4 is good
    QVector <face> null_face;
    QVector <face> find_faces;
    int number_of_true_face = -1;

    CascadeClassifier cascade, nestedCascade;
    string cascadeName = "/home/vgromov/tmp/opencv/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
    string nestedCascadeName = "/home/vgromov/tmp/opencv/opencv/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    if ( !nestedCascade.load( nestedCascadeName ) )
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return null_face;
    }

    Mat image = qimage2mat(frame);
    if(image.empty()) cout << "Couldn't read image" << endl;
    if(!image.empty())
    {
        *ResultImg = Mat2QImage(detect_Face_and_eyes(image, cascade, nestedCascade, scale, find_faces));
    }
    int case_value = 0;
    for(int i = 0; i < find_faces.size(); i++)
    {
        if(find_faces[i].number_eyes()>=1)
        {
            case_value = 1;
            number_of_true_face = i;
        }
    }
    if(number_of_true_face!=-1)
        return find_faces;
    return null_face;
}

Mat detect_Face_and_eyes( Mat& img, CascadeClassifier& cascade,
                          CascadeClassifier& nestedCascade,
                          double scale, QVector <face> &find_faces)
{
    vector<Rect> faces;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;
    cvtColor( img, gray, COLOR_BGR2GRAY);
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );//здесь проблемы
    equalizeHist( smallImg, smallImg );
    cascade.detectMultiScale( smallImg, faces,
                              1.1, 2, 0
                              |CASCADE_SCALE_IMAGE,
                              Size(30, 30) );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Scalar color = colors[i%8];
        int radius;

        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;

        face temp;
        find_faces.push_back(temp);

        double aspect_ratio = (double)r.width/r.height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
        }
        smallImgROI = smallImg( r );
        nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
                                       1.1, 2, 0
                                       |CASCADE_SCALE_IMAGE,
                                       Size(30, 30) );
        find_faces.value(i).set_coord_face(center);
        QVector <Point> write_eyes_array;
        QVector <int> write_radius_eyes_array;
        for ( size_t j = 0; j < nestedObjects.size(); j++ )
        {
            Rect nr = nestedObjects[j];
            center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
            center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
            write_eyes_array.push_back(center);

            radius = cvRound((nr.width + nr.height)*0.25*scale);
            write_radius_eyes_array.push_back(radius);

            circle(img, center, radius, color, 3, 8, 0 );
        }
        find_faces[i].set_coord_eyes(write_eyes_array);
        find_faces[i].set_radius_eyes(write_radius_eyes_array);
    }
    return img;
}

QVector<Point> face::get_coord_eyes()
{
    return coord_eyes;
}

QVector<int> face::get_radius_eyes()
{
    return radius_eyes;
}


Point face::get_coord_face()
{
    return coord_face;
}

void face::set_coord_face(Point set)
{
    coord_face = set;
}

void face::set_coord_eyes(QVector<Point>  set)
{
    coord_eyes = set;
}

void face::set_radius_eyes(QVector<int>  set)
{
    radius_eyes = set;
}

int face::number_eyes()
{
    return coord_eyes.size();
}

QImage convert_lpl_qimg(IplImage* frame)
{
    Mat img = cvarrToMat(frame);
    cv::cvtColor(img,img,CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR
    QImage imdisplay((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    return imdisplay;
}

Mat qimage2mat(const QImage& qimage) {
    cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC3, (uchar*)qimage.bits(), qimage.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    int from_to[] = { 0,0,  1,1,  2,2 };
    cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
    return mat2;
}

QImage Mat2QImage(const Mat &src) {
    cv::Mat temp;
    cvtColor(src, temp,CV_BGR2RGB);
    QImage dest((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    QImage dest2(dest);
    dest2.detach();
    return dest2;
}
