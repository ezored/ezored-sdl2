#include "FPSCounter.hpp"

namespace engine
{

FPSCounter::FPSCounter()
{
    frameCounter = 0;
    updateTimer = 0;
    fps = 0;
}

float FPSCounter::getAverage()
{
    float sum = 0;

    for (int i = 0; i < ENGINE_NUM_FRAMES_SAMPLED; i++)
    {
        sum += frameTimes[i];
    }

    return 1 / (sum / ENGINE_NUM_FRAMES_SAMPLED);
}

void FPSCounter::count(float delta)
{
    frameTimes[frameCounter] = delta;
    frameCounter = (frameCounter + 1) % ENGINE_NUM_FRAMES_SAMPLED;
    updateTimer += delta;

    if (updateTimer > UPDATE_DELAY)
    {
        updateTimer = 0;
        fps = getAverage();
    }
}

float FPSCounter::getFPS()
{
    return fps;
}

} // namespace engine