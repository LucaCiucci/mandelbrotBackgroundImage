#pragma once

#include <math.h>
#include <complex>
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define BAILOUT 100
#define COMPLETE 2000
#define FAST 30
#define Np double
#define IMAGEPATH "b.bmp"

enum class Speed : int{
	Fast = 0,
	Complete = 1
};

class FrameLocation {
public:
	// TODO dove ci metto le posizioni e grandezza delle viste salvate
};

template <class T = Np>
class Mandelbrot {
private:
	typedef std::complex<T> complex;

	uint8_t
		*inputImg;
		//*img; // displayed image
	int
		winSizeX, winSizeY, // size of current window
		inputSizeX, inputSizeY; // size of  input image
	T
		bailout = BAILOUT,
		zoomFactor = 2, // zoom
		panFactor = 0.5; // pan
	int
		completeMaxIter = COMPLETE,
		fastMaxIter = FAST, // maxIter for fast preview
		updateEvery = 5000;

	sf::RenderWindow *window; // render window

	T reMin = -1.5, reMax = 0.5, imMin = -1.0, imMax = 1.0; // limits of the current view

	sf::Uint8 *pixels; // needed for drawing
	sf::Texture texture;
	sf::Sprite sprite;

public:
	std::string imagePath = IMAGEPATH;

	Mandelbrot(sf::RenderWindow &_window, std::string _imagePath = IMAGEPATH)
	{
		window = &_window;
		updateWindowSize();
		//tmpwindow.close();

		//img = (uint8_t*)malloc(winSizeX * winSizeY * sizeof(uint8_t));

		imagePath = _imagePath;
		sf::Texture texture;
		texture.loadFromFile(imagePath);
		inputSizeX = texture.getSize().x;
		inputSizeY = texture.getSize().y;
		inputImg = (uint8_t*)malloc(inputSizeX * inputSizeY * sizeof(uint8_t)*4);
		sf::Image tmp_img = texture.copyToImage();
		for (int i = 0; i < inputSizeX; i++)
			for (int j = 0; j < inputSizeY; j++)
			{
				inputImg[(i * inputSizeY + j)*4 + 0] = tmp_img.getPixel(i, j).r;
				inputImg[(i * inputSizeY + j)*4 + 1] = tmp_img.getPixel(i, j).g;
				inputImg[(i * inputSizeY + j)*4 + 2] = tmp_img.getPixel(i, j).b;
				inputImg[(i * inputSizeY + j)*4 + 3] = tmp_img.getPixel(i, j).a;
			}
	}

	void updateWindowSize(void)
	{
		winSizeX = window->getSize().x;
		winSizeY = window->getSize().y;
		double center = (imMin + imMax) / 2.0;
		imMin = center - (reMax - reMin) / 2.0 * winSizeY / winSizeX;
		imMax = center + (reMax - reMin) / 2.0 * winSizeY / winSizeX;
		//img = (uint8_t*)realloc(img, winSizeX * winSizeY * sizeof(uint8_t));
		
		sf::FloatRect visibleArea(0, 0, winSizeX, winSizeY);
		window->setView(sf::View(visibleArea));

		std::cout << "new window size: " << winSizeX << " * " << winSizeY << std::endl;
	};
	complex fMandel(complex p, int nIter = 100)
	{
		complex
			z(0, 0),
			min(INFINITY, INFINITY);
		
		for (int i = 0; i < nIter; i++)
		{
			z = z * z + p;
			min = (abs(min) > abs(z)) ? z : min;
			if (abs(z) > bailout)
				break;
		}
		return min;
	}

