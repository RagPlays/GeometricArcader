#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <Engine.h>

class IGameState
{
public:

	enum class GameStateType
	{
		None,
		Start,
		MainGame,
		End
	};

	virtual void OnEvent(Engine::Event& e) = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() const = 0;

	virtual bool IsComplete() const = 0;
	virtual GameStateType NextState() const = 0;
};

#endif // !GAMESTATE_H