#include "legitbottab.h"

#include "../../SDK/definitions.h"
#include "../../settings.h"
#include "../../Utils/xorstring.h"
#include "../../ImGUI/imgui_internal.h"
#include "../atgui.h"

#pragma GCC diagnostic ignored "-Wformat-security"

static ItemDefinitionIndex currentWeapon = ItemDefinitionIndex::INVALID;
static bool enabled = false;
static bool silent = false;
static bool friendly = false;
static bool closestBone = true;
inline bool desiredBones[] = {
    true, true, true, true, true, true, true,	   // center mass
    false, false, false, false, false, false, false, // left arm
    false, false, false, false, false, false, false, // right arm
    false, false, false, false, false,			   // left leg
    false, false, false, false, false			   // right leg
};
static bool engageLock = false;
static bool engageLockTR = false; // Target Reacquisition
static int engageLockTTR = 700;   // Time to Target Reacquisition ( in ms )
static Bone bone = CONST_BONE_HEAD;
static ButtonCode_t aimkey = ButtonCode_t::MOUSE_MIDDLE;
static bool aimkeyOnly = false;
static bool smoothEnabled = false;
static float smoothValue = 0.5f;
static bool courseRandomizationEnabled = false;
static bool doAimAfterXShotsEnabled = false;
static float courseRandomizationValue = 2.0f;
static float doAimAfterXShotsValue = 0.0f;
static SmoothType smoothType = SmoothType::SLOW_END;
static bool smoothSaltEnabled = false;
static float smoothSaltMultiplier = 0.0f;
static bool errorMarginEnabled = false;
static float errorMarginValue = 0.0f;
static bool autoAimEnabled = false;
static float autoAimValue = 180.0f;
static bool aimStepEnabled = false;
static float aimStepMin = 25.0f;
static float aimStepMax = 35.0f;
static bool rcsEnabled = false;
static bool rcsAlwaysOn = false;
static float rcsAmountX = 2.0f;
static float rcsAmountY = 2.0f;
static bool autoPistolEnabled = false;
static bool autoShootEnabled = false;
static bool autoScopeEnabled = false;
static bool noShootEnabled = false;
static bool ignoreJumpEnabled = false;
static bool ignoreEnemyJumpEnabled = false;
static bool smokeCheck = false;
static bool flashCheck = false;
static bool spreadLimitEnabled = false;
static float spreadLimit = 0.1f;
static bool autoWallEnabled = false;
static float autoWallValue = 10.0f;
static bool autoAimRealDistance = false;
static bool autoSlow = false;
static bool predEnabled = false;
static bool scopeControlEnabled = false;
static bool TriggerBot = false;
static bool hitchanceEnaled = false;
static float hitchance = 100.f;

