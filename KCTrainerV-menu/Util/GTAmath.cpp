/**
* Copyright (C) 2007-2010 SlimDX Group
*
* Permission is hereby granted, free  of charge, to any person obtaining a copy of this software  and
* associated  documentation  files (the  "Software"), to deal  in the Software  without  restriction,
* including  without  limitation  the  rights  to use,  copy,  modify,  merge,  publish,  distribute,
* sublicense, and/or sell  copies of the  Software,  and to permit  persons to whom  the Software  is
* furnished to do so, subject to the following conditions:
*
* The  above  copyright  notice  and this  permission  notice shall  be included  in  all  copies  or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF  ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
* NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A   PARTICULAR  PURPOSE  AND
* NONINFRINGEMENT.  IN  NO  EVENT SHALL THE  AUTHORS  OR COPYRIGHT HOLDERS  BE LIABLE FOR  ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  OUT
* OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
* ALTERED SOURCE
* Menyoo PC - Grand Theft Auto V single-player trainer mod
* Copyright (C) 2019  MAFINS
*/

#include "GTAmath.h"
#ifdef VECTOR_EXTRA
// Vector3M


#include <math.h>
#include <random>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>


#pragma region Vector3M
Vector3M::Vector3M(float X, float Y, float Z)
	: x(X), y(Y), z(Z)
{
}

Vector3M::Vector3M()
	: x(0), y(0), z(0)
{
}

Vector3M::Vector3M(const Vector3M_t& xyz)
	: x(xyz.x), y(xyz.y), z(xyz.z)
{
}

void Vector3M::clear()
{
	*this = Vector3M();
}

Vector3M Vector3M::Zero()
{
	return Vector3M(0.0f, 0.0f, 0.0f);
}

Vector3M Vector3M::One()
{
	return Vector3M(1.0f, 1.0f, 1.0f);
}

Vector3M Vector3M::WorldUp()
{
	return Vector3M(0.0f, 0.0f, 1.0f);
}
Vector3M Vector3M::WorldDown()
{
	return Vector3M(0.0f, 0.0f, -1.0f);
}
Vector3M Vector3M::WorldNorth()
{
	return Vector3M(0.0f, 1.0f, 0.0f);
}
Vector3M Vector3M::WorldSouth()
{
	return Vector3M(0.0f, -1.0f, 0.0f);
}
Vector3M Vector3M::WorldEast()
{
	return Vector3M(1.0f, 0.0f, 0.0f);
}
Vector3M Vector3M::WorldWest()
{
	return Vector3M(-1.0f, 0.0f, 0.0f);
}

Vector3M Vector3M::RelativeRight()
{
	return Vector3M(1.0f, 0.0f, 0.0f);
}
Vector3M Vector3M::RelativeLeft()
{
	return Vector3M(-1.0f, 0.0f, 0.0f);
}
Vector3M Vector3M::RelativeFront()
{
	return Vector3M(0.0f, 1.0f, 0.0f);
}
Vector3M Vector3M::RelativeBack()
{
	return Vector3M(0.0f, -1.0f, 0.0f);
}
Vector3M Vector3M::RelativeTop()
{
	return Vector3M(0.0f, 0.0f, 1.0f);
}
Vector3M Vector3M::RelativeBottom()
{
	return Vector3M(0.0f, 0.0f, -1.0f);
}

float Vector3M::Length() const
{
	return static_cast<float>(sqrt((x*x) + (y*y) + (z*z)));
}
float Vector3M::LengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

void Vector3M::Normalize()
{
	float length = this->Length();
	if (length == 0.0f)
		return;
	float num = 1.0f / length;
	x *= num;
	y *= num;
	z *= num;
}

float Vector3M::DistanceTo(const Vector3M& position) const
{
	return (position - *this).Length();
}

Vector3M Vector3M::Around(float distance) const
{
	return *this + Vector3M::RandomXY() * distance;
}

Vector3M Vector3M::PointOnCircle(float radius, float angleInDegrees) const
{
	Vector3M point;
	point.x = radius * cos(DegreeToRadian(angleInDegrees)) + this->x;
	point.y = radius * sin(DegreeToRadian(angleInDegrees)) + this->y;
	point.z = this->z;

	return point;
}

