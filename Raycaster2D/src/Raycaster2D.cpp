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
				if ( event->is<sf::Event::Closed>() ) {
					window.close();
					break;
				} // if ( event->is<sf::Event::Closed>() )
			} // while ( const std::optional event = window.pollEvent() )

			window.clear();
			window.draw(shape);
			window.display();
		} // while ( window.isOpen() )
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
	Raycaster2D::Raycaster2D() : Running(true), Paused(false) {
		window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 800, 800 }), "RaycastingApp");
		window->setVerticalSyncEnabled(true);

		mousePosition = { 0.f, 0.f };
		mousePoint.setRadius(2.f);
		mousePoint.setFillColor(sf::Color::Red);
		mousePoint.setPosition(mousePosition);

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
	void Raycaster2D::run() {
		while ( window->isOpen() ) {
			update();

			window->clear();

			for ( const auto& shape : shapes ) {
				window->draw(shape);
			} // for ( const auto& shape : shapes )

			for ( const auto& point : points ) {
				window->draw(point);
			} // for ( const auto& point : points )

			window->draw(mousePoint);

			window->display();
		} // while ( window.isOpen() )
	}

	/**
	 * @brief Checks if the Raycaster is running.
	 * @author Alucat1986
	 * @date 30.12.2024
	 * @return true if the Raycaster is running, false otherwise.
	 */
	bool Raycaster2D::isRunning() const {
		return Running;
	}

	/**
	 * @brief Updates the Raycaster "every frame".
	 * @author Alucat1986
	 * @date 30.12.2024
	 */
	void Raycaster2D::update() {
		handleUserInput();

		if ( !Paused ) {
			mousePoint.setPosition(mousePosition);
		} // if ( !Paused )
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

	/**
	 * @brief Handles the user's input and updates the mouse position accordingly.
	 * @author Alucat1986
	 * @date 30.12.2024
	 */
	void Raycaster2D::handleUserInput() {
		while ( const std::optional event = window->pollEvent() ) {
			if ( event->is<sf::Event::Closed>() ||
				 event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape ) {
				window->close();
				break;
			} // if ( event->is<sf::Event::Closed>() )

			if ( event->is<sf::Event::MouseEntered>() || event->is<sf::Event::MouseMoved>() ) {
				
				if ( const auto* position = event->getIf<sf::Event::MouseMoved>() ) {
					mousePosition = { static_cast<float>(position->position.x), static_cast<float>(position->position.y) };
				} // if ( const auto* position = event->getIf<sf::Event::MouseMoved>() )
			} // if ( event->is<sf::Event::MouseEntered>() || event->is<sf::Event::MouseMoved() )
		} // while ( const std::optional event = window.pollEvent() )
	}

} // namespace Core