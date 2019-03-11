#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "mandelbrot.h"

int winSizeX = 1920/2, winSizeY = 1080/2;



int main()
{
	sf::RenderWindow window(sf::VideoMode(winSizeX,winSizeY), "MANDELBROT      Luca Ciucci");

	Mandelbrot<> mandelbrot(window); // plotter

	sf::Event event;

	mandelbrot.display();

	while (window.isOpen())
	{
		sf::Vertex line[100];

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
			{
				//std::cout << "+";
			}
			mandelbrot.zoomIn();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
			{
				//std::cout << "-";
			}
			mandelbrot.zoomOut();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
			}
			mandelbrot.panUp();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
			}
			mandelbrot.panDown();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
			}
			mandelbrot.panRight();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
			}
			mandelbrot.panLeft();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			mandelbrot.displayInMode(Speed::Complete);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			mandelbrot.displayInMode(Speed::Fast);
		}
	}

	return 0;
}

double myRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}


/*
// TODO delete!!!
	sf::CircleShape circle(5.0f);
	circle.setFillColor(sf::Color::White);
	circle.setOrigin(5,5);

	-------------------------------------------------------------------------

sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window));
		circle.setPosition(mouse);
		window.clear();
		mandelbrot.drawLast();
		for (int i = 0; i < 100; i++)
		{
			std::complex<double> z = mandelbrot.zFromPixel((int)mouse.x, (int)mouse.y);
			//std::cout << z << std::endl;
			z = mandelbrot.fMandel(z, i);
			line[i] = sf::Vertex(sf::Vector2f(mandelbrot.reToPixel(z.real()), mandelbrot.imToPixel(z.imag())));
			//std::cout << mandelbrot.imToPixel(z.imag()) << std::endl;;
			line[i].color = sf::Color::Green;
		}
		window.draw(line, 100, sf::LinesStrip);
		//std::cout << line[99].position.x << std::endl;
		window.draw(circle);
		window.display();

*/