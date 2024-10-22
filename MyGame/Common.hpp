# pragma once
#include <Siv3D.hpp>

// 扱うシーンの名前を列挙
enum class State
{
	Title,
	Game,
};

// シーン間で共有するデータの構造体
struct GameData
{
};

using App = SceneManager<State, GameData>;
