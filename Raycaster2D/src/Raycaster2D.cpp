#include <iostream>
#include <ranges>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Raycaster2D.hpp"

namespace Core {

	void HelloSFML() {
		sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
		sf::CircleShape shape(100.f);
		shape.setFillColor(sf::Color::Green);

		while ( window.isOpen() ) {
			while ( const std::optional event = window.pollEvent() ) {
				if ( event->is<sf::Event::Closed>() )
					window.close();
			}

			window.clear();
			window.draw(shape);
			window.display();
		}
	}

	// ****************************************************************************************************************** //
	//                                                                                                                    //
	//                                                      CLASSES                                                       //
	//                                                                                                                    //
	// ****************************************************************************************************************** //

		/**
		 * @brief Constructor.
		 * @author Alucat1986
		 * @date 27.12.2024
		 */
	Raycaster2D::Raycaster2D() : runApp(true) {
		window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 800, 800 }), "RaycastingApp");
		window->setVerticalSyncEnabled(true);

		shapes.reserve(5);
		std::cout << "== V1: ==\n";
		shapes.emplace_back(Core::RandomConvexShape({ 50.f, 50.f }, { 350, 150 }, 6));
		std::cout << "== V2: ==\n";
		shapes.emplace_back(Core::RandomConvexShape({ 600.f, 100.f }, { 100, 250 }, 3));
		std::cout << "== V3: ==\n";
		shapes.emplace_back(Core::RandomConvexShape({ 150.f, 300.f }, { 180, 120 }, 5));
		std::cout << "== V4: ==\n";
		shapes.emplace_back(Core::RandomConvexShape({ 450.f, 400.f }, { 350, 350 }, 4));
		std::cout << "== V5: ==\n";
		shapes.emplace_back(Core::RandomConvexShape({ 200.f, 600.f }, { 200, 100 }, 8));

		preparePoints();
	}

	/**
	 * @brief Main loop for the Raycaster.
	 * @author Alucat1986
	 * @date 27.12.2024
	 */
	void Raycaster2D::run() const {
		while ( window->isOpen() ) {
			while ( const std::optional event = window->pollEvent() ) {
				if ( event->is<sf::Event::Closed>() )
					window->close();
			} // while ( const std::optional event = window.pollEvent() )

			window->clear();
			for ( const auto& shape : shapes ) {
				window->draw(shape);
			} // for ( const auto& shape : shapes )
			for ( const auto& point : points ) {
				window->draw(point);
			} // for ( const auto& point : points )
			window->display();
		} // while ( window.isOpen() )
	}

	/**
	 * @brief Useful for Debug to see where the points of the shape actually are.
	 * @author Alucat1986
	 * @date 29.12.2024
	 */
	void Raycaster2D::preparePoints() {
		for ( const auto& shape : shapes ) {
			for ( const auto& index : std::ranges::views::iota(0u, shape.getPointCount()) ) {
				points.emplace_back(2.f);
				sf::Vector2f position = shape.getPoint(index);
				position.x += shape.getPosition().x;
				position.y += shape.getPosition().y;
				points.back().setPosition(position);
			} // for ( const auto& index : std::ranges::views::iota(0, shape.getPointCount()) )
		} // for ( const auto& shape : shapes )
	}

} // namespace Core