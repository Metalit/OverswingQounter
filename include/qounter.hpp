#pragma once

#include "qounters-minus/shared/QounterRegistry.hpp"

DECLARE_CLASS_CUSTOM(OverswingQounter, Qounter, QountersMinus::Qounter,
    static bool Enabled;
    static int Position;
    static float Distance;
    static int DecimalPlaces;
    static float TargetOverswing;
    static float OverswingWarningStrength;
    static float UnderswingWarningStrength;
    static int SwingTimeTolerance;

    DECLARE_DEFAULT_CTOR();
    DECLARE_STATIC_METHOD(void, Register);
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, UpdateSwings);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, leftCutText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, rightCutText);

    private:
    struct Swing {
        float bottomSwing, topSwing;
        float& preSwing(bool swingDirectionUp) { return swingDirectionUp ? bottomSwing : topSwing; }
        float& postSwing(bool swingDirectionUp) { return swingDirectionUp ? topSwing : bottomSwing; }
    };
    std::vector<Swing> leftSwings, rightSwings;
    bool lastLeftSwingDirection, lastRightSwingDirection;
    long lastLeftSwingTime = 0, lastRightSwingTime = 0;

    void handleConnectedSwing(bool swingDirectionUp, bool rightSaber, Swing& swing);

    public:
    void AddSwingRatings(bool swingDirectionUp, bool rightSaber, float preSwing, float postSwing);
)

using OQ = OverswingQounter::Qounter;