void UI::ReloadWeaponSettings()
{
	ItemDefinitionIndex index = ItemDefinitionIndex::INVALID;
	if (Settings::Legitbot::weapons.find(currentWeapon) != Settings::Legitbot::weapons.end())
		index = currentWeapon;

	silent = Settings::Legitbot::weapons.at(index).silent;
	friendly = Settings::Legitbot::weapons.at(index).friendly;
	engageLock = Settings::Legitbot::weapons.at(index).engageLock;
	engageLockTR = Settings::Legitbot::weapons.at(index).engageLockTR;
	engageLockTTR = Settings::Legitbot::weapons.at(index).engageLockTTR;
	bone = Settings::Legitbot::weapons.at(index).bone;
	aimkey = Settings::Legitbot::weapons.at(index).aimkey;
	aimkeyOnly = Settings::Legitbot::weapons.at(index).aimkeyOnly;
	smoothEnabled = Settings::Legitbot::weapons.at(index).smoothEnabled;
	courseRandomizationEnabled = Settings::Legitbot::weapons.at(index).courseRandomizationEnabled;
	doAimAfterXShotsEnabled = Settings::Legitbot::weapons.at(index).doAimAfterXShotsEnabled;
	smoothValue = Settings::Legitbot::weapons.at(index).smoothAmount;
	courseRandomizationValue = Settings::Legitbot::weapons.at(index).courseRandomizationAmount;
	doAimAfterXShotsValue = Settings::Legitbot::weapons.at(index).doAimAfterXShotsAmount;
	smoothType = Settings::Legitbot::weapons.at(index).smoothType;
	smoothSaltEnabled = Settings::Legitbot::weapons.at(index).smoothSaltEnabled;
	smoothSaltMultiplier = Settings::Legitbot::weapons.at(index).smoothSaltMultiplier;
	errorMarginEnabled = Settings::Legitbot::weapons.at(index).errorMarginEnabled;
	errorMarginValue = Settings::Legitbot::weapons.at(index).errorMarginValue;
	autoAimEnabled = Settings::Legitbot::weapons.at(index).autoAimEnabled;
	autoAimValue = Settings::Legitbot::weapons.at(index).LegitautoAimFov;
	aimStepEnabled = Settings::Legitbot::weapons.at(index).aimStepEnabled;
	aimStepMin = Settings::Legitbot::weapons.at(index).aimStepMin;
	aimStepMax = Settings::Legitbot::weapons.at(index).aimStepMax;
	rcsEnabled = Settings::Legitbot::weapons.at(index).rcsEnabled;
	rcsAlwaysOn = Settings::Legitbot::weapons.at(index).rcsAlwaysOn;
	rcsAmountX = Settings::Legitbot::weapons.at(index).rcsAmountX;
	rcsAmountY = Settings::Legitbot::weapons.at(index).rcsAmountY;
	autoPistolEnabled = Settings::Legitbot::weapons.at(index).autoPistolEnabled;
	autoShootEnabled = Settings::Legitbot::weapons.at(index).autoShootEnabled;
	autoScopeEnabled = Settings::Legitbot::weapons.at(index).autoScopeEnabled;
	noShootEnabled = Settings::Legitbot::weapons.at(index).noShootEnabled;
	ignoreJumpEnabled = Settings::Legitbot::weapons.at(index).ignoreJumpEnabled;
	ignoreEnemyJumpEnabled = Settings::Legitbot::weapons.at(index).ignoreEnemyJumpEnabled;
	hitchanceEnaled = Settings::Legitbot::weapons.at(index).hitchanceEnaled;
	hitchance = Settings::Legitbot::weapons.at(index).hitchance;
	TriggerBot = Settings::Legitbot::weapons.at(index).TriggerBot;
	smokeCheck = Settings::Legitbot::weapons.at(index).smokeCheck;
	flashCheck = Settings::Legitbot::weapons.at(index).flashCheck;
	spreadLimitEnabled = Settings::Legitbot::weapons.at(index).spreadLimitEnabled;
	spreadLimit = Settings::Legitbot::weapons.at(index).spreadLimit;
	autoWallEnabled = Settings::Legitbot::weapons.at(index).autoWallEnabled;
	autoWallValue = Settings::Legitbot::weapons.at(index).autoWallValue;
	autoAimRealDistance = Settings::Legitbot::weapons.at(index).autoAimRealDistance;
	autoSlow = Settings::Legitbot::weapons.at(index).autoSlow;
	predEnabled = Settings::Legitbot::weapons.at(index).predEnabled;
	scopeControlEnabled = Settings::Legitbot::weapons.at(index).scopeControlEnabled;

	for (int bone = BONE_PELVIS; bone <= BONE_RIGHT_SOLE; bone++)
		desiredBones[bone] = Settings::Legitbot::weapons.at(index).desiredBones[bone];
}

