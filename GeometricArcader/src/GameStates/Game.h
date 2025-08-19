#ifndef GAME_H
#define GAME_H

#include "GameStates/IGameState.h"

#include "Collision/BorderCollision.h"
#include "Player/Player.h"
#include "Pickups/Pickup.h"
#include "Pillar/Pillar.h"
#include "UI/General/ProgressBar.h"

class Game final : public IGameState
{
public:

	Game();
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) noexcept = delete;

	virtual void OnEvent(Engine::Event& e) override;

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	virtual bool IsComplete() const override;
	virtual GameStateType NextState() const override;

	uint32_t GetScore() const;

private:

	void UpdatePickups();
	void RenderPickups() const;

	void SetupGameTimer();
	void UpdateGameTimer(float deltaTime);

	// Window Events //
	bool OnWindowResize(Engine::WindowResizeEvent& e);

private:

	Engine::Window& m_Window;
	BorderCollision m_BorderCollision;
	Player m_Player;

	ProgressBar m_GameTimer;

	std::vector<Pickup> m_Pickups;
	Pillar m_Pillar;

	uint32_t m_Score;
};

#endif // !GAME_H