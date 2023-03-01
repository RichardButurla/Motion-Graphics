#include "Vector2.h"

//Name: Richard Buturla
constexpr double PI = 3.1415926535;


/// <summary>
/// get length of vector using sqrt of the sum of the squares
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length</returns>
float vectorLength(const sf::Vector2f t_vectorA)
{
	float sumOfSquares = (t_vectorA.x * t_vectorA.x) + (t_vectorA.y * t_vectorA.y);
	const float length = std::sqrt(sumOfSquares);
	return length;
}
/// <summary>
/// gets length squared of a vector using vectoe length
/// </summary>
/// <param name="t_vectorA"> input vector</param>
/// <returns>length squared</returns>
float vectorLengthSquared(const sf::Vector2f t_vectorA)
{
	const float lengthSquared = (t_vectorA.x * t_vectorA.x) + (t_vectorA.y * t_vectorA.y);
	return lengthSquared;
}
/// <summary>
/// gets the cross product of 2 vectors
/// </summary>
/// <param name="t_vectorA"> first input vector</param>
/// <param name="t_VectorB"> second input vector</param>
/// <returns> Cross product</returns>
float vectorCrossProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	const float crossProduct = (t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x);
	return crossProduct;
}
/// <summary>
/// gets the dot product of 2 vectors
/// </summary>
/// <param name="t_vectorA"> input vector 1</param>
/// <param name="t_VectorB"> input vector 2</param>
/// <returns> dotProduct</returns>
float vectorDotProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	const float dotProduct = (t_vectorA.x * t_VectorB.x) + (t_vectorA.y * t_VectorB.y);
	return dotProduct;
}
/// <summary>
/// Finds the angle between 2 vectors
/// </summary>
/// <param name="t_vectorA"> input vector 1</param>
/// <param name="t_VectorB"> input vector 2</param>
/// <returns> angle between in degrees</returns>
float vectorAngleBetween(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	float theta = (vectorDotProduct(t_vectorA, t_VectorB)) / (vectorLength(t_vectorA) * (vectorLength(t_VectorB)));
	const float angleBetween = (acos(theta)) * (180 / PI); // multiply by 180/PI to get degrees. calculations are in radians.
	return angleBetween;
}
/// <summary>
/// Rotates a vector with radians
/// </summary>
/// <param name="t_vectorA"> input vector</param>
/// <param name="t_angleRadians"> angle in radians</param>
/// <returns> the rotated vector</returns>
sf::Vector2f vectorRotateBy(const sf::Vector2f t_vectorA, const  float t_angleRadians)
{
	sf::Vector2f rotatedVector;
	rotatedVector.x = ((t_vectorA.x * cos(t_angleRadians)) - (t_vectorA.y * sin(t_angleRadians)));
	rotatedVector.y = ((t_vectorA.x * sin(t_angleRadians)) + (t_vectorA.y * cos(t_angleRadians)));
	return rotatedVector;
}

/// <summary>
/// Projects one vector onto another vector
/// </summary>
/// <param name="t_vectorA"> input vector that is projected</param>
/// <param name="t_onto"> input vector that is projected onto</param>
/// <returns> vector resolute of vector A in the direction of vector B</returns>
sf::Vector2f vectorProjection(const sf::Vector2f t_vectorA, const  sf::Vector2f t_onto)
{
	sf::Vector2f answer;
	answer.x = (((vectorDotProduct(t_vectorA, t_onto)) / vectorLength(t_onto))) * (t_onto.x / vectorLength(t_onto));
	answer.y = (((vectorDotProduct(t_vectorA, t_onto)) / vectorLength(t_onto))) * (t_onto.y / vectorLength(t_onto));
	return answer;
}
/// <summary>
/// Gets the orthogonal projection of vector a onto the plane orthogonal to b
/// </summary>
/// <param name="t_vectorA"> input vector that is rejected</param>
/// <param name="t_onto"> input vector that is rejected from </param>
/// <returns> vector resolute of vector A perpendicular to vector B </returns>
sf::Vector2f vectorRejection(const sf::Vector2f t_vectorA, const  sf::Vector2f t_onto)
{
	sf::Vector2f answer;
	answer = t_vectorA - vectorProjection(t_vectorA, t_onto);
	return answer;
}
/// <summary>
/// Gets the length of the vector Projection
/// </summary>
/// <param name="t_vector"> input vector that is projected</param>
/// <param name="t_onto"> input vector that is projected onto</param>
/// <returns> length</returns>
float vectorScalarProjection(const sf::Vector2f t_vector, const  sf::Vector2f t_onto)
{
	const float length = (vectorDotProduct(t_vector, t_onto)) / (vectorLength(t_onto));
	return length;
}

/// <summary>
/// Gets the unit vector of a vector
/// </summary>
/// <param name="t_vectorA">input vector</param>
/// <returns> the unit vector </returns>
sf::Vector2f vectorUnitVector(const sf::Vector2f t_vectorA)
{
	sf::Vector2f answer;
	answer.x = t_vectorA.x / vectorLength(t_vectorA);
	answer.y = t_vectorA.y / vectorLength(t_vectorA);
	return answer;
}