#pragma once

class BaseApp
{
protected:
	BaseApp();
	virtual ~BaseApp();

protected:
	virtual bool init();
	virtual void initInput() = 0;
	virtual void quit();

	virtual void mainLoop() = 0;
	
	bool running = true;
	
protected:
	virtual int run() = 0;
};

