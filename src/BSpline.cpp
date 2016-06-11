#include "BSpline.h"
#include <cstdio>

using namespace Eigen;

BSpline::BSpline()
{
	m_basis << 1.0f, -3.0f,  3.0f, -1.0f,
	           4.0f,  0.0f, -6.0f,  3.0f,
	           1.0f,  3.0f,  3.0f, -3.0f,
	           0.0f,  0.0f,  0.0f,  1.0f;
	m_basis /= 6.0f;
}

void BSpline::addControlPoint(const Vector3f& point)
{
	m_controlPoints.push_back(point);
}

Vector3f BSpline::evaluate(float t)
{
	// Don't be stupid.
	// (Although we could wrap this around too.
	// Implement this if you really want to.)
	if (t < 0)
		t = 0;

	// Index of the first control point in the sliding window.
	size_t windowBegin = t;
	t -= windowBegin;

	// A B-spline needs at least 4 control points.
	// If there are too few, we use the spline endpoint
	// (which is (0,0,0) with less than 4 control points).

	size_t cpSize = m_controlPoints.size();
	if (windowBegin > cpSize - 4)
	{
		windowBegin = cpSize - 4;
		t = 1.0f;
	}
	if (windowBegin < 0)
		return Vector3f::Zero();
	


	Matrix<float, 3, 4> points;
	points << m_controlPoints[windowBegin],
	          m_controlPoints[windowBegin + 1],
	          m_controlPoints[windowBegin + 2],
	          m_controlPoints[windowBegin + 3];

	Vector4f canonicalTimeBasis;
	canonicalTimeBasis << 1, t, t*t, t*t*t;
	return (points * m_basis * canonicalTimeBasis);
}