void UI::UpdateWeaponSettings()
{
	if (Settings::Legitbot::weapons.find(currentWeapon) == Settings::Legitbot::weapons.end())
		Settings::Legitbot::weapons[currentWeapon] = LegitWeapon_t();

	LegitWeapon_t settings = {

	    .silent = silent,
	    .friendly = friendly,
	    .engageLock = engageLock,
	    .engageLockTR = engageLockTR,
	    .aimkeyOnly = aimkeyOnly,
	    .smoothEnabled = smoothEnabled,
	    .courseRandomizationEnabled = courseRandomizationEnabled,
	    .doAimAfterXShotsEnabled = doAimAfterXShotsEnabled,
	    .smoothSaltEnabled = smoothSaltEnabled,
	    .errorMarginEnabled = errorMarginEnabled,
	    .autoAimEnabled = autoAimEnabled,
	    .aimStepEnabled = aimStepEnabled,
	    .rcsEnabled = rcsEnabled,
	    .rcsAlwaysOn = rcsAlwaysOn,
	    .spreadLimitEnabled = spreadLimitEnabled,
	    .hitchanceEnaled = false,
	    .autoPistolEnabled = autoPistolEnabled,
	    .autoShootEnabled = autoShootEnabled,
	    .autoScopeEnabled = autoScopeEnabled,
	    .noShootEnabled = noShootEnabled,
	    .ignoreJumpEnabled = ignoreJumpEnabled,
	    .ignoreEnemyJumpEnabled = ignoreEnemyJumpEnabled,
	    .smokeCheck = smokeCheck,
	    .flashCheck = flashCheck,
	    .autoWallEnabled = autoWallEnabled,
	    .autoAimRealDistance = autoAimRealDistance,
	    .autoSlow = autoSlow,
	    .predEnabled = predEnabled,
	    .scopeControlEnabled = scopeControlEnabled,
	    .TriggerBot = TriggerBot,
	    .mindamage = 0,
	    .autoWall = false,

	    .engageLockTTR = engageLockTTR,
	    .bone = bone,
	    .smoothType = smoothType,
	    .aimkey = aimkey,
	    .smoothAmount = smoothValue,
	    .courseRandomizationAmount = courseRandomizationValue,
	    .doAimAfterXShotsAmount = doAimAfterXShotsValue,
	    .smoothSaltMultiplier = smoothSaltMultiplier,
	    .errorMarginValue = errorMarginValue,
	    .LegitautoAimFov = autoAimValue,
	    .aimStepMin = aimStepMin,
	    .aimStepMax = aimStepMax,
	    .rcsAmountX = rcsAmountX,
	    .rcsAmountY = rcsAmountY,
	    .autoWallValue = autoWallValue,
	    .spreadLimit = spreadLimit,
	    .minDamagevalue = 10.0f,
	    .hitchance = hitchance,
	};

	for (int bone = BONE_PELVIS; bone <= BONE_RIGHT_SOLE; bone++)
		settings.desiredBones[bone] = desiredBones[bone];

	Settings::Legitbot::weapons.at(currentWeapon) = settings;

	if (Settings::Legitbot::weapons.at(currentWeapon) == Settings::Legitbot::weapons.at(ItemDefinitionIndex::INVALID) &&
	    currentWeapon != ItemDefinitionIndex::INVALID)
	{
		Settings::Legitbot::weapons.erase(currentWeapon);
		UI::ReloadWeaponSettings();
	}
}

