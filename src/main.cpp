#include <SDL2/SDL.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Hello SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const int windowWidth = 800; // Keep window width in one place for texture sizing.
    const int windowHeight = 600; // Keep window height in one place for texture sizing.

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    ); // Enable accelerated rendering, vsync, and render-to-texture support.
    if (renderer == nullptr) {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* imageSurface = SDL_CreateRGBSurfaceWithFormat(0, 220, 220, 32, SDL_PIXELFORMAT_RGBA32); // Create a software surface that will become our image.
    if (imageSurface == nullptr) {
        std::fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_FillRect(imageSurface, nullptr, SDL_MapRGBA(imageSurface->format, 242, 163, 101, 255)); // Fill the image background with a warm color.
    SDL_Rect stripe1{20, 20, 180, 40}; // Define a horizontal stripe near the top.
    SDL_Rect stripe2{20, 90, 180, 40}; // Define a horizontal stripe in the middle.
    SDL_Rect stripe3{20, 160, 180, 40}; // Define a horizontal stripe near the bottom.
    SDL_FillRect(imageSurface, &stripe1, SDL_MapRGBA(imageSurface->format, 255, 244, 214, 255)); // Draw top stripe to give visible orientation.
    SDL_FillRect(imageSurface, &stripe2, SDL_MapRGBA(imageSurface->format, 24, 32, 48, 255)); // Draw middle stripe for contrast while rotating.
    SDL_FillRect(imageSurface, &stripe3, SDL_MapRGBA(imageSurface->format, 255, 244, 214, 255)); // Draw bottom stripe to complete the pattern.

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface); // Upload the generated surface to GPU as a texture.
    SDL_FreeSurface(imageSurface); // Free the CPU surface now that texture upload is complete.
    if (imageTexture == nullptr) {
        std::fprintf(stderr, "SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* backBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight); // Create an off-screen render target for explicit double buffering.
    if (backBuffer == nullptr) {
        std::fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyTexture(imageTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    double angle = 0.0; // Track current rotation angle in degrees.
    Uint32 previousTicks = SDL_GetTicks(); // Capture initial time for frame-to-frame delta.

    bool running = true;
    while (running) {
        Uint32 currentTicks = SDL_GetTicks(); // Read current time at start of this frame.
        double deltaSeconds = static_cast<double>(currentTicks - previousTicks) / 1000.0; // Convert elapsed milliseconds to seconds.
        previousTicks = currentTicks; // Store current time for next frame delta calculation.

        angle += 120.0 * deltaSeconds; // Advance rotation by 120 degrees per second.
        if (angle >= 360.0) {
            angle -= 360.0; // Keep angle bounded to avoid unbounded growth.
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_Rect dstRect{290, 190, 220, 220}; // Place the rotating image near the center of the window.
        SDL_Point center{dstRect.w / 2, dstRect.h / 2}; // Rotate around the image center instead of top-left.

        SDL_SetRenderTarget(renderer, backBuffer); // Render this frame to the off-screen back buffer first.
        SDL_SetRenderDrawColor(renderer, 24, 32, 48, 255); // Set frame background color.
        SDL_RenderClear(renderer); // Clear back buffer with the background color.

        SDL_RenderCopyEx(renderer, imageTexture, nullptr, &dstRect, angle, &center, SDL_FLIP_NONE); // Draw the texture rotated by current angle.

        //SDL_SetRenderDrawColor(renderer, 255, 244, 214, 255); // Choose a light accent color for an outline.
        //SDL_RenderDrawRect(renderer, &dstRect); // Draw an outline around the rotating image bounds.

        SDL_SetRenderTarget(renderer, nullptr); // Switch rendering back to the window framebuffer.
        SDL_RenderCopy(renderer, backBuffer, nullptr, nullptr); // Copy completed back buffer to the window in one operation.

        SDL_RenderPresent(renderer); // Present the completed frame (paired with vsync for smooth output).
    }

    SDL_DestroyTexture(backBuffer);
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
