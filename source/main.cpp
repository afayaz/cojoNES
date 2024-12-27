#include <memory>
#include <algorithm>

#include <spdlog/spdlog.h>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_dialog.h>
#undef SDL_MAIN_HANDLED

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "CPU.hpp"
#include "Memory.hpp"
#include "System.hpp"
#include "Cartridge.hpp"

static bool shouldOpenROM = false;
static std::string romPath;

void FileDialogCallback(void* userdata, const char* const* filelist, int filter)
{
	if (*filelist)
	{
		shouldOpenROM = true;
		romPath = *filelist;

		SPDLOG_INFO("User selected ROM file \"{}\"", romPath);
	}
	else
	{
		SPDLOG_INFO("User cancelled ROM selection.");
	}
}

int main(int argc, char** argv)
{
	// NES is 256x240, extra space is for debug tools.
	constexpr int kScreenWidth = 640;
	constexpr int kScreenHeight = 480;

	std::shared_ptr<CPU>    cpu = std::make_shared<CPU>();
	std::shared_ptr<Memory> memory = std::make_shared<Memory>();
	std::shared_ptr<Cartridge> cart = std::make_shared<Cartridge>();

	std::shared_ptr<System> system;

	if (argc >= 2)
	{
		romPath = argv[1];
		shouldOpenROM = true;
		SPDLOG_INFO("Auto loading ROM file \"{}\"", romPath);
	}
	else
	{
		SPDLOG_INFO("No ROM file specified.");
	}

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_INPUT_FOCUS;
		SDL_Window* window = SDL_CreateWindow("cojoNES", kScreenWidth, kScreenHeight, windowFlags);
		if (!window)
		{
			SPDLOG_ERROR("Failed to create SDL_Window! Error code: {}", SDL_GetError());
			return 1;
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
		SDL_SetRenderVSync(renderer, 1);
		if (!renderer)
		{
			SPDLOG_ERROR("Failed to create SDL_Renderer! Error code: {}", SDL_GetError());
			return 1;
		}
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_ShowWindow(window);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer3_Init(renderer);

		bool running = false;

		// Hack to get window to stay up
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				ImGui_ImplSDL3_ProcessEvent(&e);

				if (e.type == SDL_EVENT_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_EVENT_KEY_DOWN)
				{
					switch (e.key.scancode)
					{
						// Nothing yet...
					}
				}
			}

			// Start the Dear ImGui frame
			ImGui_ImplSDLRenderer3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();

			{
				ImGui::Begin("ROM Info");

				if (ImGui::Button("Load ROM"))
				{
					SDL_ShowOpenFileDialog(FileDialogCallback, nullptr, window, nullptr, 0, nullptr, false);
				}

				size_t pos = romPath.find_last_of("\\/");
				std::string filename = romPath.substr(pos+1);
				ImGui::Text("%s", filename.c_str());

				ImGui::Separator();

				NESHeader romHeader = cart->GetHeader();
				ImGui::Text("Header version: %s", HeaderVersionToString(romHeader.version));
				ImGui::Text("prgSize: %d", romHeader.prgSize);
				ImGui::Text("chrSize: %d", romHeader.chrSize);
				ImGui::Text("isHMirrored: %d", romHeader.isHMirrored);
				ImGui::Text("hasBattery: %d", romHeader.hasBattery);
				ImGui::Text("hasTrainer: %d", romHeader.hasTrainer);
				ImGui::Text("Mapper: %d", romHeader.mapper);

				ImGui::End();
			}

			if (shouldOpenROM)
			{
				shouldOpenROM = false;
				bool isRomValid = cart->Load(romPath);

				if (isRomValid)
				{
					// Initialise system now that ROM is loaded.
					system = std::make_shared<System>(cpu, memory, cart);
					system->Reset();
					running = true;
				}
				else
				{
					SPDLOG_ERROR("File \"{}\" is not a valid NES ROM.", romPath);
					running = false;
				}
			}

			{
				static int memStart = 0x8000;

				ImGui::Begin("Memory");
				ImGui::InputInt("Address", &memStart, 0x10, 0x100, ImGuiInputTextFlags_CharsHexadecimal);

				memStart = std::clamp(memStart, 0x00, 0x10000 - 0x100);

				memStart = memStart - memStart % 0x10;

				for (uint16_t i = static_cast<uint16_t>(memStart); i < static_cast<uint16_t>(memStart + 0x100); ++i)
				{
					uint8_t val = system ? system->Read(i) : memory->Read(i);

					if (i % 0x10 == 0x00)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextSelectedBg, 1.0f));
						ImGui::Text("%04X", i);
						ImGui::PopStyleColor();
						ImGui::SameLine();
					}

					ImGui::Text("%02X", val);
					ImGui::SameLine();

					if (i % 0x10 == 0x0F)
					{
						ImGui::NewLine();
					}
				}

				ImGui::End();
			}

			static bool stepMode = false;
			{
				ImGui::Begin("CPU");

				ImGui::Checkbox("Step mode", &stepMode);
				if (ImGui::Button(stepMode ? "Step" : "Run"))
				{
					running = true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset") && system)
				{
					system->Reset();
				}

				CPURegisters r = cpu->GetRegisters();

				ImGui::Text("Opcode: %s", OpcodeToString(cpu->GetCurrentOpcode()));

				CPU::DecodedOperand operand = cpu->GetCurrentOperand();
				ImGui::Text("Operand: %s %04X", operand.operandType == CPU::OT_Address ? "Address" : "Value", operand.operand);

				ImGui::Separator();

				ImGui::Text("PC: %4X", r.PC); ImGui::SameLine(); ImGui::Text("SP: %2X", r.SP); ImGui::SameLine(); ImGui::Text("ACC: %2X", r.ACC);
				ImGui::Text("IX:   %2X", r.IX); ImGui::SameLine(); ImGui::Text("IY: %2X", r.IY);

				ImGui::Separator();

				ImGui::Text("C: %d", cpu->GetProcessorStatus(PS_CarryFlag));
				ImGui::SameLine();
				ImGui::Text("Z: %d", cpu->GetProcessorStatus(PS_ZeroFlag));
				ImGui::SameLine();
				ImGui::Text("I: %d", cpu->GetProcessorStatus(PS_InterruptDisable));
				ImGui::SameLine();
				ImGui::Text("D: %d", cpu->GetProcessorStatus(PS_DecimalMode));

				ImGui::Text("B: %d", cpu->GetProcessorStatus(PS_BreakCommand));
				ImGui::SameLine();
				ImGui::Text("-: %d", cpu->GetProcessorStatus(PS_Ignored));
				ImGui::SameLine();
				ImGui::Text("V: %d", cpu->GetProcessorStatus(PS_OverflowFlag));
				ImGui::SameLine();
				ImGui::Text("N: %d", cpu->GetProcessorStatus(PS_NegativeFlag));

				ImGui::End();
			}


			if (running && system)
			{
				running = system->Process();
				if (stepMode)
				{
					running = false;
				}
			}

			// Rendering
			ImVec4 clearColor = ImVec4(0.1f, 0.4f, 0.8f, 1.00f);

			ImGui::Render();
			SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
			SDL_SetRenderDrawColorFloat(renderer, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
			SDL_RenderClear(renderer);
			ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
			SDL_RenderPresent(renderer);
		}

		// Cleanup
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	else
	{
		SPDLOG_ERROR("Failed to initialise SDL! Error code: {}", SDL_GetError());
		return 1;
	}

	return 0;
}
