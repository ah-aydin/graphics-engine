#pragma once


class Level
{
public:
	Level() {};
	virtual bool init();
	virtual void update(double dt);
	virtual ~Level() {};
};