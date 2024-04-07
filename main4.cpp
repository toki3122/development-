#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
using namespace std;
SDL_Texture *playerTex;
class Game
{
    bool isRunning=false;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int cnt=0;
public:
    Game();
    ~Game();
    void init(const char *title,int xpos,int ypos,int width,int height,bool fullscreen);
    void update();
    void render();
    void clean();
    void handleEvents();
    bool running()
    {
        return isRunning;
    }
};
Game::Game()
{}
Game::~Game()
{}
void Game::init(const char *title,int xpos,int ypos,int width,int height,bool fullscreen)
{
    int flags=0;
    if(fullscreen)
    {
        flags=SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
    window=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
    renderer=SDL_CreateRenderer(window,-1,0);
    if(renderer)
    {
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
    }
    isRunning=true;
    }
    SDL_Surface *tmpSurface=IMG_Load("assets/player.png");
    playerTex=SDL_CreateTextureFromSurface(renderer,tmpSurface);
    SDL_FreeSurface(tmpSurface);
}
void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
        isRunning=false;
        break;
    default:
        break;
    }
}
void Game::update()
{
    cnt++;
    cout<<cnt<<endl;
}
void Game::render()
{
    SDL_RenderClear(renderer);
    //here we add stuff to render
    SDL_RenderCopy(renderer,playerTex,NULL,NULL);
    SDL_RenderPresent(renderer);
}
void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout<<"game cleaned!.."<<endl;
}
Game *game=nullptr;
int main(int argc, char* argv[])
{
    game=new Game();
    game->init("BirchEngine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,false);

    while(game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }
    game->clean();
    return 0;
}
