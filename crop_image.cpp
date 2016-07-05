#include "crop_image.h"

using namespace std;

QImage CropFace(QImage a, int left_x, int left_y, int right_x, int right_y, float offset_pct_1, float offset_pct_2, int dest_sz_x, int dest_sz_y);
QImage ScaleRotateTranclate(QImage, float , double* , int , float);

float Distance(double* p1, double* p2)
{
    int dx = p2[0] - p1[0];
    int dy = p2[1] - p1[1];
    return sqrt(dx*dx+dy*dy);
}

QImage ScaleRotateTranclate(QImage aa, float angle, double* center_old, int center_new, float scale)
{
    QImage rotated_img;
    if((scale ==0)&&(center_old[0] == 0)&&(center_old[1]==0))
    {
        QTransform rotating;
        rotating.rotate(angle);
        rotated_img = aa.transformed(rotating);//вращаем все изображение
        return rotated_img;
    }

    int nx = center_old[0], x = center_old[0];
    int ny = center_old[1], y = center_old[1];
    float sx = 1.0, sy = 1.0;
    if(center_new!=0)
        nx=ny=center_new;
    if(scale!=0)
        sx = sy = scale;
    double cosine = cos(angle);
    double sine = sin(angle);
    double a = cosine/(double)sx;
    double b = sine/sx;
    double c = x - nx * a - ny * b;
    double d = (-1)*sine/sy;
    double e = cosine/sy;
    double f = y-nx*d - ny*e;
    QTransform transform_img;
    transform_img.setMatrix(a, b, c, d, e, f, 0, 0, 1);
    aa = aa.transformed(transform_img.toAffine());
    return aa;
}


QImage CropFace(QImage a, int left_x, int left_y, int right_x, int right_y, float offset_pct_1, float offset_pct_2, int dest_sz_x, int dest_sz_y)
{
    int offset_h = offset_pct_1 * dest_sz_x;
    int offset_v = offset_pct_2 * dest_sz_y;
    int eye_direction_x = right_x - left_x;
    int eye_direction_y = right_y - left_y;
    double rotation = (-1)*atan2((float)eye_direction_y, (float)eye_direction_x);
    double eye_left[2];
    eye_left[0] = left_x;
    eye_left[1] = left_y;
    double eye_right[2];
    eye_right[0] = right_x;
    eye_right[1] = right_y;
    float dist = Distance(eye_left, eye_right);
    double reference = dest_sz_x - 2 * offset_h;
    double scale = (float)dist/reference;
    //поворот изображения
    QImage new1 = ScaleRotateTranclate(a, rotation, eye_left, 0, 0);


    double crop_xy[2];
    crop_xy[0] = eye_left[0] - scale*offset_h;
    crop_xy[1] = eye_left[1] - scale*offset_v;

    double crop_size[2];
    crop_size[0] = dest_sz_x*scale;
    crop_size[1] = dest_sz_y*scale;

    QImage face;
    face = new1.copy((int)crop_xy[0], (int)crop_xy[1], (int)(crop_size[0]), (int)crop_size[1]);
    return face;
}

