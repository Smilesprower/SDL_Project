#pragma once

namespace Command
{
	enum ID
	{
		NONE,
		QUIT,
		BUTTONA,
		BUTTONB,
		BUTTONX,
		BUTTONY
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
		NONE = 0,   // 0 
		Title, 
		Game,
	};
}