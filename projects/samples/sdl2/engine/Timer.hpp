#pragma once

namespace engine
{

class Timer
{
private:
    unsigned int lastTime;
    float delta;

    int currentFPS;
    int averageFPS;

public:
    Timer();

    void reset();
    void tick();

    float getDelta();
};

} // namespace engine