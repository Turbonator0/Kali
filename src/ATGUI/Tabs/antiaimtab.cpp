#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"

#include "antiaimtab.h"

#include "../../interfaces.h"
#include "../../Utils/xorstring.h"
#include "../../settings.h"
#include "../../Hacks/valvedscheck.h"
#include "../../ImGUI/imgui_internal.h"
#include "../atgui.h"



static void RageAntiAIm()
{
const char* yType[] = {
    "NONE",
    "Static",
    "Balanced",
    "Jitter",
    "Brainfuck Fake Fake AA",
    "JitterRandom",
    "P100 SPIN",
};
const char* pitchType[] = {
    "Up",
    "Down",
	"Zero",
	"Emotion",
	"Fake Zero",
    "Fake Up",
	"Fake Down",
	"Fake Jitter",
};

const char* lbyType[] = {
	"Normal",
	"Opposite",
	"Sway",
};
    const char* RageAntiAimType[] = {
        "Default Rage",
        "Free Stand",
    };
    ImGui::Columns(1, nullptr, false); // Pick Rage Anti Aim type
    {
        ImGui::PushItemWidth(-1);
//        ImGui::Combo(XORSTR("##RageAntiAimType"), (int*)&Settings::AntiAim::RageAntiAim::Type, RageAntiAimType, IM_ARRAYSIZE(RageAntiAimType));
        ImGui::PopItemWidth();
    }

  //  if (Settings::AntiAim::RageAntiAim::Type == RageAntiAimType::FreeStand)
   // {
        ImGui::Columns(2, nullptr, false);
        {
            ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
            ImGui::Text(XORSTR("Yaw"));
            ImGui::Text(XORSTR("Pitch"));

        }
        ImGui::NextColumn();
        {
            ImGui::PushItemWidth(-1);
            ImGui::Combo(XORSTR("##YACTUALTYPE"), (int*)& Settings::AntiAim::Yaw::typeReal, yType, IM_ARRAYSIZE(yType));
            ImGui::Combo(XORSTR("##PITCHTYPE"), (int*)& Settings::AntiAim::pitchtype, pitchType, IM_ARRAYSIZE(pitchType));

            ImGui::PopItemWidth();
        }
   ImGui::Columns(1);
        {
            ImGui::PushItemWidth(-1);

        if(Settings::AntiAim::Yaw::typeReal == AntiAimRealType_Y::Randome || Settings::AntiAim::Yaw::typeReal == AntiAimRealType_Y::JitterSwitch || Settings::AntiAim::Yaw::typeReal == AntiAimRealType_Y::JitterRandom || Settings::AntiAim::Yaw::typeReal == AntiAimRealType_Y::Spin)
            ImGui::SliderFloat(XORSTR("##RealJitterPercentage"), &Settings::AntiAim::RageAntiAim::JitterPercent, 1, 100, "Jitter Ammount : %.0f");
            //ImGui::SliderFloat(XORSTR("##BodyLeanAmmount"), &Settings::AntiAim::RageAntiAim::bodylean, 1, 100, "Body Lean Ammount : %.0f");
        if(Settings::AntiAim::Yaw::typeReal == AntiAimRealType_Y::Static)
            ImGui::SliderFloat(XORSTR("##RealJitterPercentage"), &Settings::AntiAim::RageAntiAim::AntiAimOffset,1,360,"Static Offset : %.0f");

    ImGui::Text(XORSTR("InvertKey"));
    ImGui::SameLine();
    UI::KeyBindButton(&Settings::AntiAim::RageAntiAim::InvertKey);

	ImGui::PopItemWidth();
	}

    ImGui::Spacing(); ImGui::Spacing();
    ImGui::Columns(1, nullptr, false);
    
    ImGui::Checkbox(XORSTR("At Targets (alpha)"), &Settings::AntiAim::RageAntiAim::atTheTarget);
    ImGui::Checkbox(XORSTR("Invert on hurt"), &Settings::AntiAim::RageAntiAim::invertOnHurt);
    ImGui::Checkbox(XORSTR("Legit AA on E"), &Settings::AntiAim::RageAntiAim::legitkey::enabled);

    ImGui::Spacing(); ImGui::Spacing();
 
    ImGui::Columns(2, nullptr, false);
    {

    ImGui::Checkbox(XORSTR("Break LBY"), &Settings::AntiAim::RageAntiAim::lby::enabled);

    }
    ImGui::NextColumn();
    {

    ImGui::Combo(XORSTR("##LbyType"), (int*)&Settings::AntiAim::RageAntiAim::lby::type, lbyType, IM_ARRAYSIZE(lbyType));

    }
    ImGui::Columns(1);
    ImGui::Text(XORSTR("Edging(Alfa)"));
    
    ImGui::Columns(2, nullptr, false);
    {
        ImGui::Checkbox(XORSTR("Enabled"), &Settings::AntiAim::HeadEdge::enabled);
    }
    ImGui::NextColumn();
    {
        ImGui::PushItemWidth(-1);
        ImGui::SliderFloat(XORSTR("##EDGEDISTANCE"), &Settings::AntiAim::HeadEdge::distance, 20, 30, "Distance: %0.0f");
        ImGui::PopItemWidth();
    }
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::Columns(1);

    ImGui::Columns(2, nullptr, false);
    {

//    ImGui::Text(XORSTR("LBY Mode"));

    }
    ImGui::NextColumn();
    {

//    ImGui::Combo(XORSTR("##LbyType"), (int*)&Settings::AntiAim::RageAntiAim::lbym, lbyType, IM_ARRAYSIZE(lbyType));

    }
    /*
    ** Starting of Manual anti aim
    */
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::Columns(1);
    ImGui::Checkbox(XORSTR("##Manual Anti Aim"), &Settings::AntiAim::ManualAntiAim::Enable);
    ImGui::SameLine();
    ImGui::Text(XORSTR("Manual AntiAim"));
    if (Settings::AntiAim::ManualAntiAim::Enable){
        //For player to move right
        ImGui::Text("Align Right");
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        UI::KeyBindButton(&Settings::AntiAim::ManualAntiAim::RightButton);
        ImGui::PopItemWidth();

        // For player Move back
        ImGui::Text("Align Back");
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        UI::KeyBindButton(&Settings::AntiAim::ManualAntiAim::backButton);
        ImGui::PopItemWidth();

        //For player to move left
        ImGui::Text("Align Left");
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        UI::KeyBindButton(&Settings::AntiAim::ManualAntiAim::LeftButton);
        ImGui::PopItemWidth();
        // END
        ImGui::Checkbox(XORSTR("Auto Direction"), &Settings::AntiAim::AutoInvert);

    }
}

