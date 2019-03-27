#pragma once

#define ENGINE_NUM_FRAMES_SAMPLED (10)

// interval in seconds before updating FPS calculation
#define UPDATE_DELAY (0.25f)

namespace engine
{

class FPSCounter
{

    float frameTimes[ENGINE_NUM_FRAMES_SAMPLED];
    int frameCounter;
    float updateTimer;
    float fps;

    float getAverage();

public:
    FPSCounter();
    void count(float delta);
    float getFPS();
};

} // namespace engine