#ifndef PICKUP_H
#define PICKUP_H

#include <Engine.h>
#include "FlyFish.h"

class Player;

class Pickup final
{
public:

	Pickup();
	~Pickup() = default;

	void OnEvent(Engine::Event& e);

	void Render() const;

	void RandomizePosition();

	const TriVector& GetPosition() const;
	const glm::vec2& GetSize() const;

private:

	const Engine::Window& m_Window;
	TriVector m_Position;
	const glm::vec2 m_Size;

};

#endif // !PICKUP_H