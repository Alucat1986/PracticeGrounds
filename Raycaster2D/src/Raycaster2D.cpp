#include <iostream>
#include <map>
#include <numbers>
#include <numeric>
#include <ranges>

#include <SFML/System/Clock.hpp>

#include "Raycaster2D.hpp"



namespace Core {
	/**
	 * @brief Struct for an intersection point.
	 * @author Alucat1986
	 * @date 02.01.2025
	 */
	struct IntersectionPoint {
		float Scalar;
		sf::Vector2f Point;
	};

	/**
	 * @brief A little test function to see if a window with a green circle is displayed correctly using SFML.
	 *
	 * @depracated Might be removed in the future.
	 */
	[[deprecated("Might be removed in the future. Do not use")]]
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
		window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 800, 800 }), "RaycastingApp");
		window->setVerticalSyncEnabled(true);
		//window->setFramerateLimit(60);

		MousePreviousPosition = { 50.f, 50.f };

		if ( !FpsFont.openFromFile("C:/Windows/Fonts/arial.ttf") ) {
			std::cerr << "Could not load font from file!\n";
		} // if ( !FpsFont.loadFromFile("C:/Windows/Fonts/arial.ttf") )

		FpsText = std::make_unique<sf::Text>(FpsFont, "FPS: 0", 18);
		FpsText->setFillColor(sf::Color::White);
		FpsText->setPosition({ 10.f, 10.f });

		PauseText = std::make_unique<sf::Text>(FpsFont, "Game Paused\nPress 'R' to Resume");
		PauseText->setFillColor(sf::Color::Yellow);
		PauseText->setPosition({ 200.f, 200.f });

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
		prepareRays();
	}

	/**
	 * @brief Main loop for the Raycaster.
	 * @author Alucat1986
	 * @date 27.12.2024
	 */
	void Raycaster2D::run() {
		sf::Clock clock;
		while ( window->isOpen() ) {
			sf::Time deltaTime = clock.restart();
			handleUserInput();

			if ( !Paused ) {
				update(deltaTime.asSeconds());
			} // if ( !Paused )

			render(deltaTime.asSeconds());
		} // while ( window.isOpen() )
	}

	/**
	 * @brief Handles the user's input and updates the mouse position accordingly.
	 * @author Alucat1986
	 * @date 30.12.2024
	 */
	void Raycaster2D::handleUserInput() {
		while ( const std::optional event = window->pollEvent() ) {
			if ( event->is<sf::Event::Closed>() ) {
				window->close();
				Running = false;
				break;
			} // if ( event->is<sf::Event::Closed>() )

			if ( event->is<sf::Event::KeyPressed>() ) {
				if ( event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape ) {
					window->close();
					Running = false;
					break;
				} // if ( event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape )
				if ( event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::R ) {
					Paused = !Paused;
				} // if ( event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::R )
			} // if ( event->is<sf::Event::KeyPressed>() )

			if ( event->is<sf::Event::MouseEntered>() || event->is<sf::Event::MouseMoved>() ) {
				if ( const auto* position = event->getIf<sf::Event::MouseMoved>() ) {
					MousePreviousPosition = Rays[0].position;
					Rays[0].position = { static_cast<float>(position->position.x), static_cast<float>(position->position.y) };
				} // if ( const auto* position = event->getIf<sf::Event::MouseMoved>() )
			} // if ( event->is<sf::Event::MouseEntered>() || event->is<sf::Event::MouseMoved() )
		} // while ( const std::optional event = window.pollEvent() )
	}

	/**
	 * @brief Updates the Raycaster "every frame".
	 * @author Alucat1986
	 * @date 30.12.2024
	 */
	void Raycaster2D::update(float deltaTime) {
		if ( Rays[0].position != MousePreviousPosition ) {
			std::vector<float> scalars = handleCollisions();
			//std::vector<float> scalars = { 0.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f };
			moveRays(scalars);
			MousePreviousPosition = Rays[0].position;
		} // if ( Rays[0].position != MousePreviousPosition )
	}

	/**
	 * @brief Renders the scene onto the screen.
	 * @author Alucat1986
	 * @date 31.12.2024
	 */
	void Raycaster2D::render(float deltaTime) {
		static float fpsTimer = 0.f;
		static int frameCount = 0;

		fpsTimer += deltaTime;
		frameCount++;

		if ( fpsTimer >= 1.f ) {
			float fps = frameCount / fpsTimer;
			FpsText->setString("FPS: " + std::to_string(static_cast<int>(fps)));
			fpsTimer = 0.f;
			frameCount = 0;
		} // if ( fpsTimer >= 1.f )


		window->clear();

		if ( Paused ) {
			window->draw(*PauseText);
		} // if ( Paused )

		for ( const auto& shape : shapes ) {
			window->draw(shape);
		} // for ( const auto& shape : shapes )

		for ( const auto& point : points ) {
			window->draw(point);
		} // for ( const auto& point : points )

		drawRays();
		window->draw(*FpsText);

		window->display();
	}

	/**
	 * @brief Handles the collisions of the rays with the shapes in the scene.
	 * @author Alucat1986
	 * @date 31.12.2024
	 */
	std::vector<float> Raycaster2D::handleCollisions() {
		std::vector<float> result;
		result.emplace_back(0.f);

		for ( std::size_t vIndex = 1; vIndex < Rays.getVertexCount(); vIndex++ ) {
			for ( const auto& shape : shapes ) {
				const sf::Vector2f intersectionPoint = intersect(Rays[0], Rays[vIndex], shape);
				Rays[vIndex].position = intersectionPoint;
				sf::Vector2f direction = intersectionPoint - Rays[0].position;
				if ( vIndex >= result.size() ) {
					result.push_back(direction.length());
					continue;
				} // if ( vIndex > result.size() )
				else if ( result[vIndex - 1] > direction.length() ) {
					result[vIndex - 1] = direction.length();
				} // else
			} // for ( const auto& shape : shapes )
		} // for ( std::size_t vIndex = 1; vIndex < Rays.getVertexCount(); vIndex++ )
		return result;
	}

	/**
	 * @brief Calculates the intersection point of a line and a shape or the World Border.
	 * @author Alucat1986
	 * @date 02.01.2025
	 * @param[in] pointA The starting point of the ray.
	 * @param[in] pointB The end point of the ray.
	 * @param[in] shape The shape to calculate the intersection point for.
	 * @return The intersection point if it exists, either one with the shape or with the window border.
	 */
	sf::Vector2f Raycaster2D::intersect(const sf::Vertex& pointA, const sf::Vertex& pointB, const Core::RandomConvexShape& shape) const {
		sf::Vector2f rayDirection = pointB.position - pointA.position;
		std::size_t countOfPoints = shape.getPointCount();

		std::map<std::size_t, sf::Vector2f> intersections;

		const sf::FloatRect windowBounds({ 0.f, 0.f }, { static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) });
		std::array<sf::Vector2f, 4> borderSegments = {
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(windowBounds.size.x, 0.f),
			sf::Vector2f(0.f, windowBounds.size.y),
			sf::Vector2f(windowBounds.size.x, windowBounds.size.y)
		};

		// Intersection Points with the Window Border.
		for ( std::size_t borIndex = 0; borIndex < borderSegments.size(); borIndex++ ) {
			sf::Vector2f borderDirection = borderSegments[(borIndex + 1) % borderSegments.size()] - borderSegments[borIndex];
			IntersectionPoint intersectPoint = calculateIntersectionPoint({ pointA.position.x, pointA.position.y },
																		    rayDirection, borderSegments[borIndex],
																		    borderDirection);

			if ( intersectPoint.Scalar != -1.f ) {
				intersections.emplace(intersectPoint.Scalar, intersectPoint.Point);
			} // if ( intersectPoint.Scalar != -1.f )
		} // for ( std::size_t borIndex = 0; borIndex < borderSegments.size(); borIndex++ )

		//std::size_t segIndex = 0; // huh??? Inside the for it doesn't work but here?
		// std::size_t segIndex = 0; => segIndex == 0;
		// for ( std::size_t segIndex = 0; segIndex < countOfPoints; segIndex++ ) => segIndex == 1412923942927392472 (very long number)
		// Intersection Points with the Shape.
		for ( std::size_t segIndex = 0; segIndex < countOfPoints; segIndex++ ) {
			sf::Vector2f segStart = shape.getPoint(segIndex);
			sf::Vector2f segDirection = shape.getPoint((segIndex + 1) % countOfPoints) - segStart;

			IntersectionPoint intersectPoint = calculateIntersectionPoint({ pointA.position.x, pointA.position.y },
																			rayDirection, segStart, segDirection);
			if ( intersectPoint.Scalar != -1.f ) {
				intersections.emplace(intersectPoint.Scalar, intersectPoint.Point);
			} // if ( intersectPoint.Scalar != -1.f )
		} // for ( std::size_t segIndex = 0; segIndex < shape.getPointCount; segIndex++ )

		auto minElement = std::ranges::min_element(intersections, [](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first; });

		if ( minElement != intersections.end() ) {
			return minElement->second;
		} // if ( minElement != intersections.end() )
		else {
			return { -1.f, -1.f };
		} // else
	}

	/**
	 * @brief Calculates the intersection point of a ray and a segment of a shape.
	 * @author Alucat1986
	 * @date 02.01.2025
	 * @param[in] pointA The origin of the ray.
	 * @param[in] rayDirection The direction of the ray.
	 * @param[in] segStart The origin of the segment.
	 * @param[in] segDirection The direction of the segment.
	 * @return The intersection point if it exists, otherwise an intersection point with all values == -1.f.
	 */
	IntersectionPoint Raycaster2D::calculateIntersectionPoint(const sf::Vector2f&	pointA,
															  const sf::Vector2f&	rayDirection,
															  const sf::Vector2f&	segStart,
															  const sf::Vector2f&	segDirection) const {
		// Solve the equation system, after https://paulbourke.net/geometry/pointlineplane/
		// Pa = pointA + t * (pointB - pointA)
		// Pb = segStart + s * (segEnd - segStart)
		// Pa = Pb =>
		// pointA.x + t * rayDirection.x = segStart.x + s * segDirection.x
		// pointA.y + t * rayDirection.y = segStart.y + s * segDirection.y
		// =>
		// t = (segDirection.x * (pointA.y - segStart.y) - (segDirection.y * (pointA.x - segStart.x))) /
		//	   (segDirection.y * rayDirection.x - segDirection.x * rayDirection.y)
		// s = (rayDirection.x * (pointA.y - segStart.y) - (segDirection.y * (pointA.x - segStart.x))) /
		//	   (segDirection.y * rayDirection.x - segDirection.x * rayDirection.y)
		// =>
		// Intersection.x = pointA.x + t * rayDirection.x
		// Intersection.y = pointA.y + t * rayDirection.y

		float denominator = segDirection.y * rayDirection.x - segDirection.x * rayDirection.y;
		if ( denominator != 0 ) {
			// Calculate the intersection point
			float numeratorT = segDirection.x * (pointA.y - segStart.y) - (segDirection.y * (pointA.x - segStart.x));
			float numeratorS = rayDirection.x * (pointA.y - segStart.y) - (segDirection.y * (pointA.x - segStart.x));

			float t = numeratorT / denominator;
			float s = numeratorS / denominator;

			if ( (t >= 0 && t <= 1) && (s >= 0 && s <= 1) ) {
				return { t, { pointA.x + t * rayDirection.x, pointA.y + t * rayDirection.y} };
			} // if ( (t >= 0 && t <= 1) && (s >= 0 && s <= 1) )
		} // if ( denominator != 0 )
		else {
			return { -1.f, { -1.f, -1.f } };
		} // else if ( denominator == 0 )
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
				points.back().setOrigin({ 1.f, 1.f });
			} // for ( const auto& index : std::ranges::views::iota(0, shape.getPointCount()) )
		} // for ( const auto& shape : shapes )
	}

	/**
	 * @brief Prepares the vector of rays for casting them out onto the field and for later intersection.
	 * @author Alucat1986
	 * @date 30.12.2024
	 */
	void Raycaster2D::prepareRays() {
		const std::size_t countOfRays = 8;
		Rays.setPrimitiveType(sf::PrimitiveType::Lines);
		Rays.resize(countOfRays + 1);
		Rays[0].position = MousePreviousPosition;

		for ( std::size_t pos = 0; pos <= countOfRays; pos++ ) {
			Rays[pos].color = sf::Color::Red;
		} // for ( std::size_t pos = 0; pos <= countOfRays; pos++ )
		moveRays({ 0.f, 1'500.f, 1'500.f, 1'500.f, 1'500.f, 1'500.f, 1'500.f, 1'500.f, 1'500.f });
	}

	/**
	 * @brief Moves the rays by the given difference vector.
	 * @author Alucat1986
	 * @date 30.12.2024
	 * @param[in] scalars The scalars of each ray to determine it's length.
	 */
	void Raycaster2D::moveRays(const std::vector<float>& scalars) {
		const double angleStep = 2 * std::numbers::pi / (Rays.getVertexCount() - 1);
		// Equally spaced rays in x directions
		for ( std::size_t pos = 1; pos < Rays.getVertexCount(); pos++ ) {
			Rays[pos].position = Rays[0].position;
			Rays[pos].position.x += scalars[pos] * static_cast<float>(std::cos(pos * angleStep));
			Rays[pos].position.y += scalars[pos] * static_cast<float>(std::sin(pos * angleStep));
		} // for ( std::int8_t pos = 1; pos < countOfRays; pos++ )
	}

	/**
	 * @brief Draws the rays onto the window for visualization purposes. The first vertex is the origin of the rays, the following vertices re the end points of the rays.
	 * @author Alucat1986
	 * @date 30.12.2024
	 */
	void Raycaster2D::drawRays() const {
		for ( std::size_t pos = 1; pos < Rays.getVertexCount(); pos++ ) {
			sf::Vertex line[]{ {Rays[0]}, {Rays[pos]} };
			window->draw(line, 2, sf::PrimitiveType::Lines);
		} // for ( std::size_t pos = 1; pos < Rays.size(); pos++ )
	}

} // namespace Core