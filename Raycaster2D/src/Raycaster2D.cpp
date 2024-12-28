#include "Raycaster2D.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace Core {

	void HelloSFML()
	{
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
	 * @param[in] windowSize The size of the sf::RenderWindow.
	 */
	Raycaster2D::Raycaster2D(sf::VideoMode windowSize) : runApp(true) {
		window = std::make_shared<sf::RenderWindow>(windowSize, "RaycastingApp");
		window->setVerticalSyncEnabled(true);

		shapes.reserve(5);
		shapes.emplace_back(Core::RandomConvexShape({ 200.f, 200.f }, { 350, 150 }, 6));
		shapes.emplace_back(Core::RandomConvexShape({ 600.f, 230.f }, { 100, 250 }, 6));
		shapes.emplace_back(Core::RandomConvexShape({ 400.f, 300.f }, { 180, 120 }, 6));
		shapes.emplace_back(Core::RandomConvexShape({ 230.f, 500.f }, { 350, 350 }, 6));
		shapes.emplace_back(Core::RandomConvexShape({ 550.f, 600.f }, { 100, 100 }, 6));
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
			window->display();
		} // while ( window.isOpen() )
	}

} // namespace Core