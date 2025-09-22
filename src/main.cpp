#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "vectormath.h"

sf::Font font;

float pi = 3.14159265359f;

const int windowWidth = 1600;
const int windowHeight = 1000;

sf::Vector3f cam_position = {0, 0, -50};
std::vector<float> cam_angle = {0, 0, 0};
sf::Vector3f display_position = {0, 0, 100};

Matrix3<float> rotMatrixX;
Matrix3<float> rotMatrixY;
Matrix3<float> rotMatrixZ;

const int ballAmount = 2000;

const float speed = 0.1f;

const int caseNumber = 0;
float sigma = 10.f, rho = 28.f, beta = 8.f / 3.f;
float a = 0.1f, b = 0.1f, c = 14.0f;

void updatePos(float pos[ballAmount][3], float elapsed)
{
	if (caseNumber == 0)
	{
		for (int i = 0; i < ballAmount; i++)
		{
			pos[i][0] += (sigma * (pos[i][1] - pos[i][0])) * speed * elapsed;
			pos[i][1] += (pos[i][0] * (rho - pos[i][2]) - pos[i][1]) * speed * elapsed;
			pos[i][2] += (pos[i][0] * pos[i][1] - beta * pos[i][2]) * speed * elapsed;
		}
	}
}

float getParticleSpeed(float pos[3])
{
	float dx = -(pos[1] + pos[2]);
	float dy = pos[0] + (a * pos[1]);
	float dz = b + pos[2] * (pos[0] - c);
	return sqrt(dx * dx + dy * dy + dz * dz);
}

float lerpColor(float t, float left, float right)
{
	return left + t * (right - left);
}

int main()
{
	font.loadFromFile("../src/ArchitectsDaughter.ttf");
	// font.setSmooth(false);
	sf::Text txt = sf::Text("Lorenz Attractor", font, 24);
	txt.setFillColor(sf::Color::White);
	txt.setScale(0.05f, 0.05f);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lorenz Attractor", sf::Style::Fullscreen);
	sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(100.f, 100.f));
	window.setView(view);
	window.setFramerateLimit(1000);

	float pos[ballAmount][3];
	for (int i = 0; i < ballAmount; i++)
	{
		// spread the balls along the line y = -x evenly between -10 and 10
		pos[i][0] = (i + 1) * 1.0f;
		pos[i][1] = 0;
		pos[i][2] = 0;
	}

	sf::CircleShape shape[ballAmount];
	for (int i = 0; i < ballAmount; i++)
	{
		shape[i].setRadius(0.2f);
		shape[i].setPosition(pos[i][0], pos[i][1]);
		shape[i].setFillColor(sf::Color::Red);
	}

	sf::Clock clock;
	int frame = 0;
	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				cam_position.x -= sin(cam_angle[1] + pi / 2.0f) * 0.25f;
				cam_position.z -= cos(cam_angle[1] + pi / 2.0f) * 0.25f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				cam_position.x += sin(cam_angle[1] + pi / 2.0f) * 0.25f;
				cam_position.z += cos(cam_angle[1] + pi / 2.0f) * 0.25f;
			}

			// Move Forwards and Backwards
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				cam_position.z -= cos(cam_angle[1]) * 0.25f;
				cam_position.x -= sin(cam_angle[1]) * 0.25f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				cam_position.z += cos(cam_angle[1]) * 0.25f;
				cam_position.x += sin(cam_angle[1]) * 0.25f;
			}

			// Move Up and Down
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				cam_position.y += 0.1f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				cam_position.y -= 0.1f;

			// Update Camera Angle

			// Look Left and Right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				cam_angle[1] -= 0.003f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				cam_angle[1] += 0.003f;

			// Look Up and Down
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				cam_angle[0] += 0.003f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				cam_angle[0] -= 0.003f;

			rotMatrixX =
			    {
				{1, 0, 0},
				{0, cos(cam_angle[0]), sin(cam_angle[0])},
				{0, -sin(cam_angle[0]), cos(cam_angle[0])}};
			rotMatrixY =
			    {
				{cos(cam_angle[1]), 0, -sin(cam_angle[1])},
				{0, 1, 0},
				{sin(cam_angle[1]), 0, cos(cam_angle[1])}};
			rotMatrixZ =
			    {
				{cos(cam_angle[2]), sin(cam_angle[2]), 0},
				{-sin(cam_angle[2]), cos(cam_angle[2]), 0},
				{0, 0, 1}};

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		updatePos(pos, elapsed.asSeconds());

		for (int i = 0; i < ballAmount; i++)
		{
			/*shape[i].setPosition(pos[i][0], pos[i][1]);
			shape[i].setFillColor(sf::Color(pos[i][2] < 0 ? 255 : - 255 * (pos[i][2] - 50) / 50, 0, pos[i][2] > 50 ? 255 : 255 * (pos[i][2] - 0) / 50));
			window.draw(shape[i]);*/

			sf::Vector3f d;
			d = {pos[i][0] - cam_position.x, pos[i][1] - cam_position.y, pos[i][2] - cam_position.z};
			d = rotMatrixX * (rotMatrixY * (rotMatrixZ * d));

			// Only if the point is infront of the camera
			if (d.z >= 0)
			{
				sf::Vector2f projected_point =
				    {
					display_position.z * d.x / d.z + display_position.x,
					display_position.z * d.y / d.z + display_position.y};

				shape[i].setPosition(sf::Vector2f(projected_point.x, projected_point.y));

				float startcolor[3] = {255, 0, 0};
				float endcolor[3] = {0, 255, 255};

				float minspeed = 0.0f;
				float maxspeed = 50.0f;
				shape[i].setFillColor(sf::Color(
				    lerpColor(getParticleSpeed(pos[i]) / maxspeed, startcolor[0], endcolor[0]),
				    lerpColor(getParticleSpeed(pos[i]) / maxspeed, startcolor[1], endcolor[1]),
				    lerpColor(getParticleSpeed(pos[i]) / maxspeed, startcolor[2], endcolor[2])));
				window.draw(shape[i]);
			}
		}

		// txt.setPosition(0, 0);
		// window.draw(txt);

		window.display();

		frame++;
	}

	return 0;
}