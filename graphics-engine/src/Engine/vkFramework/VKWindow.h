#pragma once

#include <Engine/BaseWindow.h>

class VKWindow : public BaseWindow
{
public:
	VKWindow(const char* title): BaseWindow(title) {};
	~VKWindow() {};

	bool init() override;

	void tick() override;

private:
	void setWindowHints() override;
};
