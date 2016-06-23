#include "find_face_and_eyes.h"

face center_faces(QString image_name)
{
    //image_name = "/home/vgromov/tmp/resize_images/testing/1/image1.jpg";
    face null_face;
    QVector <face> find_faces;
    double scale = 0.3;//scale 0.4 is good

    bool case_value = 0;
    int number_of_true_face = -1;
    while((scale<=0.5)&&(!case_value))
    {
        Mat image;
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
        image = imread(image_name.toStdString(), 1 );
        if(image.empty()) cout << "Couldn't read image" << endl;
        if( !image.empty() )
        {
            detect_Face_and_eyes(image, cascade, nestedCascade, scale, find_faces);
        }
        for(int i = 0; i < find_faces.size(); i++)
        {
            //qDebug()<<find_faces[i].number_eyes()<<"------"<<find_faces.size();
            if(find_faces[i].number_eyes()==2)
            {
                case_value = 1;
                number_of_true_face = i;
            }
        }
        scale+=0.1;
    }
    if(number_of_true_face!=-1)
        return find_faces[number_of_true_face];
    return null_face;
}

void detect_Face_and_eyes( Mat& img, CascadeClassifier& cascade,
                           CascadeClassifier& nestedCascade,
                           double scale, QVector <face> &find_faces)
{
    //ubrat
    double t = 0;
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
    //ubrat


    //vector<Rect> faces;
    Mat gray, smallImg;
    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );//здесь проблемы
    equalizeHist( smallImg, smallImg );
    cascade.detectMultiScale( smallImg, faces,
                              1.1, 2, 0
                              |CASCADE_SCALE_IMAGE,
                              Size(30, 30) );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Scalar color = colors[i%8];//ubrat
        int radius;//ubrat

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
        for ( size_t j = 0; j < nestedObjects.size(); j++ )
        {
            //qDebug()<<"IMSSSS";
            Rect nr = nestedObjects[j];
            center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
            center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
            write_eyes_array.push_back(center);

            radius = cvRound((nr.width + nr.height)*0.25*scale);//ubrat
            circle( img, center, radius, color, 3, 8, 0 );//ubrat
        }
        find_faces[i].set_coord_eyes(write_eyes_array);

        qDebug()<<"IM HERE";
        if(nestedObjects.size()>0)
        {
            imshow( "result", img );//ubrat
            char c = ' ';
            while(c!=27)
                c = cvWaitKey(33);
            //destroyWindow("result");
        }
        else
        {
            imshow( "result", img );//ubrat
        }
    }
}


QVector<Point> face::get_coord_eyes()
{
    return coord_eyes;
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

int face::number_eyes()
{
    return coord_eyes.size();
}

