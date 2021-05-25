#include "SFML/Graphics.hpp"
#include<time.h>
using namespace sf;

float speed = 10;
float b = 2; //bullet speed factor

int main()
{
	RenderWindow window(VideoMode(2000, 1000), "Space!");
	window.setFramerateLimit(60);

	float currentFrame;

	Texture t1, t3;

	t1.loadFromFile("courseSFML/resources/black.png");

	t3.loadFromFile("courseSFML/resources/SpaceShipTransparent.png");

	t1.setRepeated(true);

	Sprite sB1(t1), sShip(t3);

	sB1.setTextureRect(IntRect(0, 0, 2000, 100000));

	sShip.setTextureRect(IntRect(40, 0, 40, 40));
	sShip.setPosition(2000 / 2, 100000 - 1000);
	sShip.scale(2, 2);

	View view(FloatRect(0, 0, 2000, 1000));

	//SHOOTER
	CircleShape projectile;
	projectile.setFillColor(Color::Green);
	projectile.setRadius(5.f);

	CircleShape projectile2;
	projectile2.setFillColor(Color::Green);
	projectile2.setRadius(5.f);

	Vector2f playerCenter;
	int shootTimer = 0;

	std::vector<CircleShape> projectiles;
	projectiles.push_back(CircleShape(projectile));

	std::vector<CircleShape> projectiles2;
	projectiles2.push_back(CircleShape(projectile2));

	//enemy
	RectangleShape enemy;
	enemy.setFillColor(Color::Magenta);
	enemy.setSize(Vector2f(50.f, 50.f));


	std::vector<RectangleShape> enemies;
	enemies.push_back(RectangleShape(enemy));
	int enemySpawnTimer = 0;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}


		//PROJECTILES
		playerCenter = Vector2f(sShip.getPosition().x, sShip.getPosition().y);
		shootTimer++;

		if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= 10) //Shoot
		{
			projectile.setPosition(playerCenter);
			projectiles.push_back(CircleShape(projectile));

			projectile2.setPosition(sShip.getPosition().x + 60, sShip.getPosition().y);
			projectiles2.push_back(CircleShape(projectile2));

			shootTimer = 0;
		}

		for (size_t i = 0; i < projectiles.size(); i++)
		{
			projectiles[i].move(0.f, -b * speed);

			if (projectiles[i].getPosition().y <= 0)
				projectiles.erase(projectiles.begin() + i);
		}

		for (size_t i = 0; i < projectiles2.size(); i++)
		{
			projectiles2[i].move(0.f, -b * speed);

			if (projectiles2[i].getPosition().y <= 0)
				projectiles2.erase(projectiles2.begin() + i);
		}

		if (!Keyboard::isKeyPressed(Keyboard::Left))
			if (!Keyboard::isKeyPressed(Keyboard::Right))
				sShip.setTextureRect(IntRect(40, 0, 40, 40));

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			sShip.move(-speed, 0);

			currentFrame = 1;
			if (currentFrame > 6) currentFrame -= 6;

			sShip.setTextureRect(IntRect(40 * int(currentFrame) - 40, 0, 38, 40));

		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			sShip.move(speed, 0);
			currentFrame = 1;
			if (currentFrame > 6) currentFrame -= 6;

			sShip.setTextureRect(IntRect(40 * int(currentFrame) + 40, 0, 40, 40));
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			sShip.move(0, -speed);
			currentFrame = 1;
			if (currentFrame > 6) currentFrame -= 6;

			sShip.setTextureRect(IntRect(40 * int(currentFrame), 0, 40, 40));
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			sShip.move(0, speed / 2);
			currentFrame = 1;
			if (currentFrame > 6) currentFrame -= 6;

			sShip.setTextureRect(IntRect(40 * int(currentFrame), 0, 40, 40));
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				sShip.move(-speed, 0);

				currentFrame = 1;
				if (currentFrame > 6) currentFrame -= 6;

				sShip.setTextureRect(IntRect(40 * int(currentFrame) - 40, 0, 38, 40));

			}

		if (Keyboard::isKeyPressed(Keyboard::Up))
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				sShip.move(speed, 0);
				currentFrame = 1;
				if (currentFrame > 6) currentFrame -= 6;

				sShip.setTextureRect(IntRect(40 * int(currentFrame) + 40, 0, 40, 40));
			}


		view.setCenter(1000, sShip.getPosition().y - 300);

		sShip.move(0, -speed);

		//ENEMIES
		if (enemySpawnTimer < 10)
			enemySpawnTimer++;

		if (enemySpawnTimer >= 10)
		{
			enemy.setPosition(sShip.getPosition().x - 1000 + rand() % 3000, sShip.getPosition().y - 900);

			enemies.push_back(RectangleShape(enemy));

			enemySpawnTimer = 0;
		}

		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemies[i].move(0.f, 0.3f);

			if (enemies[i].getPosition().y > sShip.getPosition().y + 100)
				enemies.erase(enemies.begin() + i);
		}

		//Collision
		if (!enemies.empty() && !projectiles.empty())
		{
			for (size_t i = 0; i < projectiles.size(); i++)
			{
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (projectiles[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
					{
						projectiles.erase(projectiles.begin() + i);
						enemies.erase(enemies.begin() + k);
						break;
					}
				}
			}
		}


		if (!enemies.empty() && !projectiles2.empty())
		{
			for (size_t i = 0; i < projectiles2.size(); i++)
			{
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (projectiles2[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
					{
						projectiles2.erase(projectiles2.begin() + i);
						enemies.erase(enemies.begin() + k);
						break;
					}
				}
			}
		}

		window.clear(Color::White);
		window.setView(view);
		window.draw(sB1);
		window.draw(sShip);
		for (size_t i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i]);
		}
		for (size_t i = 0; i < projectiles.size(); i++)
		{
			window.draw(projectiles[i]);
		}
		for (size_t i = 0; i < projectiles2.size(); i++)
		{
			window.draw(projectiles2[i]);
		}
		window.display();

	}

	return 0;
}