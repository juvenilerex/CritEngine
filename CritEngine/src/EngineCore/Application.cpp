#include "Application.h"
#include "Logging/Logger.h"
#include "Core/MainLoop.h"

namespace Engine {

	Application::Application()
	{
		this->window = std::make_unique<Window>(800, 600, "Sandbox");

		this->window->SetEventCallback(BIND_EVENT_FUNC(this->OnEvent));
		this->GetInput().SetEventCallback(BIND_EVENT_FUNC(this->OnInputEvent));
	};

	Application::~Application()
	{
		this->window.reset();
	};

	void Application::Initialize()
	{
		// This function gets called by the main loop after the global engine has been initialized.
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
		MainLoop::Stop();
		return true;
	}

	void Application::Tick()
	{
		LogInfo("App", "Tick!");
	}

	void Application::OnInputEvent(Event& event)
	{
		
	}

}

