#include "Header Files/Include.h"

// for OpenGL debugging
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	std::cout << "DEBUG MESSAGE (" << id << "): " << message << "\n";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API: std::cout << "SOURCE: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "SOURCE: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "SOURCE: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "SOURCE: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION: std::cout << "SOURCE: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER: std::cout << "SOURCE: Other";
		break;
	}
	std::cout << "\n";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR: std::cout << "TYPE: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "TYPE: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "TYPE: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY: std::cout << "TYPE: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "TYPE: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER: std::cout << "TYPE: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "TYPE: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP: std::cout << "TYPE: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER: std::cout << "TYPE: Other";
		break;
	}
	std::cout << "\n";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: std::cout << "SEVERITY: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "SEVERITY: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW: std::cout << "SEVERITY: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "SEVERITY: notification";
		break;
	}
	std::cout << "\n\n";
}

Window::Window(int width, int height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		cout << "Could not Initialize the library" << endl;


	m_window = glfwCreateWindow(m_width, m_height, "KamiCarZe", nullptr, nullptr); //glfwGetPrimaryMonitor()
	if (!m_window)
	{
		glfwTerminate();
		cout << "Could not open window" << endl;
	}

	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK)
		printf("Error\n");

	// for enabling OpenGL debugging
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);


	glfwMakeContextCurrent(m_window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/Rowdy-Regular.ttf", 30.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/Rowdy-Regular.ttf", 60.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/outrun_future/outrun-future-Bold.ttf", 40.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/outrun_future/outrun-future-Bold.ttf", 80.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/HEART shapes.ttf", 30.0f)); // HEART font
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/CFLegendsoftheFall-Regular.ttf", 15.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/CFLegendsoftheFall-Regular.ttf", 60.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/thememefont.ttf", 20.0f));	// MEME font
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/Rowdy-Regular.ttf", 90.0f));
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/ghostsmileys.ttf", 30.0f)); // Ghosts
	m_fonts.push_back(io.Fonts->AddFontFromFileTTF("src/IMGUI/Fonts/SugarpunchDEMO.ttf", 60.0f));
	ImGui::StyleColorsDark();

	// Application init
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;


	ImGui_ImplGlfw_InitForOpenGL(m_window, true);

	ImGui_ImplOpenGL3_Init("#version 130");
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

int Window::GetWidht()
{
	return m_width;
}

int Window::GetHeight()
{
	return m_height;
}

void Window::SetWidht(int width)
{
	m_width = width;
}

void Window::SetHeight(int height)
{
	m_height = height;
}
