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
    
    DECLARE_STATIC_METHOD(void, Register);
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, UpdateSwings);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, leftCutText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, rightCutText);

    struct Swing {
        float bottomSwing, topSwing;
    };
    std::vector<Swing> leftSwings, rightSwings;

    public:
    void AddSwingRatings(bool swingDirectionUp, bool rightSaber, float preSwing, float postSwing);
)

using OQ = OverswingQounter::Qounter;
