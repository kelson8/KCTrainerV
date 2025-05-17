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

#pragma once

// I renamed this from Vector3 to Vector3M, it was conflicting with ScriptHookV
// Also, I renamed Vector2 to Vector2M
// This now builds! I also got this to work with GTAmemory.cpp, now to fix the other files.

#define VECTOR_EXTRA

//#ifdef MENYOO_SCRIPT_FILES
#ifdef VECTOR_EXTRA

#include <vector>
#include <array>
#include <string>

typedef unsigned long DWORD;

#define MATH_PI 3.14159265

#pragma pack(push, 1)
typedef struct
{
	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;
} Vector3M_t;
#pragma pack(pop)

class Vector3M
{
public:
	/// <summary>
	/// Gets or sets the x component of the vector.
	/// </summary>
	/// <value>The x component of the vector.</value>
	float x;

	/// <summary>
	/// Gets or sets the y component of the vector.
	/// </summary>
	/// <value>The y component of the vector.</value>
	float y;

	/// <summary>
	/// Gets or sets the z component of the vector.
	/// </summary>
	/// <value>The z component of the vector.</value>
	float z;

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector3M"/> class.
	/// </summary>
	/// <param name="x">Initial value for the x component of the vector.</param>
	/// <param name="y">Initial value for the y component of the vector.</param>
	/// <param name="z">Initial value for the z component of the vector.</param>
	Vector3M(float X, float Y, float Z);

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector3M"/> class.
	/// </summary>
	Vector3M();

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector3M"/> class.
	/// </summary>
	/// <param name="xyz">Initial value of the x,y,z components.</param>
	Vector3M(const Vector3M_t& xyz);

	/// <summary>
	/// Nulls the vector (0,0,0)
	/// </summary>
	void clear();

	/// <summary>
	/// Returns a null vector. (0,0,0)
	/// </summary>
	static Vector3M Zero();

	/// <summary>
	/// Returns a vector with all components as '1'. (1,1,1)
	/// </summary>
	static Vector3M One();

	/// <summary>
	/// Returns the world Up vector. (0,0,1)
	/// </summary>
	static Vector3M WorldUp();
	/// <summary>
	/// Returns the world Down vector. (0,0,-1)
	/// </summary>
	static Vector3M WorldDown();
	/// <summary>
	/// Returns the world North vector. (0,1,0)
	/// </summary>
	static Vector3M WorldNorth();
	/// <summary>
	/// Returns the world South vector. (0,-1,0)
	/// </summary>
	static Vector3M WorldSouth();
	/// <summary>
	/// Returns the world East vector. (1,0,0)
	/// </summary>
	static Vector3M WorldEast();
	/// <summary>
	/// Returns the world West vector. (-1,0,0)
	/// </summary>
	static Vector3M WorldWest();

	/// <summary>
	/// Returns the relative Right vector. (1,0,0)
	/// </summary>
	static Vector3M RelativeRight();
	/// <summary>
	/// Returns the relative Left vector. (-1,0,0)
	/// </summary>
	static Vector3M RelativeLeft();
	/// <summary>
	/// Returns the relative Front vector. (0,1,0)
	/// </summary>
	static Vector3M RelativeFront();
	/// <summary>
	/// Returns the relative Back vector. (0,-1,0)
	/// </summary>
	static Vector3M RelativeBack();
	/// <summary>
	/// Returns the relative Top vector. (0,0,1)
	/// </summary>
	static Vector3M RelativeTop();
	/// <summary>
	/// Returns the relative Bottom vector as used. (0,0,-1)
	/// </summary>
	static Vector3M RelativeBottom();


	/// <summary>
	/// Calculates the length of the vector.
	/// </summary>
	/// <returns>The length of the vector.</returns>
	float Length() const;

	/// <summary>
	/// Calculates the squared length of the vector.
	/// </summary>
	/// <returns>The squared length of the vector.</returns>
	float LengthSquared() const;

	/// <summary>
	/// Converts the vector into a unit vector.
	/// </summary>
	void Normalize();

	/// <summary>
	/// Calculates the distance between two vectors.
	/// </summary>
	/// <param name="position">The second vector to calculate the distance to.</param>
	/// <returns>The distance to the other vector.</returns>
	float DistanceTo(const Vector3M& position) const;

	/// <summary>
	/// Creates a random vector inside the circle around this position.
	/// </summary>
	/// <param name="distance">The distance from the vector within the new vector will lie.</param>
	/// <returns>A random vector around the position</returns>
	Vector3M Around(float distance) const;

