#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
#include <Eigen/Dense>

class BSpline {
public:
	BSpline();

	void addControlPoint(const Eigen::Vector3f&);
	Eigen::Vector3f evaluate(float t);
private:
	Eigen::Matrix4f m_basis;
	std::vector<Eigen::Vector3f> m_controlPoints;
};
#endif // BSPLINE_H
