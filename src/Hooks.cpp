#include "Hooks.h"

namespace Hooks
{
    void Install() noexcept
    {
        MainUpdate::oldGetEquippedWeight = SKSE::GetTrampoline().write_call<5>(REL::ID(RELOCATION_ID(36994, 38022)).address() + 0xc1, MainUpdate::getEquippedWeight);
        MainUpdate::oldGetSprintStaminaDrain = SKSE::GetTrampoline().write_call<5>(REL::ID(RELOCATION_ID(36994, 38022)).address() + 0xc9, MainUpdate::getSprintStaminaDrain);
        logger::info("Installed main update hook");
        logger::info("");
    }

    void InstallWbnb() noexcept {
        MainUpdate::oldGetEquippedWeight = SKSE::GetTrampoline().write_call<5>(REL::ID(RELOCATION_ID(36994, 38022)).address() + 0xc1, MainUpdate::getEquippedWeightBandB);
        MainUpdate::oldGetSprintStaminaDrain = SKSE::GetTrampoline().write_call<5>(REL::ID(RELOCATION_ID(36994, 38022)).address() + 0xc9, MainUpdate::getSprintStaminaDrain);
        logger::info("Installed main update hook");
        logger::info("");
    }

    static inline float getRate(RE::Actor* a) {
        float totalRate;
        float rate =
            a->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStaminaRate) * 0.01f;  // use the formula from regen_stamina_withwait
        if (rate <= 0.0f) {
            totalRate = 0.0f;
        }
        else {
            totalRate = rate * a->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
        }
        if (totalRate > 0) {
            if (a->IsInCombat()) {
                totalRate *= RE::GameSettingCollection::GetSingleton()->GetSetting("fCombatStaminaRegenRateMult")->GetFloat();
            }
            totalRate *= a->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStaminaRateMult) * 0.01f;
        }
        return totalRate;
    }
    float MainUpdate::getEquippedWeight(RE::Actor* a) {
        if (a == nullptr) {
            return 0;
        }
        float w = oldGetEquippedWeight(a);
        float drain = oldGetSprintStaminaDrain(w, RE::GetSecondsSinceLastFrame());
        float rate = getRate(a) * RE::GetSecondsSinceLastFrame();
        return std::max(0.0f, -rate + drain);
    }

    float MainUpdate::getEquippedWeightBandB(RE::Actor* a) {
        if (a == nullptr) {
            return 0;
        }
        float t = RE::GetSecondsSinceLastFrame();
        float rate = getRate(a) * t;
        return std::max(-8.0f * t, -rate);
    }

    float MainUpdate::getSprintStaminaDrain(float weight, float) {
        return weight;
    }
} // namespace Hooks