	complex zFromPixel(int px, int py)
	{
		Np x = (Np)px / winSizeX * (reMax - reMin) + reMin;
		Np y = (Np)py / winSizeY * (imMax - imMin) + imMin;
		return complex(x, y);
	};
	Np reToPixel(Np re)
	{
		Np pos = (re - reMin) / (reMax - reMin) * winSizeX;
		pos = fmax(pos, 0.01);
		pos = fmin(pos, winSizeX - 0.01);
		return pos;
	};
	Np imToPixel(Np im)
	{
		Np pos = (im - imMin) / (imMax - imMin) * winSizeY;
		pos = fmax(pos, 0.01);
		pos = fmin(pos, winSizeY - 0.01);
		return pos;
	};
	void displayInMode(Speed speed = Speed::Complete)
	{
		if (speed == Speed::Complete)
		{
			display(completeMaxIter);
			return;
		}
		display(fastMaxIter);
	};
	void display(int maxIter)
	{
		updateWindowSize();
		
		free(pixels);
		/*sf::Uint8 **/pixels = new sf::Uint8[winSizeX * winSizeY * 4];
		//sf::Texture texture;
		if (!texture.create(winSizeX, winSizeY))
			std::cout << "TEXTURE FAIL!!!!";
		//sf::Sprite sprite(texture);
		sprite.setPosition(sf::Vector2f(0.0, 0.0));
		sprite.setTexture(texture);
		sprite.setScale(sf::Vector2f(1.0,1.0));

		for (int i = 0; i < winSizeX * winSizeY; i++)
		{
			// current pixel position
			T x = (((T)(i / winSizeX) / winSizeY -0.5)*2.0) * (imMax - imMin) + (imMin + imMax) / 2.0;
			T y = (((T)(i % winSizeX) / winSizeX -0.5)*2.0) * (reMax - reMin) + (reMin + reMax) / 2.0;
			complex z = fMandel(complex(y, x), maxIter);

			if (abs(z) < 1)
			{
				int index = (floor((z.real() + 1) * inputSizeX/2) * inputSizeY + floor((z.imag() + 1)*inputSizeY/2)) * 4;
				// TODO elimina-> int index = (floor((z.real() + (reMax + reMin) / 2) * X/(reMax - reMin) + X / 2) * Y + floor((z.imag() + (imMax + imMin)/2)*Y/(imMax - imMin) + Y/2)) * 4;

				pixels[i * 4 + 0] = inputImg[index + 0];
				pixels[i * 4 + 1] = inputImg[index + 1];
				pixels[i * 4 + 2] = inputImg[index + 2];
				pixels[i * 4 + 3] = inputImg[index + 3];
			}
			else
			{
				pixels[i * 4 + 0] = 0;
				pixels[i * 4 + 1] = 0;
				pixels[i * 4 + 2] = 0;
				pixels[i * 4 + 3] = 255;
			}

			if (i % updateEvery == 0)
			{
				sf::Event _event;
				while (window->pollEvent(_event)) {}
				window->clear();
				texture.update(pixels);
				window->draw(sprite);
				window->display();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
				break;
		}

		window->clear();
		texture.update(pixels);
		window->draw(sprite);
		window->display();
		std::cout << "saving..." << std::endl;
		texture.copyToImage().saveToFile("output.bmp");
		std::cout << "frame completed! @ maxIter = " << maxIter << std::endl;
	}
	void display(void)
	{
		display(completeMaxIter);
	};
	void display(FrameLocation);
	void drawLast(void)
	{
		window->draw(sprite);
	};
	void zoomIn(T factor)
	{
		double lenght = reMax - reMin;
		double center = (reMax + reMin) / 2.0;

		reMax = center + lenght / 2.0 / factor;
		reMin = center - lenght / 2.0 / factor;
		updateWindowSize();

		sprite.setTexture(texture);
		sprite.scale(sf::Vector2f(factor, factor));
		sprite.setPosition( (sprite.getPosition() - sf::Vector2f(winSizeX / 2.0, winSizeY / 2.0)) * (float)factor + sf::Vector2f(winSizeX / 2.0, winSizeY / 2.0));
		window->clear();
		window->draw(sprite);
		window->display();
		//Sleep(5000);

		//displayInMode(Speed::Fast);
	};
	void zoomIn(void)
	{
		zoomIn(zoomFactor);
	}
	void zoomOut(T factor)
	{
		zoomIn(1.0 / factor);
	};
	void zoomOut(void)
	{
		zoomOut(zoomFactor);
	};
	void panUp(T factor)
	{
		updateWindowSize();

		double lenght;
		double center;

		lenght = imMax - imMin;
		center = (imMax + imMin) / 2.0;
		imMax -= lenght * factor;
		imMin -= lenght * factor;

		sprite.setTexture(texture);
		sprite.move(sf::Vector2f(0, winSizeY * factor / 2.0));
		window->clear();
		window->draw(sprite);
		window->display();
		//Sleep(5000);

		//displayInMode(Speed::Fast);
	};
	void panUp(void)
	{
		panUp(panFactor);
	};
	void panDown(T factor)
	{
		panUp(-factor);
	};
	void panDown(void)
	{
		panUp(-panFactor);
	};
	void panLeft(T factor)
	{
		updateWindowSize();

		double lenght;
		double center;

		lenght = reMax - reMin;
		center = (reMax + reMin) / 2.0;
		reMax -= lenght * factor;
		reMin -= lenght * factor;

		sprite.setTexture(texture);
		sprite.move(sf::Vector2f(winSizeX * factor / 2.0, 0));
		window->clear();
		window->draw(sprite);
		window->display();
		//Sleep(5000);

		//displayInMode(Speed::Fast);
	};
	void panLeft(void)
	{
		panLeft(panFactor);
	};
	void panRight(T factor)
	{
		panLeft(-factor);
	};
	void panRight(void)
	{
		panLeft(-panFactor);
	};
};



/*void setReMin(Np _reMin)
	{
		reMin = _reMin;
		Np yCenter = (imMin + imMax) * 0.5;
		Np h_2 = (reMax - reMin) * wino
	};*/