void Vector3M::PointsOnCircle(std::vector<Vector3M>& results, float fullRadius, float angleDifference, float intervalDistance, bool includeCentre) const
{
	//results.clear();

	auto& origin = *this;
	Vector3M current;
	float u, d;

	if (includeCentre)
		results.push_back(origin);
	if (intervalDistance == 0.0f)
		return;

	for (u = 0.0f; u < 360.0f; u += angleDifference)
	{
		for (d = intervalDistance; d < fullRadius; d += intervalDistance)
		{
			current.x = d * cos(DegreeToRadian(u)) + origin.x;
			current.y = d * sin(DegreeToRadian(u)) + origin.y;
			//current.z = origin.z; // this will stay uniform as we're only considering the circle
			results.push_back(current);
		}
	}

}

Vector3M Vector3M::PointOnSphere(float radius, float longitude, float latitude) const
{
	float u = DegreeToRadian(longitude);
	float v = DegreeToRadian(latitude);
	Vector3M point;
	point.x = radius * sin(u) * cos(v) + this->x;
	point.y = radius * cos(u) * cos(v) + this->y;
	point.z = radius * sin(v) + this->z;

	return point;
}

Vector3M_t Vector3M::ToTypeStruct() const
{
	Vector3M_t Return;
	Return.x = this->x;
	Return.y = this->y;
	Return.z = this->z;
	return Return;
}

void Vector3M::ToArray(float* out) const
{
	out[0] = this->x;
	out[1] = this->y;
	out[2] = this->z;
}

std::array<float,3> Vector3M::ToArray() const
{
	return{ { this->x, this->y, this->z } };
}

bool Vector3M::IsZero() const
{
	return (this->x == 0 && this->y == 0 && this->z == 0);
}

Vector3M Vector3M::RandomXY()
{
	Vector3M v;
	v.x = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.y = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.z = 0.0f;
	v.Normalize();
	return v;
}
Vector3M Vector3M::RandomXYZ()
{
	Vector3M v;
	v.x = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.y = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.z = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.Normalize();
	return v;
}

