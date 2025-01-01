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
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "RandomConvexShape.hpp"

namespace Core {

	void HelloSFML();

	class Raycaster2D {
	public:
		Raycaster2D();
		void run();

	private:
		void handleUserInput();
		void update(float deltaTime);
		void render(float deltaTime);

		std::vector<float> handleCollisions();
		sf::Vector2f intersect(const sf::Vertex& pointA, const sf::Vertex& pointB, const Core::RandomConvexShape& shape) const;
		sf::Vector2f intersectWithWindowBorder(const sf::Vertex& pointA, const sf::Vector2f& rayDirection) const;

		void preparePoints();
		void prepareRays();
		void moveRays(const std::vector<float>& scalars);
		void drawRays() const;

	private:
		bool Running;
		bool Paused;
		sf::Font FpsFont;
		std::shared_ptr<sf::Text> FpsText;
		std::shared_ptr<sf::Text> PauseText;

		std::shared_ptr<sf::RenderWindow> window;
		std::vector<Core::RandomConvexShape> shapes;
		std::vector<sf::CircleShape> points;
		
		sf::VertexArray Rays;
		sf::Vector2f MousePreviousPosition;
	};

}

#endif