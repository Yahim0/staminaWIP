#pragma once

namespace Hooks
{
    void Install() noexcept;

    void InstallWbnb() noexcept;

    class MainUpdate
    {
    public:
        static float getEquippedWeight(RE::Actor* a);
        static float getEquippedWeightBandB(RE::Actor* a);
        static float getSprintStaminaDrain(float weight, float);

        static inline REL::Relocation<decltype(&getEquippedWeight)> oldGetEquippedWeight;
        static inline REL::Relocation<decltype(&getSprintStaminaDrain)> oldGetSprintStaminaDrain;
    };
} // namespace Hooks
