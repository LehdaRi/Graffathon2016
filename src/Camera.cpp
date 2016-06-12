#include "Camera.hpp"


using namespace Eigen;


void Camera::perspective(int width, int height, float fov, float near, float far) {
    float n = near;                                         float f = far;
    float AR = width / (float)height;
    float r = std::tan(fov / 2.0f) * n; float l = -r;
    float t = r / AR;                                       float b = -t;

    perspective_ << 2.0f*n/(r-l) , 0.0f         ,  (r+l)/(r-l) ,  0.0f           ,
                    0.0f         , 2.0f*n/(t-b) ,  (t+b)/(t-b) ,  0.0f           ,
                    0.0f         , 0.0f         , -(f+n)/(f-n) , -2.0f*f*n/(f-n) ,
                    0.0f         , 0.0f         , -1.0f        ,  0.0f           ;
}

void Camera::lookAt(const Vector3f& eye, const Vector3f& target, const Vector3f& up) {
    Vector3f z = (eye - target).normalized();
    Vector3f y = (up - up.dot(z) * z).normalized();
    Vector3f x = y.cross(z).normalized();

    Matrix4f rot;
    rot << x , y , z , Vector3f::Zero() ,
           0 , 0 , 0 , 1                ;
    rot.transposeInPlace();

    Matrix4f trans = Matrix4f::Identity();
    trans.col(3) << -eye ,
                     1   ;

    orientation_ = rot * trans;
}

const Matrix4f& Camera::getOrientation(void) const {
    return orientation_;
}

const Matrix4f& Camera::getPerspective(void) const {
    return perspective_;
}

void Camera::setOrientation(const Eigen::Matrix4f& orientation)
{
	orientation_ = orientation;
}

void Camera::setPerspective(const Eigen::Matrix4f& perspective)
{
	perspective_ = perspective;
}
