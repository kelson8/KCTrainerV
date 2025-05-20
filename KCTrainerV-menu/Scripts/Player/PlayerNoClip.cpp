#include "pch.h"

#include "PlayerNoClip.h"

#include "Util/Enums.h"

//#include "Scripts/Extras/Classes/GTAblip.h"
//#include "Scripts/Extras/Classes/GTAentity.h"
//#include "Scripts/Extras/Classes/GTAped.h"
//#include "Scripts/Extras/Classes/GTAplayer.h"
//#include "Scripts/Extras/Classes/GTAvehicle.h"

// TODO Implement this, copied from Menyoo in Routine.cpp

// TODO Copy in GTAentity file - mostly complete.

// Copy these remaining files from Menyoo Scripting folder:
/*
Camera.cpp
Checkpoint, Checkpoint.h
CustomHelpText.cpp, CustomHelpText.h
DxHookIMG.cpp, DxHookIMG.h
Game.cpp, Game.h
GameplayCamera.cpp, GameplayCamera.h
GTAprop.cpp, GTAprop.h
ModelNames.cpp, ModelNames.h
PTFX.cpp, PTFX.h
TimecycleModification.cpp, TimecycleModification.h
WeaponIndivs.cpp, WeaponIndivs.h
World.cpp, World.h
*/

// Fix these files to work:
/*
Rope
Task_old - Rename to something else.
*/

// Files that this builds with so far:
/*
GTAblip
GTAentity
GTAped,
GTAplayer,
GTAvehicle,
Model,
Scaleform
*/

// This contains required items for this, although I could probably adapt it to use Ped and other ScriptHookV items.
//#define MENYOO_SCRIPT_FILES
#ifdef MENYOO_SCRIPT_FILES

// Misc - FreeCam
bool loop_no_clip = false, 
bool loop_no_clip_toggle = false;

bool bit_noclip_already_invis, bit_noclip_already_collis, bit_noclip_show_help = true;
Camera g_cam_noClip;
void set_no_clip_off1()
{
	GTAentity myPed = PLAYER_PED_ID();
	GTAentity ent = IS_PED_IN_ANY_VEHICLE(myPed.Handle(), false) ? GET_VEHICLE_PED_IS_IN(myPed.Handle(), false) : myPed;

	ent.RequestControl();
	ent.SetVisible(!bit_noclip_already_invis);
	ent.IsCollisionEnabled_set(bit_noclip_already_collis);
	ent.FreezePosition(false);
	ENABLE_CONTROL_ACTION(2, INPUT_VEH_HORN, TRUE);
	ENABLE_CONTROL_ACTION(2, INPUT_LOOK_BEHIND, TRUE);
	ENABLE_CONTROL_ACTION(2, INPUT_VEH_LOOK_BEHIND, TRUE);
	ENABLE_CONTROL_ACTION(2, INPUT_SELECT_WEAPON, TRUE);
	bit_noclip_show_help = true;
}
void set_no_clip_off2()
{
	auto& cam = g_cam_noClip;
	if (cam.Exists())
	{
		cam.SetActive(false);
		cam.Destroy();
		World::RenderingCamera_set(0);
	}
}

