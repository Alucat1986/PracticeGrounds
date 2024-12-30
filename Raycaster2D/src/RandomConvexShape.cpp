/**
 * @file
 * @brief Contains the implementation of the RandomConvexShape class.
 * @author Alucat1986
 * @date 26.12.2024
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <numbers>
#include <iostream>
#include <random>

#include "RandomConvexShape.hpp"

 // ****************************************************************************************************************** //
 //                                                                                                                    //
 //                                                     FUNCTIONS                                                      //
 //                                                                                                                    //
 // ****************************************************************************************************************** //

static int quadrantOfPoint(const sf::Vector2f& point, const sf::Vector2f& origin) {
	if ( point.x > origin.x && point.y > origin.y ) return 1;
	else if ( point.x < origin.x && point.y > origin.y ) return 2;
	else if ( point.x < origin.x && point.y < origin.y ) return 3;
	return 4;
}

static sf::Vector2f calculateCentroid(const std::vector<sf::Vector2f>& points) {
	float sumX = 0, sumY = 0;
	for ( const auto& p : points ) {
		sumX += p.x;
		sumY += p.y;
	} // for ( const auto& p : points )
	return { sumX / points.size(), sumY / points.size() };
}

// ****************************************************************************************************************** //
//                                                                                                                    //
//                                                      CLASSES                                                       //
//                                                                                                                    //
// ****************************************************************************************************************** //

namespace Core {

	/**
	 * @brief Constructor.
	 * @author Alucat1986
	 * @date 07.05.2024
	 * @param[in] position The position of the shape.
	 * @param[in] boundingBoxSize The size of the boundingBox in which the random points will be generated.
	 * @param[in] numberOfPoints The count of Points that are to be generated for this shape.
	 *
	 * The constructor generates a random shape, based upon the given arguments. Due to restrictions from the SFML library
	 * the points have to be sorted in a way that they are either clock or counterclockwise ordered, otherwise it may not draw the shape correctly.
	 */
	RandomConvexShape::RandomConvexShape(sf::Vector2f position, sf::Vector2i boundingBoxSize, const std::size_t& countOfPoints) {
		std::random_device random;
		std::mt19937::result_type seed = random() ^ (
			static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::system_clock::now().time_since_epoch()).count()) +
			static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count()));
		std::mt19937 randomGenerator(seed);

		std::vector<sf::Vector2f> points;
		setPosition(position);
		setPointCount(countOfPoints);

		for ( unsigned char i = 0; i < countOfPoints; i++ ) {
			sf::Vector2f randomPoint;

			randomPoint.x = static_cast<float>(randomGenerator() % boundingBoxSize.x);
			randomPoint.y = static_cast<float>(randomGenerator() % boundingBoxSize.y);
			points.push_back(randomPoint);
			std::cout << "Random Point(" << randomPoint.x << "/" << randomPoint.y << ")\n";
		} // for ( unsigned char i = 0; i < countOfPoints; i++ )

		sortPoints(points);

		for ( unsigned char i = 0; i < countOfPoints; i++ ) {
			setPoint(i, points.at(i));
		} // for ( unsigned char i = 0; i < countOfPoints; i++ )

		setFillColor(sf::Color::Cyan);
		setOutlineColor(sf::Color::Red);
		setOutlineThickness(1);
		return;
	}

	/**
	 * @brief Sorts the given vector of point to get them into a clockwise order.
	 * @author Alucat1986
	 * @date 07.05.2024
	 * @param[in,out] points The vector of points that needs sorting.
	 */
	void RandomConvexShape::sortPoints(std::vector<sf::Vector2f>& points) {
		sf::Vector2f originPoint = calculateCentroid(points);

		auto angleComparator = [&originPoint](const sf::Vector2f& a, const sf::Vector2f& b) {
			float angleA = std::atan2(a.y - originPoint.y, a.x - originPoint.x);
			float angleB = std::atan2(b.y - originPoint.y, b.x - originPoint.x);
			return angleA < angleB;
			};

		std::sort(points.begin(), points.end(), angleComparator);
	}

	/**
	 * @brief Calculates the angle between two given points.
	 * @author Alucat1986
	 * @date 07.05.2024
	 * @param[in] pointOne First point.
	 * @param[in] pointTwo Second point.
	 * @return The angle in degrees
	 */
	float RandomConvexShape::angleBetweenTwoPoints(const sf::Vector2f& PointOne, const sf::Vector2f& PointTwo) const {
		return static_cast<float>((std::atan2(PointOne.y - PointTwo.y, PointOne.x - PointTwo.x) * 180 /
								   std::numbers::pi) + 180);
	}

} // namespace