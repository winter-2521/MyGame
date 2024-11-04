#pragma once
#include <Siv3D.hpp>

// 扱うシーンの名前を列挙
enum class State
{
	Title,
	Game,
	Option,
	Stage,
	Level,
};

// シーン間で共有するデータの構造体
struct GameData
{
	// 選択したステージ番号
	int32 stage_number = 0;

	// 選択したレべル番号
	int32 level_number = 0;
};

using App = SceneManager<State, GameData>;
