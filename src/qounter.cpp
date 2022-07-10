#include "main.hpp"
#include "qounter.hpp"

DEFINE_TYPE(OverswingQounter, Qounter);

using namespace QountersMinus;

bool OQ::Enabled = false;
int OQ::Position = static_cast<int>(QounterPosition::AboveMultiplier);
float OQ::Distance = 1.0f;
int OQ::DecimalPlaces = 0;
float OQ::TargetOverswing = 8;
float OQ::OverswingWarningStrength = 12;
float OQ::UnderswingWarningStrength = 8;

void OQ::Register() {
    QounterRegistry::Register<OQ>("Overswing", "Overswing Qounter", "OverswingConfig");
    QounterRegistry::RegisterConfig<OQ>({
        .ptr = &DecimalPlaces,
        .field = "DecimalPlaces",
        .displayName = "Decimal Places",
        .helpText = "How many decimals should be shown on the overswings?",
        .type = QounterRegistry::ConfigType::Int,
        .intMin = 0,
        .intMax = 4,
    });
    QounterRegistry::RegisterConfig<OQ>({
        .ptr = &TargetOverswing,
        .field = "TargetOverswing",
        .displayName = "Target Overswing",
        .helpText = "What overswing should show as the correct amount?",
        .type = QounterRegistry::ConfigType::Float,
        .floatMin = -30,
        .floatMax = 30,
    });
    QounterRegistry::RegisterConfig<OQ>({
        .ptr = &OverswingWarningStrength,
        .field = "OverswingWarningStrength",
        .displayName = "Overswing Warning Strength",
        .helpText = "How much above the target overswing should be shown as completely yellow?",
        .type = QounterRegistry::ConfigType::Float,
        .floatMin = 0,
        .floatMax = 20,
    });
    QounterRegistry::RegisterConfig<OQ>({
        .ptr = &UnderswingWarningStrength,
        .field = "UnderswingWarningStrength",
        .displayName = "Underswing Warning Strength",
        .helpText = "How much below the target overswing should be shown as completely red?",
        .type = QounterRegistry::ConfigType::Float,
        .floatMin = 0,
        .floatMax = 20,
    });
}

void OQ::Start() {
    CreateBasicTitle("Overswing");

    auto defaultText = FormatNumber(0, false);
    defaultText = defaultText + "\n" + defaultText;

    float xOffset = 20 + (9 * DecimalPlaces);

    leftCutText = CreateBasicText(defaultText);
    leftCutText->set_fontSize(28);
    leftCutText->set_lineSpacing(-40.0f);
    leftCutText->get_rectTransform()->set_anchoredPosition({xOffset * -1.0f, -45});

    rightCutText = CreateBasicText(defaultText);
    rightCutText->set_fontSize(28);
    rightCutText->set_lineSpacing(-40.0f);
    rightCutText->get_rectTransform()->set_anchoredPosition({xOffset, -45});
}

void OQ::UpdateSwings() {
    if(!rightSwings.empty()) {
        float rightBottomTotal = 0, rightTopTotal = 0;
        for(auto& swing : rightSwings) {
            rightBottomTotal += swing.bottomSwing;
            rightTopTotal += swing.topSwing;
        }
        float rightBottomPct = rightBottomTotal / rightSwings.size();
        float rightTopPct = rightTopTotal / rightSwings.size();
        rightCutText->set_text(FormatNumber(rightTopPct) + "\n" + FormatNumber(rightBottomPct));
    }
    if(!leftSwings.empty()) {
        float leftBottomTotal = 0, leftTopTotal = 0;
        for(auto& swing : leftSwings) {
            leftBottomTotal += swing.bottomSwing;
            leftTopTotal += swing.topSwing;
        }
        float lefBottomPct = leftBottomTotal / leftSwings.size();
        float leftTopPct = leftTopTotal / leftSwings.size();
        leftCutText->set_text(FormatNumber(leftTopPct) + "\n" + FormatNumber(lefBottomPct));
    }
}

void OQ::AddSwingRatings(bool swingDirectionUp, bool rightSaber, float preSwing, float postSwing) {
    auto swing = swingDirectionUp ? Swing{preSwing - 70, postSwing - 30} : Swing{postSwing - 30, preSwing - 70};
    if(rightSaber)
        rightSwings.push_back(swing);
    else
        leftSwings.push_back(swing);
    UpdateSwings();
}
