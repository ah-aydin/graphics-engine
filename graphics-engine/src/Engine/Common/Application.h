#pragma once
class Application
{
protected:
	Application();
	virtual ~Application();

	Application(const Application&) = delete;
	Application operator=(const Application&) = delete;

	virtual void run() = 0;
};