Vector3M Vector3M::Add(Vector3M left, Vector3M right)
{
	return Vector3M(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3M Vector3M::Subtract(Vector3M left, Vector3M right)
{
	return Vector3M(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3M Vector3M::Multiply(Vector3M value, float scale)
{
	return Vector3M(value.x * scale, value.y * scale, value.z * scale);
}

Vector3M Vector3M::Modulate(Vector3M left, Vector3M right)
{
	return Vector3M(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vector3M Vector3M::Divide(Vector3M value, float scale)
{
	return Vector3M(value.x / scale, value.y / scale, value.z / scale);
}

Vector3M Vector3M::Negate(Vector3M value)
{
	return Vector3M(-value.x, -value.y, -value.z);
}

Vector3M Vector3M::Clamp(Vector3M value, Vector3M min, Vector3M max)
{
	float x = value.x;
	x = (x > max.x) ? max.x : x;
	x = (x < min.x) ? min.x : x;

	float y = value.y;
	y = (y > max.y) ? max.y : y;
	y = (y < min.y) ? min.y : y;

	float z = value.z;
	z = (z > max.z) ? max.z : z;
	z = (z < min.z) ? min.z : z;

	return Vector3M(x, y, z);
}

Vector3M Vector3M::Lerp(Vector3M start, Vector3M end, float amount)
{
	Vector3M vector;

	vector.x = start.x + ((end.x - start.x) * amount);
	vector.y = start.y + ((end.y - start.y) * amount);
	vector.z = start.z + ((end.z - start.z) * amount);

	return vector;
}

Vector3M Vector3M::Normalize(Vector3M vector)
{
	vector.Normalize();
	return vector;
}

float Vector3M::Dot(Vector3M left, Vector3M right)
{
	return (left.x * right.x + left.y * right.y + left.z * right.z);
}

Vector3M Vector3M::Cross(Vector3M left, Vector3M right)
{
	Vector3M result;
	result.x = left.y * right.z - left.z * right.y;
	result.y = left.z * right.x - left.x * right.z;
	result.z = left.x * right.y - left.y * right.x;
	return result;
}

Vector3M Vector3M::Reflect(Vector3M vector, Vector3M normal)
{
	Vector3M result;
	float dot = ((vector.x * normal.x) + (vector.y * normal.y)) + (vector.z * normal.z);

	result.x = vector.x - ((2.0f * dot) * normal.x);
	result.y = vector.y - ((2.0f * dot) * normal.y);
	result.z = vector.z - ((2.0f * dot) * normal.z);

	return result;
}

Vector3M Vector3M::Minimize(Vector3M value1, Vector3M value2)
{
	Vector3M vector;
	vector.x = (value1.x < value2.x) ? value1.x : value2.x;
	vector.y = (value1.y < value2.y) ? value1.y : value2.y;
	vector.z = (value1.z < value2.z) ? value1.z : value2.z;
	return vector;
}

Vector3M Vector3M::Maximize(Vector3M value1, Vector3M value2)
{
	Vector3M vector;
	vector.x = (value1.x > value2.x) ? value1.x : value2.x;
	vector.y = (value1.y > value2.y) ? value1.y : value2.y;
	vector.z = (value1.z > value2.z) ? value1.z : value2.z;
	return vector;
}

Vector3M operator + (Vector3M const& left, Vector3M const& right)
{
	return Vector3M(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3M operator - (Vector3M const& left, Vector3M const& right)
{
	return Vector3M(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3M operator - (Vector3M const& value)
{
	return Vector3M(-value.x, -value.y, -value.z);
}

Vector3M operator * (Vector3M const& left, Vector3M const& right)
{
	return Vector3M(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vector3M operator * (Vector3M const& value, float const& scale)
{
	return Vector3M(value.x * scale, value.y * scale, value.z * scale);
}

Vector3M operator * (float const& scale, Vector3M const& value)
{
	return Vector3M(value.x * scale, value.y * scale, value.z * scale);
}

Vector3M operator *= (Vector3M& value, float const& scale)
{
	value.x *= scale;
	value.y *= scale;
	value.z *= scale;
	return value;
}

Vector3M operator / (Vector3M const& left, Vector3M const& right)
{
	return Vector3M(left.x / right.x, left.y / right.y, left.z / right.z);
}

Vector3M operator / (Vector3M const& value, float const& scale)
{
	return Vector3M(value.x / scale, value.y / scale, value.z / scale);
}

Vector3M operator /= (Vector3M& value, float const& scale)
{
	value.x /= scale;
	value.y /= scale;
	value.z /= scale;
	return value;
}

bool operator == (Vector3M const& left, Vector3M const& right)
{
	return Vector3M::Equals(left, right);
}

bool operator != (Vector3M const& left, Vector3M const& right)
{
	return !Vector3M::Equals(left, right);
}


std::string Vector3M::ToString() const
{
	std::stringstream ss;
	ss << "X:" << std::setprecision(4) << std::fixed << this->x;
	ss << " Y:" << std::setprecision(4) << std::fixed << this->y;
	ss << " Z:" << std::setprecision(4) << std::fixed << this->z;
	return ss.str();
}

bool Vector3M::Equals(const Vector3M& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3M::Equals(const Vector3M_t& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3M::Equals(const Vector3M& value1, const Vector3M& value2)
{
	return (value1.x == value2.x && value1.y == value2.y && value1.z == value2.z);
}


float Vector3M::DistanceBetween(const Vector3M& value1, const Vector3M& value2)
{
	return (value1 - value2).Length();
}

Vector3M Vector3M::RotationToDirection(const Vector3M& rotation)
{
	float retz = rotation.z * 0.0174532924F; // Degree to radian
	float retx = rotation.x * 0.0174532924F;
	float absx = abs(cos(retx));
	return Vector3M(-sin(retz) * absx, cos(retz) * absx, sin(retx));
}
Vector3M Vector3M::DirectionToRotation(Vector3M direction)
{
	direction.Normalize();
	float xx = atan2(direction.z, direction.y) / 0.0174532924F; // Radian to degree
	float yy = 0;
	float zz = -atan2(direction.x, direction.y) / 0.0174532924F;
	return Vector3M(xx, yy, zz);
}

#pragma endregion

#pragma region Vector2M
Vector2M::Vector2M(float X, float Y)
	: x(X), y(Y)
{
}

Vector2M::Vector2M()
	: x(0), y(0)
{
}

void Vector2M::clear()
{
	*this = Vector2M(0.0f, 0.0f);
}

Vector2M Vector2M::Zero()
{
	return Vector2M(0.0f, 0.0f);
}

Vector2M Vector2M::One()
{
	return Vector2M(1.0f, 1.0f);
}

Vector2M Vector2M::Up()
{
	return Vector2M(0.0f, 1.0);
}

Vector2M Vector2M::Down()
{
	return Vector2M(0.0f, -1.0f);
}

Vector2M Vector2M::Right()
{
	return Vector2M(1.0f, 0.0f);
}

Vector2M Vector2M::Left()
{
	return Vector2M(-1.0f, 0.0f);
}

float Vector2M::Length() const
{
	return sqrt((x*x) + (y*y));
}

Vector2M Vector2M::PointOnCircle(float radius, float angleInDegrees) const
{
	Vector2M point;
	point.x = radius * cos(DegreeToRadian(angleInDegrees)) + this->x;
	point.y = radius * sin(DegreeToRadian(angleInDegrees)) + this->y;

	return point;
}

void Vector2M::PointsOnCircle(std::vector<Vector2M>& results, float fullRadius, float angleInDegrees, float intervalDistance, bool includeCentre) const
{
	//results.clear();

	auto& origin = *this;
	Vector2M current;
	float u, d;

	if (includeCentre) results.push_back(origin);
	if (intervalDistance == 0.0f) return;

	for (u = 0.0f; u < 360.0f; u += angleInDegrees)
	{
		for (d = intervalDistance; d < fullRadius; d += intervalDistance)
		{
			current.x = d * cos(DegreeToRadian(u)) + origin.x;
			current.y = d * sin(DegreeToRadian(u)) + origin.y;
			results.push_back(current);
		}
	}
}

float Vector2M::LengthSquared() const
{
	return (x * x) + (y * y);
}

void Vector2M::Normalize()
{
	float length = Length();
	if (length == 0.0f)
		return;
	float num = 1.0f / length;
	x *= num;
	y *= num;
}

float Vector2M::DistanceTo(const Vector2M& position) const
{
	return (position - *this).Length();
}

std::array<float,2> Vector2M::ToArray() const
{
	return{ {this->x, this->y} };
}

bool Vector2M::IsZero() const
{
	return (this->x == 0 && this->y == 0);
}

Vector2M Vector2M::RandomXY()
{
	Vector2M v;
	v.x = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.y = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.Normalize();
	return v;
}

Vector2M Vector2M::Add(Vector2M const& left, Vector2M const& right)
{
	return Vector2M(left.x + right.x, left.y + right.y);
}

Vector2M Vector2M::Subtract(Vector2M const& left, Vector2M const& right)
{
	return Vector2M(left.x - right.x, left.y - right.y);
}

Vector2M Vector2M::Multiply(Vector2M const& value, float const& scale)
{
	return Vector2M(value.x * scale, value.y * scale);
}

Vector2M Vector2M::Modulate(Vector2M const& left, Vector2M const& right)
{
	return Vector2M(left.x * right.x, left.y * right.y);
}

Vector2M Vector2M::Divide(Vector2M const& value, float const& scale)
{
	return Vector2M(value.x / scale, value.y / scale);
}

Vector2M Vector2M::Negate(Vector2M const& value)
{
	return Vector2M(-value.x, -value.y);
}

Vector2M Vector2M::Clamp(Vector2M const& value, Vector2M const& min, Vector2M const& max)
{
	float ex = value.x;
	ex = (ex > max.x) ? max.x : ex;
	ex = (ex < min.x) ? min.x : ex;

	float why = value.y;
	why = (why > max.y) ? max.y : why;
	why = (why < min.y) ? min.y : why;

	return Vector2M(ex, why);
}

Vector2M Vector2M::Lerp(Vector2M const& start, Vector2M const& end, float const& amount)
{
	Vector2M vector;

	vector.x = start.x + ((end.x - start.x) * amount);
	vector.y = start.y + ((end.y - start.y) * amount);

	return vector;
}

Vector2M Vector2M::Normalize(Vector2M value)
{
	value.Normalize();
	return value;
}

float Vector2M::Dot(Vector2M const& left, Vector2M const& right)
{
	return (left.x * right.x + left.y * right.y);
}

Vector2M Vector2M::Reflect(Vector2M const& vector, Vector2M const& normal)
{
	Vector2M result;
	float dot = ((vector.x * normal.x) + (vector.y * normal.y));

	result.x = vector.x - ((2.0f * dot) * normal.x);
	result.y = vector.y - ((2.0f * dot) * normal.y);

	return result;
}

Vector2M Vector2M::Minimize(Vector2M const& value1, Vector2M const& value2)
{
	Vector2M vector;
	vector.x = (value1.x < value2.x) ? value1.x : value2.x;
	vector.y = (value1.y < value2.y) ? value1.y : value2.y;
	return vector;
}

Vector2M Vector2M::Maximize(Vector2M value1, Vector2M value2)
{
	Vector2M vector;
	vector.x = (value1.x > value2.x) ? value1.x : value2.x;
	vector.y = (value1.y > value2.y) ? value1.y : value2.y;
	return vector;
}

Vector2M operator + (Vector2M const& left, Vector2M const& right)
{
	return Vector2M(left.x + right.x, left.y + right.y);
}

Vector2M operator - (Vector2M const& left, Vector2M const& right)
{
	return Vector2M(left.x - right.x, left.y - right.y);
}

Vector2M operator - (Vector2M const& value)
{
	return Vector2M(-value.x, -value.y);
}

Vector2M operator * (Vector2M const& vec, float const& scale)
{
	return Vector2M(vec.x * scale, vec.y * scale);
}

Vector2M operator * (float const& scale, Vector2M const& vec)
{
	return vec * scale;
}

Vector2M operator / (Vector2M const& vec, float const& scale)
{
	return Vector2M(vec.x / scale, vec.y / scale);
}

bool operator == (Vector2M const& left, Vector2M const& right)
{
	return Vector2M::Equals(left, right);
}

bool operator != (Vector2M const& left, Vector2M const& right)
{
	return !Vector2M::Equals(left, right);
}


std::string Vector2M::ToString() const
{
	//return "X:" + std::to_string(x) + " Y:" + std::to_string(y);
	std::stringstream ss;
	ss << "X:" << std::setprecision(4) << std::fixed << this->x;
	ss << " Y:" << std::setprecision(4) << std::fixed << this->y;
	return ss.str();
}


bool Vector2M::Equals(const Vector2M& other) const
{
	return (x == other.x && y == other.y);
}

bool Vector2M::Equals(Vector2M value1, Vector2M value2)
{
	return (value1.x == value2.x && value1.y == value2.y);
}
#pragma endregion


int get_random_int_in_range(int min, int max)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> u(min, max); // inclusive
	return u(rng);
}
float get_random_float_in_range(float min, float max)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	//boost::uniform_real<float> u(min, max);
	std::uniform_real_distribution<float> u(min, max); // inclusive
	//boost::variate_generator< boost::mt19937&, boost::uniform_real<float> > gen(rng, u);
	return u(rng);
	//return gen();
}

float DegreeToRadian(float angle)
{
	return angle * 0.0174532925199433F;
}
float RadianToDegree(float angle)
{
	return angle / 0.0174532925199433F;
}

Vector3M DegreeToRadian(const Vector3M& angles)
{
	return Vector3M(angles.x * 0.0174532925199433F, angles.y * 0.0174532925199433F, angles.z * 0.0174532925199433F);
}
float GetHeadingFromCoords(const Vector3M& source, const Vector3M& target)
{
	return atan2((target.y - source.y), (target.x - source.x));
}

#endif // VECTOR_EXTRA