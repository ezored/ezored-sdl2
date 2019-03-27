#include "Timer.hpp"
#include "SDL2/SDL.h"

namespace engine
{

Timer::Timer()
{
    lastTime = SDL_GetTicks();
    delta = 0.0f;
}

void Timer::reset()
{
    lastTime = SDL_GetTicks();
}

void Timer::tick()
{
    float currentTime = SDL_GetTicks();
    delta = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
}

float Timer::getDelta()
{
    return delta;
}

} // namespace engine