	/// <summary>
	/// Gets a point on the circumference of a circle of the provided radius. Z axis ignored.
	/// </summary>
	/// <param name="radius">The radius of the circle.</param>
	/// <param name="angleInDegrees">The angle (in degrees) that the new point will have from 0.</param>
	/// <returns>A point on the circumference of a circle of the provided radius.</returns>
	Vector3M PointOnCircle(float radius, float angleInDegrees) const;

	/// <summary>
	/// Creates a list of points around this position within the given radius. Z axis ignored.
	/// </summary>
	/// <param name="results">The output points. The collection is not cleared before adding in points.</param>
	/// <param name="fullRadius">The (boundry) radius of the circle to be made.</param>
	/// <param name="angleDifference">The difference in angles (in degrees) of each 2 points.</param>
	/// <param name="intervalDistance">The distance between 2 consecutive points.</param>
	/// <param name="includeCentre">Determines whether the origin should also be included in the list of points.</param>
	///// <returns>A list of points around this position within the given radius.</returns>
	void PointsOnCircle(std::vector<Vector3M>& results, float fullRadius, float angleDifference, float intervalDistance, bool includeCentre = false) const;

	///<summary>
	///Gets a point on the surface of a sphere
	///</summary>
	///<param name="radius">The radius of the sphere</param>
	///<param name="longitude">The longitude/pitch, in degrees, ranges from 0 to 360</param>
	///<param name="latitude">The latitude/yaw, in degrees, ranges from -90 to +90</param>
	Vector3M PointOnSphere(float radius, float longitude, float latitude) const;

	/// <summary>
	/// Gets a point on the circumference of a sphere of the provided radius.
	/// </summary>
	/// <param name="radius">The radius of the sphere.</param>
	/// <param name="angleInDegrees">The angle (in degrees) that the new point will have from 0.</param>
	/// <returns>A point on the circumference of a sphere of the provided radius.</returns>
	/*Vector3M PointOnSphere(float radius, float angleInDegrees)
	{
		Vector3M point;
		point.x = this->x + radius * sin(u) * cos(v);
		point.y = this->x + radius * cos(u) * cos(v);
		point.z = this->z + radius * sin(v);

		return point;
	}*/

	/// <summary>
	/// Creates a list of points around this position within the given radius.
	/// </summary>
	/// <param name="fullRadius">The (boundry) radius of the sphere to be made.</param>
	/// <param name="angleInDegrees">The difference in angles (in degrees) of each 2 points.</param>
	/// <param name="intervalDistance">The distance between 2 consecutive points.</param>
	/// <param name="includeCentre">Determines whether the origin should also be included in the list of points.</param>
	/// <returns>A list of points around this position within the given radius.</returns>
	/*std::vector<Vector3M> PointsOnSphere(float fullRadius, float angleInDegrees, float intervalDistance, bool includeCentre = false)
	{
		std::vector<Vector3M> points;

		auto& origin = *this;
		Vector3M current;
		float u, d;

		if (includeCentre) points.push_back(origin);
		if (intervalDistance == 0.0f) return points;

		for (u = 0.0f; u < 360.0f; u += angleInDegrees)
		{
			for (d = intervalDistance; d < fullRadius; d += intervalDistance)
			{
				current.x = d * cos(DegreeToRadian(u)) + origin.x;
				current.y = d * sin(DegreeToRadian(u)) + origin.y;
				current.z = origin.z; // this will stay uniform as we're only considering the circle
				points.push_back(current);
			}
		}

		return points;
	}*/


	/// <summary>
	/// Returns the vector as a float,[4 bytes],float,[4 bytes],float,[4 bytes] struct.
	/// </summary>
	/// <returns>The vector as a float,[4 bytes],float,[4 bytes],float,[4 bytes] struct.</returns>
	Vector3M_t ToTypeStruct() const;

	/// <summary>
	/// Copies the vector coordinates into a float array.
	/// </summary>
	/// <param name="out">The output array (needs 12 allocated bytes).</param>
	void ToArray(float* out) const;

	/// <summary>
	/// Copies the vector coordinates into a float array.
	/// </summary>
	/// <returns>The vector as an std::array of floats.</returns>
	std::array<float,3> ToArray() const;

	/// <summary>
	/// Returns true if the vector is 0,0,0
	/// </summary>
	bool IsZero() const;

	/// <summary>
	/// Returns a new normalized vector with random X and Y components.
	/// </summary>
	static Vector3M RandomXY();