void Legitbot::RenderTab()
{
	const char *targets[] = {"PELVIS", "HIP", "LOWER SPINE", "MIDDLE SPINE", "UPPER SPINE", "NECK", "HEAD"};
	const char *smoothTypes[] = {"Slow Near End", "Constant Speed", "Fast Near End"};
	static char filterWeapons[32];

	if (ImGui::Checkbox(XORSTR("Enabled"), &Settings::Legitbot::enabled))
		UI::UpdateWeaponSettings();
	ImGui::Separator();

	ImGui::BeginGroup();
	{
		ImGui::Columns(3, nullptr, true);
		{
			ImGui::SetColumnOffset(1, 200);
			ImGui::PushItemWidth(-1);
			ImGui::InputText(XORSTR("##FILTERWEAPONS"), filterWeapons, IM_ARRAYSIZE(filterWeapons));
			ImGui::PopItemWidth();
			ImGui::ListBoxHeader(XORSTR("##GUNS"), ImVec2(-1, -1));
			for (auto it : ItemDefinitionIndexMap)
			{
				bool isDefault = (int)it.first < 0;
				if (!isDefault && !Util::Contains(Util::ToLower(std::string(filterWeapons)), Util::ToLower(Util::Items::GetItemDisplayName(it.first).c_str())))
					continue;

				if (Util::Items::IsKnife(it.first) || Util::Items::IsGlove(it.first) || Util::Items::IsUtility(it.first))
					continue;

				const bool item_selected = ((int)it.first == (int)currentWeapon);
				ImGui::PushID((int)it.first);
				std::string formattedName;
				char changeIndicator = ' ';
				bool isChanged = Settings::Legitbot::weapons.find(it.first) != Settings::Legitbot::weapons.end();
				if (!isDefault && isChanged)
					changeIndicator = '*';
				formattedName = changeIndicator + (isDefault ? Util::Items::GetItemDisplayName(it.first).c_str() : Util::Items::GetItemDisplayName(it.first));
				if (ImGui::Selectable(formattedName.c_str(), item_selected))
				{
					currentWeapon = it.first;
					UI::ReloadWeaponSettings();
				}
				ImGui::PopID();
			}
			ImGui::ListBoxFooter();
		}
		ImGui::NextColumn();
		{
			ImGui::SetColumnOffset(2, ImGui::GetWindowWidth() / 2 + 226);
			ImGui::BeginChild(XORSTR("COL1"), ImVec2(0, 0), true);
			{
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Accuracy"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, true);
				{
					if (ImGui::Checkbox(XORSTR("Auto Aim"), &autoAimEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Distance-Based FOV"), &autoAimRealDistance))
						UI::UpdateWeaponSettings();
				}
				ImGui::NextColumn();
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::SliderFloat(XORSTR("##AA"), &autoAimValue, 0, 180))
						UI::UpdateWeaponSettings();
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
				ImGui::Columns(2, nullptr, true);
				{
					if (ImGui::Checkbox(XORSTR("Recoil Control Enabled"), &rcsEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Recoil Control Always on"), &rcsAlwaysOn))
						UI::UpdateWeaponSettings();
				}
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::SliderFloat(XORSTR("##RCSX"), &rcsAmountX, 0, 2, XORSTR("Pitch: %0.3f")))
					UI::UpdateWeaponSettings();
				if (ImGui::SliderFloat(XORSTR("##RCSY"), &rcsAmountY, 0, 2, XORSTR("Yaw: %0.3f")))
					UI::UpdateWeaponSettings();
				ImGui::PopItemWidth();
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Casual / DM Only"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, true);
				{
					if (ImGui::Checkbox(XORSTR("Aim Step"), &aimStepEnabled))
						UI::UpdateWeaponSettings();
				}
				ImGui::NextColumn();
				{
					ImGui::PushItemWidth(-1);
					ImGui::Text(XORSTR("Min"));
					if (ImGui::SliderFloat(XORSTR("##STEPMIN"), &aimStepMin, 5, 35))
						UI::UpdateWeaponSettings();
					ImGui::Text(XORSTR("Max"));
					if (ImGui::SliderFloat(XORSTR("##STEPMAX"), &aimStepMax, (aimStepMin) + 1.0f, 35))
						UI::UpdateWeaponSettings();
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Humanizing"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, true);
				{
					if (ImGui::Checkbox(XORSTR("Smoothing"), &smoothEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Smooth Salting"), &smoothSaltEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Error Margin"), &errorMarginEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Advanced Error"), &courseRandomizationEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Aim After Shot #"), &doAimAfterXShotsEnabled))
						UI::UpdateWeaponSettings();
					ImGui::PushItemWidth(-1);
					if (ImGui::Combo(XORSTR("##SMOOTHTYPE"), (int *)&smoothType, smoothTypes, IM_ARRAYSIZE(smoothTypes)))
						UI::UpdateWeaponSettings();
					ImGui::PopItemWidth();
				}
				ImGui::NextColumn();
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::SliderFloat(XORSTR("##SMOOTH"), &smoothValue, 0, 1))
						UI::UpdateWeaponSettings();
					if (ImGui::SliderFloat(XORSTR("##SALT"), &smoothSaltMultiplier, 0, smoothValue))
						UI::UpdateWeaponSettings();
					if (ImGui::SliderFloat(XORSTR("##ERROR"), &errorMarginValue, 0, 2))
						UI::UpdateWeaponSettings();
					if (ImGui::SliderFloat(XORSTR("##COURSERANDOMIZATION"), &courseRandomizationValue, 1, 6))
						UI::UpdateWeaponSettings();
					if (ImGui::SliderFloat(XORSTR("##DOAIMAFTERXSHOTS"), &doAimAfterXShotsValue, 0, 30))
						UI::UpdateWeaponSettings();
					ImGui::PopItemWidth();
				}
				ImGui::EndColumns();
				ImGui::Separator();
				ImGui::Columns(1);
				ImGui::Text(XORSTR("Target Hitboxes"));
				ImGui::Separator();
				ImGui::Columns(3, nullptr, false);
				{
					ImGui::PushItemWidth(-1);
					ImGui::Text(XORSTR("Player's Right Arm"));
					if (ImGui::Checkbox(XORSTR("Collarbone"), &desiredBones[BONE_RIGHT_COLLARBONE]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Shoulder"), &desiredBones[BONE_RIGHT_SHOULDER]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Armpit"), &desiredBones[BONE_RIGHT_ARMPIT]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Bicep"), &desiredBones[BONE_RIGHT_BICEP]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Elbow"), &desiredBones[BONE_RIGHT_ELBOW]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Forearm"), &desiredBones[BONE_RIGHT_FOREARM]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Wrist"), &desiredBones[BONE_RIGHT_WRIST]))
						UI::UpdateWeaponSettings();
					ImGui::Text(XORSTR("Player's Right Leg"));
					if (ImGui::Checkbox(XORSTR("Buttcheek"), &desiredBones[BONE_RIGHT_BUTTCHEEK]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Thigh"), &desiredBones[BONE_RIGHT_THIGH]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Knee"), &desiredBones[BONE_RIGHT_KNEE]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Ankle"), &desiredBones[BONE_RIGHT_ANKLE]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Sole"), &desiredBones[BONE_RIGHT_SOLE]))
						UI::UpdateWeaponSettings();
				}
				ImGui::NextColumn();
				{
					ImGui::Text(XORSTR("Center Mass"));
					if (ImGui::Checkbox(XORSTR("Head"), &desiredBones[BONE_HEAD]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Neck"), &desiredBones[BONE_NECK]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Upper Spine"), &desiredBones[BONE_UPPER_SPINAL_COLUMN]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Middle Spine"), &desiredBones[BONE_MIDDLE_SPINAL_COLUMN]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Lower Spine"), &desiredBones[BONE_LOWER_SPINAL_COLUMN]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Pelvis"), &desiredBones[BONE_PELVIS]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Hip"), &desiredBones[BONE_HIP]))
						UI::UpdateWeaponSettings();
				}
				ImGui::NextColumn();
				{ // these spaces are here in the strings because checkboxes can't have duplicate titles.
					ImGui::Text(XORSTR("Player's Left Arm"));
					if (ImGui::Checkbox(XORSTR("Collarbone "), &desiredBones[BONE_LEFT_COLLARBONE]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Shoulder "), &desiredBones[BONE_LEFT_SHOULDER]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Armpit "), &desiredBones[BONE_LEFT_ARMPIT]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Bicep "), &desiredBones[BONE_LEFT_BICEP]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Elbow "), &desiredBones[BONE_LEFT_ELBOW]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Forearm "), &desiredBones[BONE_LEFT_FOREARM]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Wrist "), &desiredBones[BONE_LEFT_WRIST]))
						UI::UpdateWeaponSettings();

					ImGui::Text(XORSTR("Player's Left Leg"));
					if (ImGui::Checkbox(XORSTR("Buttcheek "), &desiredBones[BONE_LEFT_BUTTCHEEK]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Thigh "), &desiredBones[BONE_LEFT_THIGH]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Knee "), &desiredBones[BONE_LEFT_KNEE]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Ankle "), &desiredBones[BONE_LEFT_ANKLE]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Sole "), &desiredBones[BONE_LEFT_SOLE]))
						UI::UpdateWeaponSettings();
				}
				ImGui::EndColumns();
				ImGui::Separator();
				ImGui::PopItemWidth();
			}
			ImGui::EndChild();
		}
		ImGui::NextColumn();
		{
			ImGui::BeginChild(XORSTR("COL2"), ImVec2(0, 0), true);
			{
				ImGui::Text(XORSTR("Aimkey Only"));
				ImGui::Separator();
				ImGui::Columns(2, nullptr, true);
				{
					if (ImGui::Checkbox(XORSTR("Enabled"), &aimkeyOnly))
						UI::UpdateWeaponSettings();
				}
				ImGui::NextColumn();
				{
					UI::KeyBindButton(&aimkey);
				}
				ImGui::Columns(1, nullptr, true);
				ImGui::Separator();
				ImGui::Text(XORSTR("Other Features"));
				ImGui::Separator();
				ImGui::Columns(1, nullptr, true);
				{

					switch (currentWeapon)
					{
					case ItemDefinitionIndex::INVALID:
					case ItemDefinitionIndex::WEAPON_DEAGLE:
					case ItemDefinitionIndex::WEAPON_ELITE:
					case ItemDefinitionIndex::WEAPON_FIVESEVEN:
					case ItemDefinitionIndex::WEAPON_GLOCK:
					case ItemDefinitionIndex::WEAPON_TEC9:
					case ItemDefinitionIndex::WEAPON_HKP2000:
					case ItemDefinitionIndex::WEAPON_USP_SILENCER:
					case ItemDefinitionIndex::WEAPON_P250:
					case ItemDefinitionIndex::WEAPON_CZ75A:
					case ItemDefinitionIndex::WEAPON_REVOLVER:
						if (ImGui::Checkbox(XORSTR("Auto Pistol"), &autoPistolEnabled))
							UI::UpdateWeaponSettings();
						break;
					default:
						break;
					}

					if (ImGui::Checkbox(XORSTR("Silent Aim"), &silent))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Smoke Check"), &smokeCheck))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Prediction"), &predEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("No Shoot"), &noShootEnabled))
						UI::UpdateWeaponSettings();

					switch (currentWeapon)
					{
					case ItemDefinitionIndex::WEAPON_DEAGLE:
					case ItemDefinitionIndex::WEAPON_ELITE:
					case ItemDefinitionIndex::WEAPON_FIVESEVEN:
					case ItemDefinitionIndex::WEAPON_GLOCK:
					case ItemDefinitionIndex::WEAPON_TEC9:
					case ItemDefinitionIndex::WEAPON_HKP2000:
					case ItemDefinitionIndex::WEAPON_USP_SILENCER:
					case ItemDefinitionIndex::WEAPON_P250:
					case ItemDefinitionIndex::WEAPON_CZ75A:
					case ItemDefinitionIndex::WEAPON_REVOLVER:
						break;
					default:
						if (ImGui::Checkbox(XORSTR("Auto Scope"), &autoScopeEnabled))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XORSTR("Scope Control"), &scopeControlEnabled))
							UI::UpdateWeaponSettings();
					}

					if (ImGui::Checkbox(XORSTR("Flash Check"), &flashCheck))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Ignore Jump (Self)"), &ignoreJumpEnabled))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XORSTR("Ignore Jump (Enemies)"), &ignoreEnemyJumpEnabled))
						UI::UpdateWeaponSettings();
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Misc"));
				ImGui::Separator();
				ImGui::Columns(1);
				{
					if (ImGui::Checkbox(XORSTR("EngageLock"), &engageLock))
						UI::UpdateWeaponSettings();
					if (engageLock)
					{
						if (ImGui::Checkbox(XORSTR("Target Reacquisition"), &engageLockTR))
							UI::UpdateWeaponSettings();
						if (engageLockTR)
						{
							if (ImGui::SliderInt(XORSTR("##TTR"), &engageLockTTR, 0, 1000))
								UI::UpdateWeaponSettings();
						}
					}
					if (ImGui::Checkbox(XORSTR("FriendlyFire"), &friendly))
						UI::UpdateWeaponSettings();
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Semirage Options"));
				ImGui::Separator();
				if (ImGui::Checkbox(XORSTR("Auto Shoot"), &autoShootEnabled))
					UI::UpdateWeaponSettings();
				ImGui::Checkbox(XORSTR("Velocity Check"), &Settings::Legitbot::AutoShoot::velocityCheck);
				if (ImGui::Checkbox(XORSTR("Spread Limit"), &spreadLimitEnabled))
					UI::UpdateWeaponSettings();
				if (ImGui::SliderFloat(XORSTR("##SPREADLIMIT"), &spreadLimit, 0, 0.1))
					UI::UpdateWeaponSettings();
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("Autowall"));
				ImGui::Separator();
				ImGui::Columns(1);
				{

					if (ImGui::Checkbox(XORSTR("Autowall"), &autoWallEnabled))
						UI::UpdateWeaponSettings();
				}
				ImGui::Columns(1);
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::SliderFloat(XORSTR("##AUTOWALLDMG"), &autoWallValue, 0, 100, XORSTR("Min Damage: %f")))
						UI::UpdateWeaponSettings();
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text(XORSTR("AutoSlow"));
				ImGui::Separator();
				if (ImGui::Checkbox(XORSTR("Enabled##AUTOSLOW"), &autoSlow))
					UI::UpdateWeaponSettings();
				ImGui::Columns(1);
				ImGui::Separator();
				if (currentWeapon > ItemDefinitionIndex::INVALID && Settings::Legitbot::weapons.find(currentWeapon) != Settings::Legitbot::weapons.end())
				{
					if (ImGui::Button(XORSTR("Clear Weapon Settings"), ImVec2(-1, 0)))
					{
						Settings::Legitbot::weapons.erase(currentWeapon);
						UI::ReloadWeaponSettings();
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndColumns();
	}
	ImGui::EndGroup();
}