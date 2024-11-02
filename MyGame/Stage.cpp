#include "Stage.hpp"

Stage::Stage(const InitData& init)
	: IScene{ init } {}

void Stage::update()
{
	m_stage1_Transition.update(m_stage1_Button.mouseOver());
	m_stage2_Transition.update(m_stage2_Button.mouseOver());
	m_stage3_Transition.update(m_stage3_Button.mouseOver());
	m_back_Transition.update(m_back_Button.mouseOver());

	if (m_stage1_Button.mouseOver() || m_stage2_Button.mouseOver() || m_stage3_Button.mouseOver() || m_back_Button.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (m_back_Button.leftClicked())
	{
		// タイトルに戻る
		changeScene(State::Title);
	}
}

void Stage::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	m_stage1_Button.draw(ColorF{ 1.0, m_stage1_Transition.value() }).drawFrame(2);
	m_stage2_Button.draw(ColorF{ 1.0, m_stage2_Transition.value() }).drawFrame(2);
	m_stage3_Button.draw(ColorF{ 1.0, m_stage3_Transition.value() }).drawFrame(2);
	m_back_Button.draw(ColorF{ 1.0, m_back_Transition.value() }).drawFrame(2);

	FontAsset(U"Menu")(U"Stage 1").drawAt(m_stage1_Button.center().movedBy(0, -30), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"Stage 2").drawAt(m_stage2_Button.center().movedBy(0, -30), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"Stage 3").drawAt(m_stage3_Button.center().movedBy(0, -30), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"戻る").drawAt(m_back_Button.center(), ColorF{ 0.25 });
}
