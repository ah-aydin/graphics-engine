#pragma once

namespace kbb
{
	class Level
	{
	public:
		Level() {};
		virtual void update(double dt) = 0;
		virtual ~Level() {};
	};
}