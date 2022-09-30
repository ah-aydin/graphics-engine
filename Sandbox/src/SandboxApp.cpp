#include <Kebab.h>

class Sandbox : public kbb::Application
{
public:
	Sandbox() : Application()
	{
		kbb::Input::createAxis("VERTICAL", GLFW_KEY_W, GLFW_KEY_S);
		kbb::Input::createAxis("HORIZONTAL", GLFW_KEY_D, GLFW_KEY_A);
		kbb::Input::createAction("QUIT", GLFW_KEY_ESCAPE);
	}
	~Sandbox() {}
};

kbb::Application* kbb::CreateApplication()
{
	return new Sandbox();
}