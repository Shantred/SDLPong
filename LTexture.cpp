#include "LTexture.h"

// Wrapper for SDL_Texture that serves as a superclass for all of our textures
LTexture::LTexture()
{
    // Initialize texture to prevent errors rendering if we do something wrong trying to load the texture.
	texture = NULL;
	width = 0;
	height = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path) {

	// Clear any existing textures. Allows us to reuse an instance of LTexture if we need to or swap out the texture.
	free();

	SDL_Texture* newTexture = NULL;

    // Load image from provided path as a surface
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s SDL_image error %s\n", path.c_str(), IMG_GetError());
	} else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        
        // Create a texture from our loaded surface
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s, SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			width = loadedSurface->w;
			height = loadedSurface->h;

		}

		// Unload the surface, it's no longer needed now that we have a texture.
		SDL_FreeSurface(loadedSurface);
	}

	texture = newTexture;
	return newTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    free();

    // Create a surface out of a TTF_Font and a string. 
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        // Just like from an image, we can convert the surface to a texture
        texture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);
        if (texture == NULL) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            width = textSurface->w;
            height = textSurface->h;
        }

        // Unload surface
        SDL_FreeSurface(textSurface);
    }

    return texture != NULL;
}

void LTexture::free()
{
    SDL_DestroyTexture(texture);
    texture = NULL;
    width = 0;
    height = 0;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	// Modulate texture rgb
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	// Set blending function
	SDL_SetTextureBlendMode(texture, blending);
}
		
void LTexture::setAlpha(Uint8 alpha)
{
	// Modulate texture alpha
	SDL_SetTextureAlphaMod(texture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	// Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	// Set clip rendering dimensions
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// Render to screen
	SDL_RenderCopyEx(gameRenderer, texture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return width;
}

int LTexture::getHeight()
{
	return height;
}