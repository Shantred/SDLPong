# SDLPong
A simple Pong clone made using C++, SDL.

This Pong clone was made as a way to test my knowledge of C++. SDL was chosen to provide simple access to the window, input and graphics rendering.

All logic for the game was recreated from a visual reference to the original game. All logic, including image rendering, collision detection, etc was written by me without any reference code or snippets.

## Compiling

There's a macOS release and x64 release for windows you can find [here](https://github.com/Shantred/SDLPong/releases). Alternatively,

This repo contains both a VS2019 solution and a g++ based Makefile. You'll likely need to modify the build configuration/make your own for VS2019 with your local links to SDL2, SDL2_image, SDL2_ttf. Once your libraries/includes are set up, the build should do the rest for you, including copying over assets/dlls to the release folder. You may also need to download the latest VC++ Redistributables, which you can find [here](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)

For compiling on mac (I used catalina), you'll need g++ and to have SDL2, SDL_image, SDL_ttf installed locally. Once that is done, simply run `make game` from the root directory. Once it is built, you can run it with `./play`.

## Controls
**Player 1** Move with W and S, launch ball with Space.

**Player 2** Move with Up and Down arrow keys, launch ball with space.

Press "R" to restart after a game over.