// TODO Look into replicating this, might be useful for my menu.
void set_no_clip()
{
	//if (sub::Spooner::SpoonerMode::bEnabled)
	//	return;

	GTAentity myPed = PLAYER_PED_ID();
	GTAplayer myPlayer = PLAYER_ID();
	GTAentity ent = IS_PED_IN_ANY_VEHICLE(myPed.Handle(), false) ? GET_VEHICLE_PED_IS_IN(myPed.Handle(), false) : myPed;

	if (ent.Exists())
	{
		// TODO Figure this part out.
		
		if (Menu::bit_controller ? (IS_CONTROL_PRESSED(2, INPUT_FRONTEND_X) && IS_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_LS)) : IsKeyJustUp(bind_no_clip))
		{
			loop_no_clip_toggle = !loop_no_clip_toggle;
			if (!loop_no_clip_toggle)
			{
				set_no_clip_off1();
			}
			else
			{
#ifdef DISABLED_CODE
				if (bit_noclip_show_help)
				{
					bit_noclip_show_help = false;
					if (Menu::bit_controller) Game::CustomHelpText::ShowTimedText(oss_ << "FreeCam:~n~~INPUT_MOVE_UD~ = " << Game::GetGXTEntry("ITEM_MOV_CAM")
						<< "~n~~INPUT_LOOK_LR~ = " << Game::GetGXTEntry("ITEM_MOVE") << "~n~~INPUT_FRONTEND_RT~/~INPUT_FRONTEND_LT~ = " << "Ascend/Descend" << "~n~~INPUT_FRONTEND_RB~ = " << "Hasten", 6000);
					else Game::CustomHelpText::ShowTimedText(oss_ << "FreeCam:~n~~INPUT_MOVE_UD~/~INPUT_MOVE_LR~ = " << Game::GetGXTEntry("ITEM_MOV_CAM")
						<< "~n~~INPUT_LOOK_LR~ = " << Game::GetGXTEntry("ITEM_MOVE") << "~n~~INPUT_PARACHUTE_BRAKE_RIGHT~/~INPUT_PARACHUTE_BRAKE_LEFT~ = " << "Ascend/Descend" << "~n~~INPUT_SPRINT~ = " << "Hasten", 6000);
					bit_noclip_show_help = false;
				}
#endif //DISABLED_CODE
				bit_noclip_already_invis = !ent.IsVisible();
				bit_noclip_already_collis = ent.IsCollisionEnabled_get();
				//if (IS_ENTITY_A_PED(tempEntity)) task_HandsUp(tempEntity);
			}
		}

		if (!loop_no_clip_toggle)
		{
			set_no_clip_off2();
			return;
		}

		DISABLE_CONTROL_ACTION(2, INPUT_VEH_HORN, TRUE);
		DISABLE_CONTROL_ACTION(2, INPUT_LOOK_BEHIND, TRUE);
		DISABLE_CONTROL_ACTION(2, INPUT_VEH_LOOK_BEHIND, TRUE);
		DISABLE_CONTROL_ACTION(2, INPUT_SELECT_WEAPON, TRUE);
		DISABLE_CONTROL_ACTION(2, INPUT_VEH_ACCELERATE, TRUE);
		DISABLE_CONTROL_ACTION(2, INPUT_VEH_BRAKE, TRUE);
		DISABLE_CONTROL_ACTION(2, INPUT_VEH_RADIO_WHEEL, TRUE);

		const Vector3& entPos = ent.Position_get();
		const Vector3& camOffset = Vector3();//Vector3(0, -4.0f, 3.6f);

		if (!cam.Exists())
		{
			ent.RequestControl();
			cam = World::CreateCamera();
			cam.Position_set(GameplayCamera::Position_get());
			cam.Rotation_set(GameplayCamera::Rotation_get());
			cam.AttachTo(ent, camOffset);
			cam.FieldOfView_set(GameplayCamera::FieldOfView_get());
			cam.DepthOfFieldStrength_set(0.0f);
			World::RenderingCamera_set(cam);
		}

		ent.RequestControl();
		//ent.Position_set(cam.GetOffsetInWorldCoords(-camOffset));
		ent.FreezePosition(true);
		ent.IsCollisionEnabled_set(false);
		ent.SetVisible(false);
		myPed.SetVisible(false);

		Vector3 nextRot = cam.Rotation_get() - Vector3(GET_DISABLED_CONTROL_NORMAL(0, INPUT_LOOK_UD), 0, GET_DISABLED_CONTROL_NORMAL(0, INPUT_LOOK_LR)) * (Menu::bit_controller ? 2.5f : 11.0f);
		nextRot.y = 0.0f; // No roll
		ent.Rotation_set(Vector3(0, 0, nextRot.z));
		cam.Rotation_set(nextRot);
		if (!myPlayer.IsFreeAiming() && !myPlayer.IsTargetingAnything())
			SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);

		if (Menu::bit_controller)
		{
			DISABLE_CONTROL_ACTION(0, INPUT_VEH_HORN, TRUE);

			if (ent == myPed)
			{
				if (GET_PED_STEALTH_MOVEMENT(myPed.Handle()))
					SET_PED_STEALTH_MOVEMENT(myPed.Handle(), false, 0);
				if (GET_PED_COMBAT_MOVEMENT(myPed.Handle()))
					SET_PED_COMBAT_MOVEMENT(myPed.Handle(), 0);
			}
			float noclip_prec_level = IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) ? 1.8f : 0.8f;
			Vector3 offset;
			offset.x = GET_CONTROL_NORMAL(0, INPUT_MOVE_LR) * noclip_prec_level;
			offset.y = -GET_CONTROL_NORMAL(0, INPUT_MOVE_UD) * noclip_prec_level;
			offset.z = (GET_DISABLED_CONTROL_NORMAL(2, INPUT_FRONTEND_RT) - GET_DISABLED_CONTROL_NORMAL(2, INPUT_FRONTEND_LT)) * noclip_prec_level;
			if (!offset.IsZero())
				ent.Position_set(cam.GetOffsetInWorldCoords(offset - camOffset));

			//if (Menu::currentsub == SUB::CLOSED)
			//{
			//	Menu::add_IB(INPUT_MOVE_UD, "ITEM_MOV_CAM");
			//	Menu::add_IB(INPUT_LOOK_LR, "ITEM_MOVE");
			//	Menu::add_IB(INPUT_FRONTEND_LT, "");
			//	Menu::add_IB(INPUT_FRONTEND_RT, "Ascend/Descend");
			//	Menu::add_IB(INPUT_FRONTEND_RB, "Hasten");
			//	//Menu::add_IB(INPUT_FRONTEND_RLEFT, "Visibility");
			//}
		}
		else
		{
			float noclip_prec_level = IS_DISABLED_CONTROL_PRESSED(0, INPUT_SPRINT) ? 1.77f : 0.35f;
			Vector3 offset;
			offset.x = GET_CONTROL_NORMAL(0, INPUT_MOVE_LR) * noclip_prec_level;
			offset.y = -GET_CONTROL_NORMAL(0, INPUT_MOVE_UD) * noclip_prec_level;
			offset.z = IS_DISABLED_CONTROL_PRESSED(2, INPUT_PARACHUTE_BRAKE_RIGHT) ? noclip_prec_level : IS_DISABLED_CONTROL_PRESSED(2, INPUT_PARACHUTE_BRAKE_LEFT) ? -noclip_prec_level : 0.0f;
			if (!offset.IsZero())
				ent.Position_set(cam.GetOffsetInWorldCoords(offset - camOffset));

			//if (Menu::currentsub == SUB::CLOSED)
			//{
			//	Menu::add_IB(INPUT_LOOK_LR, "ITEM_MOVE");
			//	Menu::add_IB(INPUT_SCRIPT_PAD_RIGHT, "");
			//	Menu::add_IB(INPUT_SCRIPT_PAD_DOWN, "");
			//	Menu::add_IB(INPUT_SCRIPT_PAD_LEFT, "");
			//	Menu::add_IB(INPUT_SCRIPT_PAD_UP, "ITEM_MOV_CAM");
			//	Menu::add_IB(INPUT_PARACHUTE_BRAKE_LEFT, "");
			//	Menu::add_IB(INPUT_PARACHUTE_BRAKE_RIGHT, "Ascend/Descend");
			//	Menu::add_IB(INPUT_SPRINT, "Hasten");
			//	//Menu::add_IB(-3, "Visibility - T");
			//}
		}
	}

}


#endif // MENYOO_SCRIPT_FILES