/**
 * @file
 * @brief Contains the declaration of the Raycaster2D class.
 * @author Alucat1986
 * @date 30.12.2024
 */

#ifndef RAYCASTER2D_HPP
#define RAYCASTER2D_HPP

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "RandomConvexShape.hpp"

namespace Core {

	void HelloSFML();

	class Raycaster2D {
	public:
		Raycaster2D();
		void run();
		bool isRunning() const;
		void update();

	private:
		void preparePoints();
		void prepareRays();
		void handleUserInput();
		void moveRays(const sf::Vector2f& differenceVector);
		void drawRays() const;

	private:
		bool Running;
		bool Paused;
		std::shared_ptr<sf::RenderWindow> window;
		std::vector<Core::RandomConvexShape> shapes;
		std::vector<sf::CircleShape> points;
		sf::VertexArray Rays;
		sf::Vector2f MousePreviousPosition;
	};

}

#endif