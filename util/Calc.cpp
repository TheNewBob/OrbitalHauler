#include "core/Common.h"
#include "Calc.h"


Calc::Calc()
{
}


Calc::~Calc()
{
}

const  double Calc::RAD_PER_DEGREE = 0.0174532925;

bool Calc::IsNear(const double number, const double compare, const double inaccuracy)
{
	return ((number >= compare - inaccuracy) && (number <= compare + inaccuracy));
}

bool Calc::IsNear(const VECTOR3& v1, const VECTOR3& v2, double inaccuracy)
{
	return IsNear(v1.x, v2.x, inaccuracy) && IsNear(v1.y, v2.y, inaccuracy) && IsNear(v1.z, v2.z, inaccuracy);
}

bool Calc::IsEqual(const double n1, const double n2)
{
	return IsNear(n1, n2, 1e-9);
}

bool Calc::IsEqual(const VECTOR3& v1, const VECTOR3& v2)
{
	return IsEqual(v1.x, v2.x) && Calc::IsEqual(v1.y, v2.y) && Calc::IsEqual(v1.z, v2.z);
}

int Calc::Round(double d)
{
	return (int)(d > 0.0 ? d + 0.5 : d - 0.5);
}

double Calc::Round(double d, double precision)
{
	return Round(d * precision) / precision;
}

void Calc::RoundVector(VECTOR3& v, double precision)
{
	v.x = Round(v.x * precision) / precision;
	v.y = Round(v.y * precision) / precision;
	v.z = Round(v.z * precision) / precision;
}

double Calc::DirectionSimilarity(const VECTOR3& v1, const VECTOR3& v2)
{
	if (length(v1) < .00001 || length(v2) < .00001)
	{
		//vectors too small to return similarity
		return 0;
	}
	return max(dotp(v1, v2) / (length(v1) * length(v2)), 0);
}

bool Calc::IsDirectionSimilar(const VECTOR3& v1, const VECTOR3& v2, double threshold)
{
	return DirectionSimilarity(v1, v2) > threshold;
}
