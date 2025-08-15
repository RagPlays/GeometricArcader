#ifndef PILLAR_H
#define PILLAR_H

#include <Engine.h>

#include "FlyFish.h"

class Pillar final
{
public:

	Pillar(const TriVector& position);
	~Pillar() = default;

	Pillar(const Pillar& other) = delete;
	Pillar(Pillar&& other) noexcept = delete;
	Pillar& operator=(const Pillar& other) = delete;
	Pillar& operator=(Pillar&& other) noexcept = delete;

	void Render() const;

	const TriVector& GetPosition() const;

private:

	const TriVector m_Position;
	const glm::vec2 m_Size;
};

#endif // !PILLAR_H
