#include "Engine.h"
#include "TextureManager.h"
#include "Warrior.h"
#include "Input.h"
#include "SDL.h"
#include "Timer.h"
#include "MapParser.h"
#include<iostream>

Engine* Engine::s_Instance= nullptr;
Warrior* player=nullptr;

bool Engine::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Failed to initialize SDL or SDL_image: %s", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow(
        "Soft Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    if (m_Window == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(
        m_Window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (m_Renderer == nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    if(!MapParser::GetInstance()->Load()){
        std::cout<<"Failed to Load map"<<std::endl;
    }

m_LevelMap = MapParser::GetInstance()->GetMaps("MAP");
if (!m_LevelMap) {
    std::cout << "Error: Map not loaded correctly, m_LevelMap is null!" << std::endl;
    return false; // Optionally stop the engine from running.
}
    TextureManager::GetInstance()->Load("player", "assets/Idle.png");
    TextureManager::GetInstance()->Load("player_run", "assets/Run.png");
    player = new Warrior(new Properties("player", 100, 200, 200, 200));
    return m_IsRunning = true;
}


bool Engine::Clean()
{
    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

bool Engine::Quit()
{
    return m_IsRunning=false;
}

void Engine::Update() {
    float dt = Timer::GetInstance()->GetDeltatime();
    if (m_LevelMap) {
        m_LevelMap->Update();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: Map not loaded correctly, m_LevelMap is null!");
    }
    if (player) {
        player->Update(dt);
    }
}
void Engine::Render() {
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    if (m_LevelMap) {
        m_LevelMap->Render();
    }
    if (player) {
        player->Draw();
    }
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events()
{
    Input::GetInstance()->listen();
}
