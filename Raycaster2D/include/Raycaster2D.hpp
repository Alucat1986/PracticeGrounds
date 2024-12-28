#ifndef RAYCASTER2D_HPP
#define RAYCASTER2D_HPP

#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "RandomConvexShape.hpp"

namespace Core {

	void HelloSFML();

	class Raycaster2D {
	public:
		Raycaster2D(sf::VideoMode windowSize);
		void run() const;

	private:
		bool runApp;
		std::shared_ptr<sf::RenderWindow> window;
		std::vector<Core::RandomConvexShape> shapes;
	};

}

#endif