	/// <summary>
	/// Returns a new normalized vector with random X, Y and Z components.
	/// </summary>
	static Vector3M RandomXYZ();

	/// <summary>
	/// Adds two vectors.
	/// </summary>
	/// <param name="left">The first vector to add.</param>
	/// <param name="right">The second vector to add.</param>
	/// <returns>The sum of the two vectors.</returns>
	static Vector3M Add(Vector3M left, Vector3M right);

	/// <summary>
	/// Subtracts two vectors.
	/// </summary>
	/// <param name="left">The first vector to subtract.</param>
	/// <param name="right">The second vector to subtract.</param>
	/// <returns>The difference of the two vectors.</returns>
	static Vector3M Subtract(Vector3M left, Vector3M right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	static Vector3M Multiply(Vector3M value, float scale);

	/// <summary>
	/// Modulates a vector by another.
	/// </summary>
	/// <param name="left">The first vector to modulate.</param>
	/// <param name="right">The second vector to modulate.</param>
	/// <returns>The modulated vector.</returns>
	static Vector3M Modulate(Vector3M left, Vector3M right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	static Vector3M Divide(Vector3M value, float scale);

	/// <summary>
	/// Reverses the direction of a given vector.
	/// </summary>
	/// <param name="value">The vector to negate.</param>
	/// <returns>A vector facing in the opposite direction.</returns>
	static Vector3M Negate(Vector3M value);

	/// <summary>
	/// Restricts a value to be within a specified range.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="min">The minimum value.</param>
	/// <param name="max">The maximum value.</param>
	/// <returns>The clamped value.</returns>
	static Vector3M Clamp(Vector3M value, Vector3M min, Vector3M max);

	/// <summary>
	/// Performs a linear interpolation between two vectors.
	/// </summary>
	/// <param name="start">Start vector.</param>
	/// <param name="end">End vector.</param>
	/// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
	/// <returns>The linear interpolation of the two vectors.</returns>
	/// <remarks>
	/// This method performs the linear interpolation based on the following formula.
	/// <code>start + (end - start) * amount</code>
	/// Passing <paramref name="amount"/> a value of 0 will cause <paramref name="start"/> to be returned; a value of 1 will cause <paramref name="end"/> to be returned. 
	/// </remarks>
	static Vector3M Lerp(Vector3M start, Vector3M end, float amount);

	/// <summary>
	/// Converts the vector into a unit vector.
	/// </summary>
	/// <param name="vector">The vector to normalize.</param>
	/// <returns>The normalized vector.</returns>
	static Vector3M Normalize(Vector3M vector);

	/// <summary>
	/// Calculates the dot product of two vectors.
	/// </summary>
	/// <param name="left">First source vector.</param>
	/// <param name="right">Second source vector.</param>
	/// <returns>The dot product of the two vectors.</returns>
	static float Dot(Vector3M left, Vector3M right);

	/// <summary>
	/// Calculates the cross product of two vectors.
	/// </summary>
	/// <param name="left">First source vector.</param>
	/// <param name="right">Second source vector.</param>
	/// <returns>The cross product of the two vectors.</returns>
	static Vector3M Cross(Vector3M left, Vector3M right);

	/// <summary>
	/// Returns the reflection of a vector off a surface that has the specified normal. 
	/// </summary>
	/// <param name="vector">The source vector.</param>
	/// <param name="normal">Normal of the surface.</param>
	/// <returns>The reflected vector.</returns>
	/// <remarks>Reflect only gives the direction of a reflection off a surface, it does not determine 
	/// whether the original vector was close enough to the surface to hit it.</remarks>
	static Vector3M Reflect(Vector3M vector, Vector3M normal);

	/// <summary>
	/// Returns a vector containing the smallest components of the specified vectors.
	/// </summary>
	/// <param name="value1">The first source vector.</param>
	/// <param name="value2">The second source vector.</param>
	/// <returns>A vector containing the smallest components of the source vectors.</returns>
	static Vector3M Minimize(Vector3M value1, Vector3M value2);

	/// <summary>
	/// Returns a vector containing the largest components of the specified vectors.
	/// </summary>
	/// <param name="value1">The first source vector.</param>
	/// <param name="value2">The second source vector.</param>
	/// <returns>A vector containing the largest components of the source vectors.</returns>
	static Vector3M Maximize(Vector3M value1, Vector3M value2);

	/// <summary>
	/// Adds two vectors.
	/// </summary>
	/// <param name="left">The first vector to add.</param>
	/// <param name="right">The second vector to add.</param>
	/// <returns>The sum of the two vectors.</returns>
	friend Vector3M operator + (Vector3M const& left, Vector3M const& right);

	/// <summary>
	/// Subtracts two vectors.
	/// </summary>
	/// <param name="left">The first vector to subtract.</param>
	/// <param name="right">The second vector to subtract.</param>
	/// <returns>The difference of the two vectors.</returns>
	friend Vector3M operator - (Vector3M const& left, Vector3M const& right);

	/// <summary>
	/// Reverses the direction of a given vector.
	/// </summary>
	/// <param name="value">The vector to negate.</param>
	/// <returns>A vector facing in the opposite direction.</returns>
	friend Vector3M operator - (Vector3M const& value);

	/// <summary>
	/// Multiplies 2 vectors.
	/// </summary>
	/// <param name="left">Vector 1.</param>
	/// <param name="right">Vector 2.</param>
	/// <returns>The product of the 2 vectors.</returns>
	friend Vector3M operator * (Vector3M const& left, Vector3M const& right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3M operator * (Vector3M const& value, float const& scale);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3M operator * (float const& scale, Vector3M const& value);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3M operator *= (Vector3M& value, float const& scale);

	/// <summary>
	/// Divides one vector by another.
	/// </summary>
	/// <param name="left">Vector 1.</param>
	/// <param name="right">Vector 2.</param>
	/// <returns>The quotient of the 2 vectors.</returns>
	friend Vector3M operator / (Vector3M const& left, Vector3M const& right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3M operator / (Vector3M const& value, float const& scale);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3M operator /= (Vector3M& value, float const& scale);

	/// <summary>
	/// Tests for equality between two objects.
	/// </summary>
	/// <param name="left">The first value to compare.</param>
	/// <param name="right">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
	friend bool operator == (Vector3M const& left, Vector3M const& right);

	/// <summary>
	/// Tests for inequality between two objects.
	/// </summary>
	/// <param name="left">The first value to compare.</param>
	/// <param name="right">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
	friend bool operator != (Vector3M const& left, Vector3M const& right);


	/// <summary>
	/// Converts the value of the object to its equivalent string representation.
	/// </summary>
	/// <returns>The string representation of the value of this instance.</returns>
	std::string ToString() const;


	/// <summary>
	/// Returns a value that indicates whether the current instance is equal to the specified object. 
	/// </summary>
	/// <param name="other">Object to make the comparison with.</param>
	/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
	virtual bool Equals(const Vector3M& other) const;

	/// <summary>
	/// Returns a value that indicates whether the current instance is equal to the specified object. 
	/// </summary>
	/// <param name="other">Object to make the comparison with.</param>
	/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
	virtual bool Equals(const Vector3M_t& other) const;

	/// <summary>
	/// Determines whether the specified object instances are considered equal. 
	/// </summary>
	/// <param name="value1">The first value to compare.</param>
	/// <param name="value2">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="value1"/> contains the same values as <paramref name="value2"/>; 
	/// otherwise, <c>false</c>.</returns>
	static bool Equals(const Vector3M& value1, const Vector3M& value2);


	static float DistanceBetween(const Vector3M& value1, const Vector3M& value2);

	static Vector3M RotationToDirection(const Vector3M& rotation);
	static Vector3M DirectionToRotation(Vector3M direction);
	

};

class Vector2M
{
public:
	/// <summary>
	/// Gets or sets the X component of the vector.
	/// </summary>
	/// <value>The X component of the vector.</value>
	float x;

	/// <summary>
	/// Gets or sets the Y component of the vector.
	/// </summary>
	/// <value>The Y component of the vector.</value>
	float y;

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector2M"/> class.
	/// </summary>
	/// <param name="x">Initial value for the X component of the vector.</param>
	/// <param name="y">Initial value for the Y component of the vector.</param>
	Vector2M(float X, float Y);

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector2M"/> class.
	/// </summary>
	Vector2M();

	/// <summary>
	/// Nulls the vector (0,0)
	/// </summary>
	void clear();

	/// <summary>
	/// Returns a null vector. (0,0)
	/// </summary>
	static Vector2M Zero();

	/// <summary>
	/// Returns a vector with all components as '1'. (1,1)
	/// </summary>
	static Vector2M One();

	/// <summary>
	/// Returns the up vector. (0,1)
	/// </summary>
	static Vector2M Up();

	/// <summary>
	/// Returns the down vector. (0,-1)
	/// </summary>
	static Vector2M Down();

	/// <summary>
	/// Returns the right vector. (1,0)
	/// </summary>
	static Vector2M Right();

	/// <summary>
	/// Returns the left vector. (-1,0)
	/// </summary>
	static Vector2M Left();

	/// <summary>
	/// Calculates the length of the vector.
	/// </summary>
	/// <returns>The length of the vector.</returns>
	float Length() const;

	/// <summary>
	/// Gets a point on the circumference of a circle of the provided radius.
	/// </summary>
	/// <param name="radius">The radius of the circle to be made.</param>
	/// <param name="angleInDegrees">The angle (in degrees) that the new point will have from 0.</param>
	/// <returns>A point on the circumference of a circle of the provided radius.</returns>
	Vector2M PointOnCircle(float radius, float angleInDegrees) const;

	/// <summary>
	/// Creates a list of points around this position within the given radius.
	/// </summary>
	/// <param name="results">The output points. The collection is not cleared before adding in points.</param>
	/// <param name="fullRadius">The (boundry) radius of the circle to be made.</param>
	/// <param name="angleInDegrees">The difference in angles (in degrees) of each 2 points.</param>
	/// <param name="intervalDistance">The distance between 2 consecutive points.</param>
	/// <param name="includeCentre">Determines whether the origin should also be included in the list of points.</param>
	///// <returns>A list of points around this position within the given radius.</returns>
	void PointsOnCircle(std::vector<Vector2M>& results, float fullRadius, float angleInDegrees, float intervalDistance, bool includeCentre = false) const;


	/// <summary>
	/// Calculates the squared length of the vector.
	/// </summary>
	/// <returns>The squared length of the vector.</returns>
	float LengthSquared() const;

	/// <summary>
	/// Converts the vector into a unit vector.
	/// </summary>
	void Normalize();

	/// <summary>
	/// Calculates the distance between two vectors.
	/// </summary>
	/// <param name="position">The second vector to calculate the distance to.</param>
	/// <returns>The distance to the other vector.</returns>
	float DistanceTo(const Vector2M& position) const;

	/// <summary>
	/// Copies the vector coordinates into a float array.
	/// </summary>
	/// <returns>The vector as an std::array of floats.</returns>
	std::array<float,2> ToArray() const;

	/// <summary>
	/// Returns true if the vector is 0,0
	/// </summary>
	bool IsZero() const;

	/// <summary>
	/// Returns a new normalized vector with random X and Y components.
	/// </summary>
	static Vector2M RandomXY();

	/// <summary>
	/// Adds two vectors.
	/// </summary>
	/// <param name="left">The first vector to add.</param>
	/// <param name="right">The second vector to add.</param>
	/// <returns>The sum of the two vectors.</returns>
	static Vector2M Add(Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Subtracts two vectors.
	/// </summary>
	/// <param name="left">The first vector to subtract.</param>
	/// <param name="right">The second vector to subtract.</param>
	/// <returns>The difference of the two vectors.</returns>
	static Vector2M Subtract(Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	static Vector2M Multiply(Vector2M const& value, float const& scale);

	/// <summary>
	/// Modulates a vector by another.
	/// </summary>
	/// <param name="left">The first vector to modulate.</param>
	/// <param name="right">The second vector to modulate.</param>
	/// <returns>The modulated vector.</returns>
	static Vector2M Modulate(Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	static Vector2M Divide(Vector2M const& value, float const& scale);

	/// <summary>
	/// Reverses the direction of a given vector.
	/// </summary>
	/// <param name="value">The vector to negate.</param>
	/// <returns>A vector facing in the opposite direction.</returns>
	static Vector2M Negate(Vector2M const& value);

	/// <summary>
	/// Restricts a value to be within a specified range.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="min">The minimum value.</param>
	/// <param name="max">The maximum value.</param>
	/// <returns>The clamped value.</returns>
	static Vector2M Clamp(Vector2M const& value, Vector2M const& min, Vector2M const& max);

	/// <summary>
	/// Performs a linear interpolation between two vectors.
	/// </summary>
	/// <param name="start">Start vector.</param>
	/// <param name="end">End vector.</param>
	/// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
	/// <returns>The linear interpolation of the two vectors.</returns>
	/// <remarks>
	/// This method performs the linear interpolation based on the following formula.
	/// <code>start + (end - start) * amount</code>
	/// Passing <paramref name="amount"/> a value of 0 will cause <paramref name="start"/> to be returned; a value of 1 will cause <paramref name="end"/> to be returned. 
	/// </remarks>
	static Vector2M Lerp(Vector2M const& start, Vector2M const& end, float const& amount);

	/// <summary>
	/// Converts the vector into a unit vector.
	/// </summary>
	/// <param name="vector">The vector to normalize.</param>
	/// <returns>The normalized vector.</returns>
	static Vector2M Normalize(Vector2M value);

	/// <summary>
	/// Calculates the dot product of two vectors.
	/// </summary>
	/// <param name="left">First source vector.</param>
	/// <param name="right">Second source vector.</param>
	/// <returns>The dot product of the two vectors.</returns>
	static float Dot(Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Returns the reflection of a vector off a surface that has the specified normal. 
	/// </summary>
	/// <param name="vector">The source vector.</param>
	/// <param name="normal">Normal of the surface.</param>
	/// <returns>The reflected vector.</returns>
	/// <remarks>Reflect only gives the direction of a reflection off a surface, it does not determine 
	/// whether the original vector was close enough to the surface to hit it.</remarks>
	static Vector2M Reflect(Vector2M const& vector, Vector2M const& normal);

	/// <summary>
	/// Returns a vector containing the smallest components of the specified vectors.
	/// </summary>
	/// <param name="value1">The first source vector.</param>
	/// <param name="value2">The second source vector.</param>
	/// <returns>A vector containing the smallest components of the source vectors.</returns>
	static Vector2M Minimize(Vector2M const& value1, Vector2M const& value2);

	/// <summary>
	/// Returns a vector containing the largest components of the specified vectors.
	/// </summary>
	/// <param name="value1">The first source vector.</param>
	/// <param name="value2">The second source vector.</param>
	/// <returns>A vector containing the largest components of the source vectors.</returns>
	static Vector2M Maximize(Vector2M value1, Vector2M value2);

	/// <summary>
	/// Adds two vectors.
	/// </summary>
	/// <param name="left">The first vector to add.</param>
	/// <param name="right">The second vector to add.</param>
	/// <returns>The sum of the two vectors.</returns>
	friend Vector2M operator + (Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Subtracts two vectors.
	/// </summary>
	/// <param name="left">The first vector to subtract.</param>
	/// <param name="right">The second vector to subtract.</param>
	/// <returns>The difference of the two vectors.</returns>
	friend Vector2M operator - (Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Reverses the direction of a given vector.
	/// </summary>
	/// <param name="value">The vector to negate.</param>
	/// <returns>A vector facing in the opposite direction.</returns>
	friend Vector2M operator - (Vector2M const& value);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="vec">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector2M operator * (Vector2M const& vec, float const& scale);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="vec">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector2M operator * (float const& scale, Vector2M const& vec);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="vec">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector2M operator / (Vector2M const& vec, float const& scale);

	/// <summary>
	/// Tests for equality between two objects.
	/// </summary>
	/// <param name="left">The first value to compare.</param>
	/// <param name="right">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
	friend bool operator == (Vector2M const& left, Vector2M const& right);

	/// <summary>
	/// Tests for inequality between two objects.
	/// </summary>
	/// <param name="left">The first value to compare.</param>
	/// <param name="right">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
	friend bool operator != (Vector2M const& left, Vector2M const& right);


	/// <summary>
	/// Converts the value of the object to its equivalent string representation.
	/// </summary>
	/// <returns>The string representation of the value of this instance.</returns>
	std::string ToString() const;


	/// <summary>
	/// Returns a value that indicates whether the current instance is equal to the specified object. 
	/// </summary>
	/// <param name="other">Object to make the comparison with.</param>
	/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
	virtual bool Equals(const Vector2M& other) const;

	/// <summary>
	/// Determines whether the specified object instances are considered equal. 
	/// </summary>
	/// <param name="value1">The first value to compare.</param>
	/// <param name="value2">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="value1"/> has the same values as <paramref name="value2"/>;
	/// otherwise, <c>false</c>.</returns>
	static bool Equals(Vector2M value1, Vector2M value2);
};


int get_random_int_in_range(int min, int max);
float get_random_float_in_range(float min, float max);

float DegreeToRadian(float angle);
float RadianToDegree(float angle);

Vector3M DegreeToRadian(const Vector3M& angles);
float GetHeadingFromCoords(const Vector3M& source, const Vector3M& target);

#endif // VECTOR_EXTRA