static void LegitAntiAim()
{
    const char* LegitAAType[] = {
        "OverWatchProof",
        "FakeLegitAA",
        "Experimental"
    };
    /*
    * part where legit anti aim ui constructed
    */
    // Really nothing much here to add more about
    ImGui::Spacing();
    ImGui::PushItemWidth(-1);
    ImGui::Combo(XORSTR("##LegitAAType"), (int*)&Settings::AntiAim::LegitAntiAim::legitAAtype, LegitAAType, IM_ARRAYSIZE(LegitAAType));
    ImGui::Spacing();
    ImGui::Columns(1, nullptr, false);
    ImGui::Text(XORSTR("InvertKey"));
    ImGui::SameLine();
    UI::KeyBindButton(&Settings::AntiAim::LegitAntiAim::InvertKey); 
    ImGui::Checkbox(XORSTR("Legit freestand"), &Settings::AntiAim::AutoInvert);
    ImGui::Checkbox(XORSTR("Set arms to fake side"), &Settings::AntiAim::arms);

//    ImGui::SliderFloat(XORSTR("##mEES"), &Settings::AntiAim::offsat, -180, 180, "Yaw Desync Angle : %.0f");
//    ImGui::Checkbox(XORSTR("aaoff"), &Settings::AntiAim::aaoff);

    ImGui::PopItemWidth();
    /*
    * End of legit antiaim
    */
}

