/**
 * @file
 * @brief Contains the declaration of the RandomConvexShape class.
 * @author Alucat1986
 * @date 07.05.2024
*/

#ifndef RANDOMCONVEXSHAPE_HPP
#define RANDOMCONVEXSHAPE_HPP

#include <SFML/Graphics.hpp>

namespace Core {

    class RandomConvexShape : public sf::ConvexShape {
    public:
        RandomConvexShape(sf::Vector2f position, sf::Vector2i boundingBoxSize, const std::size_t& countOfPoints);

    private:
        void sortPoints(std::vector<sf::Vector2f>& pointsToSort);
        float angleBetweenTwoPoints(const sf::Vector2f& PointOne, const sf::Vector2f& PointTwo) const;
    };

} // namespace

#endif