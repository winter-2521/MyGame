#include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init } {}

void Title::update()
{
	m_startTransition.update(m_stageButton.mouseOver());
	m_rankingTransition.update(m_optionButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_stageButton.mouseOver() || m_optionButton.mouseOver() || m_exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (m_stageButton.leftClicked())
	{
		// ステージ選択画面に移動
		changeScene(State::Stage);
	}

	if (m_optionButton.leftClicked())
	{
		// オプションに移動
		changeScene(State::Option);
	}

	if (m_exitButton.leftClicked())
	{
		// 終了
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	FontAsset(U"TitleFont")(U"Twins")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

	m_stageButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
	m_optionButton.draw(ColorF{ 1.0, m_rankingTransition.value() }).drawFrame(2);
	m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

	FontAsset(U"Menu")(U"あそぶ").drawAt(m_stageButton.center(), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"オプション").drawAt(m_optionButton.center(), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"やめる").drawAt(m_exitButton.center(), ColorF{ 0.25 });
}