static void LagacyAntiAim()
{
const char* pitchType[] = {
        "Up",
        "Down",
        "Zero",
        "Fake Zero",
        "Fake Up",
        "Fake Down",
        "Fake Jitter",
};
const char* aaState[] = {
        "Stand",
        "Air",
        "Move",
        "Slow Walk",
        "Fake Duck",
        "Lby Update",
};

    ImGui::Combo(XORSTR("##AASTATES"), (int*) &Settings::AntiAim::RageAntiAim::customaa::aastate, aaState, IM_ARRAYSIZE(aaState));
	switch (Settings::AntiAim::RageAntiAim::customaa::aastate)
	{
	case AAState::STAND:
	    ImGui::SliderInt(XORSTR("##YAWANGLE"), &Settings::AntiAim::RageAntiAim::customaa::standang, -180, 180, "Yaw Desync Angle : %.0f");
	break;

    case AAState::AIR:
        ImGui::SliderInt(XORSTR("##YAWANGLE"), &Settings::AntiAim::RageAntiAim::customaa::airang, -180, 180, "Yaw Desync Angle : %.0f");
    break;

    case AAState::MOVE:
	    ImGui::SliderInt(XORSTR("##YAWANGLE"), &Settings::AntiAim::RageAntiAim::customaa::moveang, -180, 180, "Yaw Desync Angle : %.0f");
	break;

    case AAState::SLOLWWALK:
        ImGui::SliderInt(XORSTR("##YAWANGLE"), &Settings::AntiAim::RageAntiAim::customaa::slowang, -180, 180, "Yaw Desync Angle : %.0f");
    break;

    case AAState::LBY:
	    ImGui::SliderInt(XORSTR("##YAWANGLE"), &Settings::AntiAim::RageAntiAim::customaa::lbyang, -180, 180, "Yaw Desync Angle : %.0f");
	break;

    case AAState::FAKEDUCK:
        ImGui::SliderInt(XORSTR("##YAWANGLE"), &Settings::AntiAim::RageAntiAim::customaa::duckang, -180, 180, "Yaw Desync Angle : %.0f");
    break;
	}
        ImGui::Columns(2, nullptr, false);
        {
            ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
            ImGui::Text(XORSTR("Pitch"));

        }
        ImGui::NextColumn();
        {
            ImGui::PushItemWidth(-1);
            ImGui::Combo(XORSTR("##PITCHTYPE"), (int*)& Settings::AntiAim::pitchtype, pitchType, IM_ARRAYSIZE(pitchType));
            ImGui::PopItemWidth();
        }
        ImGui::Columns(1);
            ImGui::Checkbox(XORSTR("Randomize Yaw"), &Settings::AntiAim::RageAntiAim::customaa::sidemove);
	    ImGui::Checkbox(XORSTR("Micromovements"), &Settings::AntiAim::RageAntiAim::customaa::sidemove);

}
void HvH::RenderTab()
{
    
    static char* AntiAimType[] = {
        "Legit AntiAim",
        "Rage AntiAim",
        "Custom AntiAim",
    }; 
      static char* slowType[] = {
        "Safety",
        "Accuracy",
    }; 
    

    ImGui::Spacing();

    ImGui::Columns(2, nullptr, false);
    {
        switch (Settings::AntiAim::Type::antiaimType)
        {
        case AntiAimType::LegitAntiAim:
           if ( ImGui::Checkbox(XORSTR("##EnableLegitAim"), &Settings::AntiAim::LegitAntiAim::enable) ){
                Settings::AntiAim::RageAntiAim::enable = false;
                Settings::AntiAim::RageAntiAim::customaa::enabled = false;
                }
            break;
        case AntiAimType::RageAntiAim:
            if ( ImGui::Checkbox(XORSTR("##EnableRageAntiAim"), &Settings::AntiAim::RageAntiAim::enable) ){
                Settings::AntiAim::LegitAntiAim::enable = false;
                Settings::AntiAim::RageAntiAim::customaa::enabled = false;
                }
            break;
        case AntiAimType::Lagacy:
            if (ImGui::Checkbox(XORSTR("##ENABLECustomAA"), &Settings::AntiAim::RageAntiAim::customaa::enabled)){
                Settings::AntiAim::LegitAntiAim::enable = false;
                Settings::AntiAim::RageAntiAim::enable = false;
                }
            break;
        default:
            break;
        }
            
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        ImGui::Combo(XORSTR("##AntiAimType"), (int*)&Settings::AntiAim::Type::antiaimType, AntiAimType, IM_ARRAYSIZE(AntiAimType));
        ImGui::PopItemWidth();
        ImGui::BeginChild(XORSTR("HVH1"), ImVec2(0, 0), false);
        {
            switch (Settings::AntiAim::Type::antiaimType)
            {
            case AntiAimType::RageAntiAim:
                RageAntiAIm();
                break;
            case AntiAimType::LegitAntiAim:
                LegitAntiAim();
                break;
            case AntiAimType::Lagacy:
                LagacyAntiAim();
                break;
            default:
                break;
            }
        }
        ImGui::EndChild();
    }
    ImGui::NextColumn();
    {
        ImGui::BeginChild(XORSTR("HVH2"), ImVec2(0, 0), false);
        {
        ImGui::Checkbox(XORSTR("Angle Indicator"), &Settings::AngleIndicator::enabled);
        ImGui::Checkbox(XORSTR("Fake Lag"), &Settings::FakeLag::enabled);
        ImGui::SameLine();
        ImGui::SliderInt(XORSTR("##FAKELAGAMOUNT"), &Settings::FakeLag::value, 0, 15, XORSTR("Amount: %0.f"));
        ImGui::Checkbox(XORSTR("Adaptive Fake Lag"), &Settings::FakeLag::adaptive);
        
        if(Settings::FakeLag::enabled == true)
	    {
            ImGui::Checkbox(XORSTR("Fake Lag Mic Fix"), &Settings::FakeLag::microphone);
            ImGui::Text(XORSTR("MicKey"));
            ImGui::SameLine();
            UI::KeyBindButton(&Settings::FakeLag::microphoneKey);
	    }
//ImGui::Checkbox(XORSTR("Random Fake Lag"), &Settings::AntiAim::randomLag::enabled);
        //   UI::KeyBindButton(&Settings::FakeLag::ckey);

        ImGui::Checkbox(XORSTR("Slow Walk"), &Settings::AntiAim::SlowWalk::enabled);
        if (Settings::AntiAim::SlowWalk::enabled){
            ImGui::SameLine(); 
            UI::KeyBindButton(&Settings::AntiAim::SlowWalk::key);
            ImGui::Combo(XORSTR("##SLOWTYPE"), (int*)& Settings::AntiAim::SlowWalk::mode, slowType, IM_ARRAYSIZE(slowType));
            
            if (Settings::AntiAim::SlowWalk::mode == SlowMode::SAFETY){
                ImGui::SliderFloat(XORSTR("##SlowWalkSPeed"), &Settings::AntiAim::SlowWalk::Speed, 0, 100, XORSTR("Amount: %0.f"));
            }
        }
        ImGui::Checkbox(XORSTR("QuickPeek"), &Settings::Ragebot::quickpeek::enabled);
        
        if(Settings::Ragebot::quickpeek::enabled){
            UI::KeyBindButton(&Settings::Ragebot::quickpeek::key);
        }
        ImGui::Checkbox(XORSTR("FakeDuck"), &Settings::AntiAim::FakeDuck::enabled);
        ImGui::SameLine(); 
        UI::KeyBindButton(&Settings::AntiAim::FakeDuck::fakeDuckKey);
		
	    ImGui::Checkbox(XORSTR("Resolver Override"), &Settings::Resolver::manual);
	    if (Settings::Resolver::manual){
            ImGui::Checkbox(XORSTR("Force Bruteforce"), &Settings::Resolver::forcebrute);
            if (!Settings::Resolver::forcebrute)
            {
                ImGui::SliderFloat(XORSTR("##GOALPITCH"), &Settings::Resolver::Pitch, -89, 89, XORSTR("Amount: %0.f"));
                ImGui::SliderFloat(XORSTR("##GOALFEETYAW"), &Settings::Resolver::goalFeetYaw, 0, 180, XORSTR("Amount: %0.f"));
                ImGui::SliderFloat(XORSTR("##EYEANGLES"), &Settings::Resolver::EyeAngles, -60, 60, XORSTR("Amount: %0.f"));
            }
	    }
        ImGui::Checkbox(XORSTR("Spin on air"), &Settings::AntiAim::airspin::enabled);
        ImGui::Checkbox(XORSTR("Animation Memes"), &Settings::AnimMemes::enabled);
        ImGui::Checkbox(XORSTR("Choke OnShot"), &Settings::AntiAim::ChokeOnShot);

        ImGui::Checkbox(XORSTR("Fake Peek"), &Settings::AntiAim::RageAntiAim::fakepeek);
		if (Settings::AntiAim::RageAntiAim::fakepeek)
            UI::KeyBindButton(&Settings::AntiAim::RageAntiAim::fakeheadkey);

        ImGui::EndChild();
        }
    }
}
