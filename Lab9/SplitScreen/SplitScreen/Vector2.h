#pragma once
// author Richard Buturla
#ifndef MY_VECTOR2_HPP
#define MY_VECTOR2_HPP

#include <SFML/Graphics.hpp>
#include "Globals.h"



float vectorLength(const sf::Vector2f t_vectorA);  // root x2 + y2
float vectorLengthSquared(const sf::Vector2f t_vectorA); // x2 + y2
sf::Vector2f vectorUnitVector(const sf::Vector2f t_vectorA);// length of ans is one, null in null out
float vectorCrossProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB);  // Vx * Uy - Vy * Ux
// don't argue that the cross product doesn't exist in 2 dimensions
float vectorDotProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB);  // Vx * Uy + Vy * Ux
float vectorAngleBetween(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB); // result always 0>= && <=180
sf::Vector2f vectorRotateBy(const sf::Vector2f t_vectorA, const float t_angleRadians); // anti-clockwise ({1,0},PI/2) = {0,1}
sf::Vector2f vectorProjection(const sf::Vector2f t_vectorA, const sf::Vector2f t_onto);// ans parallel to second vector
sf::Vector2f vectorRejection(const sf::Vector2f t_vectorA, const sf::Vector2f t_onto);// ans perpendicular to second vector
float vectorScalarProjection(const sf::Vector2f t_vectorA, const sf::Vector2f t_onto);// scalar resolute



#endif  // MY_VECTOR2_HPP

