#ifndef Vector2D_h
#define Vector2D_h
#include <iosfwd>
#include <limits>
#define NOMINMAX
#undef min
#include "../Math/MathUtilities.h"

//////////////////////////////////////////////////////////////////////////
//TODO: float->double
class Vector2D
{
public:
	float x;
	float y;
	Vector2D(float fX = 0, float fY = 0);

	//sets x and y to zero
	void			Zero(){x=0.0; y=0.0;}

	//returns true if both x and y are zero
	bool			isZero()const{return (x*x + y*y) < (std::numeric_limits<float>::min());}

	//returns the length of the vector
	inline float    Length()const;

	//returns the squared length of the vector (thereby avoiding the sqrt)
	inline float    LengthSq()const;

	inline void     Normalize();

	inline float    Dot(const Vector2D& v2)const;

	//returns positive if v2 is clockwise of this vector,
	//negative if anticlockwise (assuming the Y axis is pointing down,
	//X axis to right like a Window app)
	inline int      Sign(const Vector2D& v2)const;

	//returns the vector that is perpendicular to this one.
	inline Vector2D Perp()const;

	//adjusts x and y so that the length of the vector does not exceed max
	inline void     Truncate(float max);

	//returns the distance between this vector and th one passed as a parameter
	inline float    Distance(const Vector2D &v2)const;

	//squared version of above.
	inline float	DistanceSq(const Vector2D &v2)const;

	inline void     Reflect(const Vector2D& norm);

	//returns the vector that is the reverse of this vector
	inline Vector2D GetReverse()const;


	//we need some overloaded operators
	const Vector2D& operator+=(const Vector2D &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2D& operator-=(const Vector2D &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2D& operator*=(const float& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2D& operator/=(const float& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const Vector2D& rhs)const
	{
		return ( MathUtilities::IsEqual(x, rhs.x) 
      && MathUtilities::IsEqual(y,rhs.y) );
	}

	bool operator!=(const Vector2D& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}



	int Serialize(char* buf_end) const;
	int Deserialize(char* buf_end);

	static int NeededSize();
	static Vector2D ZERO;
};
//inline member functions
//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline float Vector2D::Length()const
{
	return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline float Vector2D::LengthSq()const
{
	return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline float Vector2D::Dot(const Vector2D &v2)const
{
	return x*v2.x + y*v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum {clockwise = 1, anticlockwise = -1};

inline int Vector2D::Sign(const Vector2D& v2)const
{
	if (y*v2.x > x*v2.y)
	{ 
		return anticlockwise;
	}
	else 
	{
		return clockwise;
	}
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::Perp()const
{
	return Vector2D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
 inline float Vector2D::Distance(const Vector2D &v2)const
 {
 	float ySeparation = v2.y - y;
 	float xSeparation = v2.x - x;
 
 	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
 }


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline float Vector2D::DistanceSq(const Vector2D &v2)const
{
	float ySeparation = v2.y - y;
	float xSeparation = v2.x - x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2D::Truncate(float max)
{
	if (this->Length() > max)
	{
		this->Normalize();

		*this *= max;
	} 
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
// inline void Vector2D::Reflect(const Vector2D& norm)
// {
// 	*this += 2.0f * this->Dot(norm) * norm.GetReverse();
// }

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::GetReverse()const
{
	return Vector2D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2D::Normalize()
{ 
	float vector_length = this->Length();

	if (vector_length > std::numeric_limits<float>::epsilon())
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}



//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
	Vector2D vec = v;

	float vector_length = vec.Length();

	if (vector_length > std::numeric_limits<float>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const Vector2D& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const Vector2D& v)
{
	return (v.x*v.x + v.y*v.y);
}


//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, float rhs)
{
	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(float lhs, const Vector2D &rhs)
{
	Vector2D result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, float val)
{
	Vector2D result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) {pos.x = 0.0;}

	if (pos.x < 0)    {pos.x = (float)MaxX;}

	if (pos.y < 0)    {pos.y = (float)MaxY;}

	if (pos.y > MaxY) {pos.y = 0.0;}
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
		(p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return !((p.x < top_left.x) || (p.x > bot_rgt.x) || 
		(p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
	return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
	Vector2D facingFirst,
	Vector2D posSecond,
	float    fov)
{
	Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

	return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}


#endif

