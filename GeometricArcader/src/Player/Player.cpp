#include "Player.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Collision/BorderCollision.h"

using namespace Engine;

Player::Player()
	: m_Position{ 0.f, 0.f, 0.f }
	, m_Size{ 50.f, 50.f }
	, m_EnergyBar{ 100.f }
	, m_SpeedController{}
{
}

void Player::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<KeyReleasedEvent>(ENGINE_BIND_EVENT_FN(Player::OnKeyReleased));
}

void Player::Update(const BorderCollision& coll, float deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateCollision(coll);

	m_SpeedController.Update();
}

void Player::Render() const
{
	Renderer2D::SetDrawColor(Color::red);
	FlyFishUtils::DrawFillRect(m_Position, glm::vec2{ m_Size.x - 4.f, m_Size.y - 4.f });
	Renderer2D::SetDrawColor(Color::white);
	FlyFishUtils::DrawFillRect(m_Position, m_Size);

	m_EnergyBar.Render();
	m_SpeedController.Render();
}

// Getters //
const TriVector& Player::GetPosition() const
{
	return m_Position;
}

const glm::vec2& Player::GetSize() const
{
	return m_Size;
}

void Player::UpdateMovement(float deltaTime)
{
	constexpr float baseMoveSpeed{ 3000.f };
	const bool sprinting{ Input::IsKeyPressed(Key::LeftShift) };
	const float moveSpeed{ sprinting ? baseMoveSpeed * 2.f : baseMoveSpeed };
	const float deltaSpeed{ moveSpeed * deltaTime };

	if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
	{
		FlyFishUtils::Translate(m_Position, glm::vec3{ 0.f, 1.f, 0.f }, deltaSpeed);
	}
	if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
	{
		FlyFishUtils::Translate(m_Position, glm::vec3{ 0.f, -1.f, 0.f }, deltaSpeed);
	}
	if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left))
	{
		FlyFishUtils::Translate(m_Position, glm::vec3{ -1.f, 0.f, 0.f }, deltaSpeed);
	}
	if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right))
	{
		FlyFishUtils::Translate(m_Position, glm::vec3{ 1.f, 0.f, 0.f }, deltaSpeed);
	}
}

void Player::UpdateCollision(const BorderCollision& coll)
{
	coll.HandleCollision(m_Position, m_Size);
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	//if (e.GetKeyCode() == Key::W || e.GetKeyCode() == Key::Up)
	//{
	//	/*const Motor motor{ Motor::Translation(static_cast<float>(1080 / 2), FlyFishUtils::yAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();*/

	//	FlyFishUtils::Translate(m_Position, glm::vec3{ 0.f, 1.f, 0.f }, static_cast<float>(1080 / 2));
	//}
	//else if (e.GetKeyCode() == Key::S || e.GetKeyCode() == Key::Down)
	//{
	//	/*const Motor motor{ Motor::Translation(-static_cast<float>(1080 / 2), FlyFishUtils::yAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();*/

	//	FlyFishUtils::Translate(m_Position, glm::vec3{ 0.f, -1.f, 0.f }, static_cast<float>(1080 / 2));
	//}
	//else if(e.GetKeyCode() == Key::A || e.GetKeyCode() == Key::Left)
	//{
	//	/*const Motor motor{ Motor::Translation(-static_cast<float>(1920 / 2), FlyFishUtils::xAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();*/

	//	FlyFishUtils::Translate(m_Position, glm::vec3{ -1.f, 0.f, 0.f }, static_cast<float>(1920 / 2));
	//}
	//else if(e.GetKeyCode() == Key::D || e.GetKeyCode() == Key::Right)
	//{
	//	/*const Motor motor{ Motor::Translation(static_cast<float>(1920 / 2), FlyFishUtils::xAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();*/

	//	FlyFishUtils::Translate(m_Position, glm::vec3{ 1.f, 0.f, 0.f }, static_cast<float>(1920 / 2));
	//}

	return false;
}