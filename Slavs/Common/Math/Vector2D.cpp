#include "Vector2D.h"
#include "../Utilities/TemplateFunctions.h"

#include <fstream>
#include <math.h>

Vector2D Vector2D::ZERO = Vector2D(0, 0);

Vector2D::Vector2D(float fX, float fY)
	: x(fX), y(fY)
{}

int Vector2D::Serialize(char* buf_end) const
{
	ToChar(x, buf_end, sizeof(float));
	ToChar(y, buf_end + sizeof(float), sizeof(float));
	buf_end += sizeof(float) << 1;
	return Vector2D::NeededSize();
}

int Vector2D::Deserialize(char* buf_end)
{
	x = FromChar<float>(buf_end);
	y = FromChar<float>(buf_end + sizeof(float));
	return Vector2D::NeededSize();
}

int Vector2D::NeededSize()
{
	return sizeof(float) << 1;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& rhs)
{
	os << " " << rhs.x << " " << rhs.y;

	return os;
}


std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs)
{
	is >> lhs.x >> lhs.y;

	return is;
}


//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D &lhs, float rhs);
inline Vector2D operator*(float lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);