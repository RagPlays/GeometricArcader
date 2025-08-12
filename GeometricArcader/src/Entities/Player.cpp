#include "Player.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Player::Player()
	: m_Position{ 0.f, 0.f, 0.f }
	, m_Size{ 50.f, 50.f }
{
}

void Player::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<KeyReleasedEvent>(ENGINE_BIND_EVENT_FN(OnKeyReleased));
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	//if (e.GetKeyCode() == Key::W || e.GetKeyCode() == Key::Up)
	//{
	//	ENGINE_TRACE("Up key released");
	//	ENGINE_TRACE("Position before: ({0}, {1}, {0})", m_Position[0], m_Position[1], m_Position[2]);
	//	ENGINE_TRACE("Position TriVector before: {}", m_Position.ToString());
	//	const Motor motor{ Motor::Translation(static_cast<float>(1080 / 2), FlyFishUtils::yAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();

	//	ENGINE_TRACE("Moved: {0}, Motor: {1}", static_cast<float>(1080 / 2), motor.ToString());
	//	ENGINE_TRACE("Position after: ({0}, {1}, {0})", m_Position[0], m_Position[1], m_Position[2]);
	//	ENGINE_TRACE("Position TriVector after: {}", m_Position.ToString());
	//	ENGINE_TRACE(""); // space
	//}
	//else if (e.GetKeyCode() == Key::S || e.GetKeyCode() == Key::Down)
	//{
	//	ENGINE_TRACE("Down key released");
	//	const Motor motor{ Motor::Translation(-static_cast<float>(1080 / 2), FlyFishUtils::yAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();
	//}
	//else if(e.GetKeyCode() == Key::A || e.GetKeyCode() == Key::Left)
	//{
	//	ENGINE_TRACE("Left key released");
	//	const Motor motor{ Motor::Translation(-static_cast<float>(1920 / 2), FlyFishUtils::xAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();
	//}
	//else if(e.GetKeyCode() == Key::D || e.GetKeyCode() == Key::Right)
	//{
	//	ENGINE_TRACE("Right key released");
	//	const Motor motor{ Motor::Translation(static_cast<float>(1920 / 2), FlyFishUtils::xAxis) };
	//	m_Position = MultiVector{ motor * m_Position * ~motor }.Grade3();
	//}

	return false;
}

void Player::Update(float deltaTime)
{
	// movespeed
	constexpr float baseMoveSpeed{ 500.f };
	bool sprinting{ Input::IsKeyPressed(Key::LeftShift) };
	const float moveSpeed{ sprinting ? baseMoveSpeed * 2.f : baseMoveSpeed };
	const float deltaSpeed{ moveSpeed * deltaTime };

	const BiVector xMoveLine{ 1.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	const BiVector yMoveLine{ 0.f, 1.f, 0.f, 0.f, 0.f, 0.f };

	if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
	{
		const Motor upMotor{ Motor::Translation(deltaSpeed, yMoveLine)};
		//ENGINE_TRACE("UpMotor: {0}, with DeltaSpeed: {1}", upMotor.ToString(), deltaSpeed);
		m_Position = MultiVector{ upMotor * m_Position * ~upMotor }.Grade3();
	}
	if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
	{
		const Motor downMotor{ Motor::Translation(-deltaSpeed, yMoveLine) };
		//ENGINE_TRACE("DownMotor: {0}, with DeltaSpeed: {1}", downMotor.ToString(), deltaSpeed);
		m_Position = MultiVector{ downMotor * m_Position * ~downMotor }.Grade3();
	}
	if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left))
	{
		const Motor leftMotor{ Motor::Translation(-deltaSpeed, xMoveLine) };
		//ENGINE_TRACE("LeftMotor: {0}, with DeltaSpeed: {1}", leftMotor.ToString(), deltaSpeed);
		m_Position = MultiVector{ leftMotor * m_Position * ~leftMotor }.Grade3();
	}
	if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right))
	{
		const Motor rightMotor{ Motor::Translation(deltaSpeed, xMoveLine) };
		//ENGINE_TRACE("RightMotor: {0}, with DeltaSpeed: {1}", rightMotor.ToString(), deltaSpeed);
		m_Position = MultiVector{ rightMotor * m_Position * ~rightMotor }.Grade3();
	}
}

void Player::Render() const
{
	Renderer2D::SetDrawColor(Color::orange);
	FlyFishUtils::DrawFillRect(m_Position, glm::vec2{ m_Size.x - 3.f, m_Size.y - 3.f });
	Renderer2D::SetDrawColor(Color::red);
	FlyFishUtils::DrawRect(m_Position, m_Size);
}