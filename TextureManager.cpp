#include "TextureManager.h"
#include "Engine.h"
TextureManager* TextureManager::s_Instance=nullptr;
bool TextureManager::Load(std::string id, std::string filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
{
    SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
    return false;
}
SDL_Log("Loaded texture: %s", filename.c_str());

    SDL_Texture* Texture = SDL_CreateTextureFromSurface(
        Engine::GetInstance()->GetRenderer(),
        surface
    );
    if (!Texture) {
    SDL_Log("Error creating texture: %s", SDL_GetError());
    return false;
}

    SDL_FreeSurface(surface);

    if (Texture == nullptr)
    {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return false;
    }

    m_TextureMap[id] = Texture;
    SDL_Log("Successfully loaded texture with ID: %s", id.c_str());
    return true;
}


void TextureManager::Drop(std::string id)
{
    SDL_DestroyTexture(m_TextureMap[id]);
    m_TextureMap.erase(id);
}

void TextureManager::Clean()
{
     std::map<std::string, SDL_Texture*>::iterator it;
     for(it=m_TextureMap.begin();it!=m_TextureMap.end();it++)
        SDL_DestroyTexture(it->second);
     m_TextureMap.clear();
     SDL_Log("texture map cleaned");
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip)
{
    SDL_Rect dstRect={x,y,tileSize,tileSize};
    SDL_Rect srcRect={tileSize*frame,tileSize*(row),tileSize,tileSize};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(),m_TextureMap[tilesetID],&srcRect,&dstRect,0,0,flip);
}
void TextureManager::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
    SDL_Rect srcRect={0,0,width,height};
    SDL_Rect dstRect={x,y,width,height};
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(),m_TextureMap[id],&srcRect,&dstRect,0,nullptr,flip);
}

void TextureManager::DrawFrame(
    std::string id,
    int x,
    int y,
    int width,
    int height,
    int row,
    int frame,
    SDL_RendererFlip flip
) {
    //SDL_Log("Drawing texture ID: %s at (%d, %d), frame: %d, row: %d", id.c_str(), x, y, frame, row);

    SDL_Rect srcRect = {frame * width, row * height, width, height};
    SDL_Rect destRect = {x, y, width, height};

    //SDL_Log("srcRect: (%d, %d, %d, %d)", srcRect.x, srcRect.y, srcRect.w, srcRect.h);
    //SDL_Log("destRect: (%d, %d, %d, %d)", destRect.x, destRect.y, destRect.w, destRect.h);


    if (SDL_RenderCopyEx(
            Engine::GetInstance()->GetRenderer(),
            m_TextureMap[id],
            &srcRect,
            &destRect,
            0,
            nullptr,
            flip
        ) != 0) {
        SDL_Log("Render failed: %s", SDL_GetError());
    }
    SDL_Log("Drawing texture ID: %s", id.c_str());
if (m_TextureMap.find(id) == m_TextureMap.end())
{
    SDL_Log("Texture ID not found: %s", id.c_str());
    return;
}

}

