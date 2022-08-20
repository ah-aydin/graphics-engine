#pragma once


class Level
{
public:
	Level() {};
	virtual bool init();
	virtual void update(float dt);
	virtual ~Level() {};
};