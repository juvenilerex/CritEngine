#include "Application.h"
#include "Logging/Logger.h"

namespace Engine {

	Application::Application()
	{
		this->window = std::make_unique<Window>(800, 600, "Sandbox");

		this->window->SetEventCallback(BIND_EVENT_FUNC(this->OnEvent));
		GetInput().SetEventCallback(BIND_EVENT_FUNC(this->OnInput));
	};

	Application::~Application()
	{
		this->window.reset();
	};

	void Application::TickInternal()
	{
		if (this->window != nullptr)
		{
			for (Layer* layer : this->layerStack)
				layer->OnUpdate();
			this->window->GetInput().PollKeyEvents();
			this->window->GetInput().PollMouseEvents();
			this->window->PollEvents();
			this->window->SwapBuffers();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		this->layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		this->layerStack.PushLayer(overlay);
	}

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(this->OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(this->OnWindowClose));

        for (auto it = layerStack.begin(); it != layerStack.end(); it++) {
            if (event.IsHandled())
                return;
			(*it)->OnEvent(event);
        }
    }

	// Tests
	void Application::OnInput(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(this->SendInput));
//		dispatcher.Dispatch<MouseReleasedEvent>(BIND_EVENT_FUNC(this->SendMouse));
//		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNC(this->SendMousePos));
//		dispatcher.Dispatch<KeyJustPressedEvent>(BIND_EVENT_FUNC(this->SendInput));
//		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNC(this->SendInput));

	}

	bool Application::SendInput(KeyboardEvent& event) {
		Debug::Log("KeyPressedEvent: ", event.GetKeyCode());
		return false;
	}

	bool Application::SendMouse(MouseEvent& event)
	{
		Debug::Log("MousePressedEvent: ", event.GetButton());
		return false;
	}

	bool Application::SendMousePos(MouseMovedEvent& event)
	{
		Debug::Log("MouseMovedEvent: ", event.GetCurPos());
		return false;
	}

	///////////////////////////////////////////////////////////////////

	Window& Application::GetWindow()
	{
		ASSERT(this->window)
		return *this->window.get();
	}

	InputListener& Application::GetInput() const
	{
		return this->window->GetInput();
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		event.Print();
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		// Window closing logic here
		return false;
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

}

