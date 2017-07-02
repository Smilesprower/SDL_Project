#pragma once

namespace Command
{
	enum ID
	{
		NONE,
		JUMP,
		MOVE,
		MOVEUP,
		QUIT
	};
}


namespace Actions
{
	enum ID
	{
		PUSH_SCENE,
		POP_SCENE,
		CLEAR_SCENE_STACK,
	};
}

namespace Events
{
	enum ID
	{
		ACTIONBUTTON,
		CANCELBUTTON,
	};
}

namespace SceneID
{
	enum ID
	{
		None = 0,   // 0 
		Title, 
		Game,
	};
}