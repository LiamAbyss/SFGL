#include "Animation.h"

namespace sfg
{

    void Animation::setTextureRect()
    {
        int width = spritesheet->getSize().x / framesInSpritesheet;
        sprite->setTextureRect(sf::IntRect(width * (currentFrame + static_cast<int>(flipped)), 0, width * (flipped ? -1 : 1), spritesheet->getSize().y));
    }

    uint32_t Animation::getFramerate() const
    {
        return framerate;
    }

    void Animation::setFramerate(uint32_t framerate)
    {
        this->framerate = framerate;
    }

    size_t Animation::getCurrentFrame() const
    {
        return currentFrame;
    }

    void Animation::setCurrentFrame(size_t currentFrame)
    {
        this->currentFrame = currentFrame;
    }

    bool Animation::getRepeat() const
    {
        return repeat;
    }

    void Animation::setRepeat(bool repeat)
    {
        this->repeat = repeat;
    }

    bool Animation::isFlipped() const
    {
        return flipped;
    }

    void Animation::flip(bool flipped)
    {
        this->flipped = flipped;
    }

    void Animation::setSpritesheet()
    {
        sprite->setTexture(*spritesheet);
        setTextureRect();
    }

    void Animation::reset()
    {
        currentFrame = start;
        setSpritesheet();
    }

    void Animation::nextFrame(const sf::Time& dt)
    {
        if (currentFrame == end && !repeat) return;

        sinceLastFrame += dt;

        while (sinceLastFrame >= frameDuration)
        {
            if (currentFrame == end)
                currentFrame = start;
            else
                currentFrame++;

            sinceLastFrame -= frameDuration;
        }

        setTextureRect();
    }

    bool Animation::isFinished() const
    {
        return currentFrame == end && !repeat;
    }

    float Animation::getProgress() const
    {
        float span = end - start;
        float cursor = currentFrame - start;
        return cursor / span;
    }

    Animation Animation::build(GTexture spritesheet, size_t framesInSpritesheet, size_t start, size_t end, uint32_t framerate, sf::Sprite& sprite, bool repeat)
    {
        Animation anim;
        anim.spritesheet = spritesheet;
        anim.framesInSpritesheet = framesInSpritesheet;
        anim.start = start;
        anim.end = end;
        anim.currentFrame = start;
        anim.framerate = framerate < 1 ? 1 : framerate;
        anim.frameDuration = sf::seconds(1.F / static_cast<float>(anim.framerate));
        anim.sprite = &sprite;
        anim.repeat = repeat;
        return anim;
    }

}