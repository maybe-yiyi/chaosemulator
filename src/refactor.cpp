#include <SFML/Graphics.hpp>
#include "vectormath.h"
#include <vector>

struct planet
{
	sf::Vector3f pos;
	sf::Vector3f vel;
	sf::CircleShape shape;
	float mass;
};

const float speed = 0.01f;
const float G = 0.0000000000667f;

void updatePos(std::vector<planet> &objs, float elapsed)
{
	for (int i = 0; i < objs.size(); i++)
	{
		// update positions
		objs[i].pos.x += (objs[i].vel.x) * speed * elapsed;
		objs[i].pos.y += (objs[i].vel.y) * speed * elapsed;
		objs[i].pos.z += (objs[i].vel.z) * speed * elapsed;

		// update velocities
		for (int j = 0; j < objs.size(); j++)
		{
			if (i != j)
			{
				float dist = sqrt(pow(objs[i].pos.x - objs[j].pos.x, 2) + pow(objs[i].pos.y - objs[j].pos.y, 2) + pow(objs[i].pos.z - objs[j].pos.z, 2));
				float force = G * objs[i].mass * objs[j].mass / pow(dist, 2);
				objs[i].vel.x += (objs[j].pos.x - objs[i].pos.x) * force * elapsed;
				objs[i].vel.y += (objs[j].pos.y - objs[i].pos.y) * force * elapsed;
				objs[i].vel.z += (objs[j].pos.z - objs[i].pos.z) * force * elapsed;
			}
		}

		std::cout << objs[i].pos.x << ", " << objs[i].pos.y << ", " << objs[i].pos.z << std::endl;
		std::cout << objs[i].vel.x << ", " << objs[i].vel.y << ", " << objs[i].vel.z << std::endl;
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1000), "Gravity", sf::Style::Fullscreen);
	sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(50000.f, 50000.f));
	window.setView(view);
	window.setFramerateLimit(240);

	std::vector<planet> objs(2);
	objs[0].pos = {-10000, 0, 0};
	objs[0].vel = {0, -10000, 0};
	objs[0].mass = 0.0123e10f;
	objs[0].shape.setRadius(600);
	objs[0].shape.setPosition(objs[0].pos.x, objs[0].pos.y);
	objs[0].shape.setFillColor(sf::Color::Red);

	objs[1].pos = {9640, 0, 0};
	objs[1].vel = {0, 10000, 0};
	objs[1].mass = 1e10f;
	objs[1].shape.setRadius(1200);
	objs[1].shape.setPosition(objs[1].pos.x, objs[1].pos.y);
	objs[1].shape.setFillColor(sf::Color::Red);

	sf::Clock clock;
	int frame = 0;
	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		updatePos(objs, elapsed.asSeconds());

		window.clear(sf::Color::Black);

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i].shape.setPosition(objs[i].pos.x, objs[i].pos.y);
			window.draw(objs[i].shape);

			std::cout << objs[i].pos.x << ", " << objs[i].pos.y << ", " << objs[i].pos.z << std::endl;
			std::cout << objs[i].vel.x << ", " << objs[i].vel.y << ", " << objs[i].vel.z << std::endl;
		}

		window.display();
		frame++;
	}

	return 0;
}