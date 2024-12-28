/**
 * @file
 * @brief Contains the implementation of the Application class.
 * @author Alucat1986
 * @date 27.12.2024
 */

#include "Application.hpp"
#include "include/Raycaster2D.hpp"

 // ****************************************************************************************************************** //
 //                                                                                                                    //
 //                                                      CLASSES                                                       //
 //                                                                                                                    //
 // ****************************************************************************************************************** //

Application::Application() {

}

void Application::run() {
	//Core::HelloSFML();
	Core::Raycaster2D raycaster({ 800, 800 });
	raycaster.run();
}