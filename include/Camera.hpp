#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <Eigen/Dense>


class Camera {
public:
    void lookAt(const Eigen::Vector3f& eye,
                const Eigen::Vector3f& target   = Eigen::Vector3f(0, 0, 0),
                const Eigen::Vector3f& up       = Eigen::Vector3f(0, 1, 0));
    void perspective(int width, int height, float fov, float near = 0.1f, float far = 15.0f);

    const Eigen::Matrix4f& getOrientation(void) const;
    const Eigen::Matrix4f& getPerspective(void) const;
    const Eigen::Vector3f& getPosition(void) const;

	void setOrientation(const Eigen::Matrix4f&);
	void setPerspective(const Eigen::Matrix4f&);

private:
    Eigen::Matrix4f orientation_;
    Eigen::Matrix4f perspective_;
    Eigen::Vector3f position_;
};


#endif // CAMERA_HPP
