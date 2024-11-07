#include "Common.hpp"
#include "Title.hpp"
#include "Option.hpp"
#include "Stage.hpp"
#include "Level.hpp"
#include "Game.hpp"

void Main()
{
	// Escキーによるプログラム終了を無効化
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	// F1キーによるライセンス表示を無効化
	LicenseManager::DisableDefaultTrigger();

	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Medium);
	FontAsset::Register(U"Option", 40, Typeface::Heavy);
	FontAsset::Register(U"Clear", 40, Typeface::Medium);

	// シーンマネージャーの初期化
	App manager;
	manager.add<Title>(State::Title);
	manager.add<Option>(State::Option);
	manager.add<Stage>(State::Stage);
	manager.add<Level>(State::Level);
	manager.add<Game>(State::Game);

	while (System::Update()){
		if (not manager.update()){
			break;
		}
	}
}

