#include "main.hpp"
#include "qounter.hpp"

#include "overswing/shared/callback.hpp"

#include "GlobalNamespace/ScoreModel_NoteScoreDefinition.hpp"
#include "GlobalNamespace/SaberSwingRatingCounter.hpp"
#include "GlobalNamespace/ISaberMovementData.hpp"
#include "GlobalNamespace/BladeMovementDataElement.hpp"

using namespace GlobalNamespace;

#include <sstream>
#include <iomanip>
#include <math.h>

static ModInfo modInfo;

int LerpColor(int hexFrom, int hexTo, float amount) {
    if(amount < 0)
        amount = 0;
    if(amount > 1)
        amount = 1;

    int ar = hexFrom >> 16,
        ag = hexFrom >> 8 & 0xff,
        ab = hexFrom & 0xff,

        br = hexTo >> 16,
        bg = hexTo >> 8 & 0xff,
        bb = hexTo & 0xff,

        rr = ar + amount * (br - ar),
        rg = ag + amount * (bg - ag),
        rb = ab + amount * (bb - ab);

    return (rr << 16) + (rg << 8) + (rb | 0);
}

std::string FormatNumber(float swing, bool color) {
    std::stringstream stream;
    if(color) {
        float diff = swing - OQ::TargetOverswing;
        int warnColor = diff > 0 ? 0xFFFF00 : 0xFF0000;
        float div = diff > 0 ? OQ::OverswingWarningStrength : OQ::UnderswingWarningStrength * -1;
        stream << "<color=#" << std::setfill('0') << std::setw(6) << std::hex << LerpColor(0xFFFFFF, warnColor, diff / div) << ">";
    }
    stream << std::fixed << std::setprecision(OQ::DecimalPlaces) << swing << "°";
    if(color)
        stream << "</color>";
    return stream.str();
}

// true for swinging up
bool FindSwingDirection(CutScoreBuffer* buffer) {
    auto lastElement = buffer->saberSwingRatingCounter->saberMovementData->get_lastAddedData();
    // the segment normal is the normal of the swing plane
    // since the swing plane is four points (first and second saber top and bottom pos) and we need three instead,
    // the plane is calculated by the top and bottom of the second saber position and the midpoint of the first
    // since we have the normal, we can tell by whether it is pointing left or right to know if the swing is up or down
    return lastElement.segmentNormal.x > 0;
}

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    overswingCallbacks.addCallback([](SwingInfo info) {
        auto qounter = (OQ*) QountersMinus::QounterRegistry::registry[{"OverswingQounter", "Qounter"}].instance;
        if(qounter) {
            float maxPre = info.scoreBuffer->noteScoreDefinition->maxBeforeCutScore;
            float maxPost = info.scoreBuffer->noteScoreDefinition->maxAfterCutScore;
            qounter->AddSwingRatings(FindSwingDirection(info.scoreBuffer), info.rightSaber, info.preSwing * maxPre, info.postSwing * maxPost);
        }
    });

    getLogger().info("Completed setup!");
}

extern "C" void load() {
    il2cpp_functions::Init();

    Modloader::requireMod("Qounters-");
    getLogger().info("Registering qounter!");
    OQ::Register();
}
