#include <algorithm>

#include "visuals.hpp"

#include "../options.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"

std::string string_format(const std::string fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}

RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

Visuals::Visuals()
{
	InitializeCriticalSection(&cs);
}

Visuals::~Visuals() {
	DeleteCriticalSection(&cs);
}

//--------------------------------------------------------------------------------
void Visuals::Render() {
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl)
{
	if (pl->IsDormant() || !pl->IsAlive())
		return false;

	ctx.pl = pl;
	ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (!ctx.is_visible && g_Options.esp_visible_only)
		return false;

	if (!ctx.is_enemy && g_Options.esp_enemies_only)
		return false;

	ctx.clr = ctx.is_enemy ? (ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded) : (ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded);

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	head.z += 15;

	if (!Math::WorldToScreen(head, ctx.head_pos) ||
		!Math::WorldToScreen(origin, ctx.feet_pos))
		return false;

	auto h = fabs(ctx.head_pos.y - ctx.feet_pos.y);
	auto w = h / 1.65f;

	ctx.bbox.left = static_cast<long>(ctx.feet_pos.x - w * 0.5f);
	ctx.bbox.right = static_cast<long>(ctx.bbox.left + w);
	ctx.bbox.bottom = static_cast<long>(ctx.feet_pos.y);
	ctx.bbox.top = static_cast<long>(ctx.head_pos.y);

	return true;
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox() {
	float edge_size;

	float
		length_horizontal = (ctx.bbox.right - ctx.bbox.left) * 0.2f,
		length_vertical = (ctx.bbox.bottom - ctx.bbox.top) * 0.2f;

	switch (g_Options.esp_player_boxes) {
	case 1:
		Render::Get().RenderBox(ctx.bbox, ctx.clr);
		Render::Get().RenderBox(ctx.bbox.left - 1.f, ctx.bbox.top - 1.f, ctx.bbox.right + 1.f, ctx.bbox.bottom + 1.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBox(ctx.bbox.left + 1.f, ctx.bbox.top + 1.f, ctx.bbox.right - 1.f, ctx.bbox.bottom - 1.f, Color(0, 0, 0, ctx.clr.a()));
		break;

	case 2:
		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.top - 1.f, ctx.bbox.left + 1.f + length_horizontal, ctx.bbox.top + 2.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 1.f - length_horizontal, ctx.bbox.top - 1.f, ctx.bbox.right + 1.f, ctx.bbox.top + 2.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.bottom - 2.f, ctx.bbox.left + 1.f + length_horizontal, ctx.bbox.bottom + 1.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 1.f - length_horizontal, ctx.bbox.bottom - 2.f, ctx.bbox.right + 1.f, ctx.bbox.bottom + 1.f, Color(0, 0, 0, ctx.clr.a()));

		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.top + 2.f, ctx.bbox.left + 2.f, ctx.bbox.top + 1.f + length_vertical, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 2.f, ctx.bbox.top + 2.f, ctx.bbox.right + 1.f, ctx.bbox.top + 1.f + length_vertical, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.left - 1.f, ctx.bbox.bottom - 1.f - length_vertical, ctx.bbox.left + 2.f, ctx.bbox.bottom - 2.f, Color(0, 0, 0, ctx.clr.a()));
		Render::Get().RenderBoxFilled(ctx.bbox.right - 2.f, ctx.bbox.bottom - 1.f - length_vertical, ctx.bbox.right + 1.f, ctx.bbox.bottom - 2.f, Color(0, 0, 0, ctx.clr.a()));


		Render::Get().RenderLine(float(ctx.bbox.left), float(ctx.bbox.top), ctx.bbox.left + length_horizontal - 1.f, float(ctx.bbox.top), ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - length_horizontal, float(ctx.bbox.top), ctx.bbox.right - 1.f, float(ctx.bbox.top), ctx.clr);
		Render::Get().RenderLine(float(ctx.bbox.left), ctx.bbox.bottom - 1.f, ctx.bbox.left + length_horizontal - 1.f, ctx.bbox.bottom - 1.f, ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - length_horizontal, ctx.bbox.bottom - 1.f, ctx.bbox.right - 1.f, ctx.bbox.bottom - 1.f, ctx.clr);

		Render::Get().RenderLine(float(ctx.bbox.left), float(ctx.bbox.top), float(ctx.bbox.left), ctx.bbox.top + length_vertical - 1.f, ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - 1.f, float(ctx.bbox.top), ctx.bbox.right - 1.f, ctx.bbox.top + length_vertical - 1.f, ctx.clr);
		Render::Get().RenderLine(float(ctx.bbox.left), ctx.bbox.bottom - length_vertical, float(ctx.bbox.left), ctx.bbox.bottom - 1.f, ctx.clr);
		Render::Get().RenderLine(ctx.bbox.right - 1.f, ctx.bbox.bottom - length_vertical, ctx.bbox.right - 1.f, ctx.bbox.bottom - 1.f, ctx.clr);
		break;

	case 3:
		Render::Get().RenderBox(ctx.bbox, ctx.clr, 1.0f, 7.0f);
		//Render::Get().RenderBox(ctx.bbox, ctx.clr, 7.0f);
		break;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSkeleton()
{
	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel(ctx.pl->GetModel());

	if (pStudioModel) {
		static matrix3x4_t pBoneToWorldOut[128];
		//if (ctx.pl->HandleBoneSetup(pBoneToWorldOut, 256, g_GlobalVars->curtime)) {
		if (ctx.pl->SetupBones(pBoneToWorldOut, 128, 256, g_GlobalVars->curtime)) {
			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->GetBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
					continue;

				Render::Get().RenderLine(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, ctx.pl->IsDormant() ? ctx.clr : Color(g_Options.color_esp_skeleton));
			}
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName()
{
	player_info_t info = ctx.pl->GetPlayerInfo();

	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, info.szName);

	Render::Get().RenderText(info.szName, ctx.feet_pos.x - sz.x / 2, ctx.head_pos.y - sz.y, 14.f,  ctx.clr);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth()
{
	auto  hp = ctx.pl->m_iHealth();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 8;

	int height = (box_h * hp) / 100;

	int green = int(hp * 2.55f);
	int red = 255 - green;

	int x = ctx.bbox.left - off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Render::Get().RenderBox(x, y, x + w, y + h, Color::Black, 1.f, true);
	Render::Get().RenderBox(x + 1, y + 1, x + w - 1, y + height - 2, Color(red, green, 0, 255), 1.f, true);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour()
{
	auto  armour = ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 4;

	int height = (((box_h * armour) / 100));

	int x = ctx.bbox.right + off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Render::Get().RenderBox(x, y, x + w, y + h, Color::Black, 1.f, true);
	Render::Get().RenderBox(x + 1, y + 1, x + w - 1, y + height - 2, Color(0, 50, 255, 255), 1.f, true);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeaponName()
{
	auto weapon = ctx.pl->m_hActiveWeapon().Get();

	if (!weapon) return;
	if (!weapon->GetCSWeaponData()) return;

	auto text = weapon->GetCSWeaponData()->szWeaponName + 7;
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, text);
	std::string ammo = string_format("[%i/%i]", weapon->m_iPrimaryReserveAmmoCount(), weapon->m_iClip1());
	auto sz2 = g_pDefaultFont->CalcTextSizeA(15.f, FLT_MAX, 0.0f, ammo.c_str());

	Render::Get().RenderText(text, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left) * 0.5f - sz.x * 0.5f, ctx.bbox.bottom + 1.f, 14.f, ctx.pl->IsDormant() ? ctx.clr : Color(255, 255, 255, 255), false, false);
	if (g_Options.esp_player_ammo)
		Render::Get().RenderText(ammo, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left) * 0.5f - sz2.x * 0.5f, ctx.bbox.bottom + 1.f + sz2.y, 14.f, ctx.pl->IsDormant() ? ctx.clr : Color(255, 255, 255, 255), false, false);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSnapline()
{

	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);

	Render::Get().RenderLine(screen_w / 2.f, (float)screen_h,
		ctx.feet_pos.x, ctx.feet_pos.y, ctx.clr);
}
//--------------------------------------------------------------------------------
void Visuals::RenderCrosshair()
{
	int w, h;

	g_EngineClient->GetScreenSize(w, h);

	int cx = w / 2;
	int cy = h / 2;
	Render::Get().RenderLine(cx - 25, cy, cx + 25, cy, g_Options.color_esp_crosshair);
	Render::Get().RenderLine(cx, cy - 25, cx, cy + 25, g_Options.color_esp_crosshair);
}
//--------------------------------------------------------------------------------
void Visuals::RenderWeapon(C_BaseCombatWeapon* ent)
{
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	// We don't want to Render weapons that are being held
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	Render::Get().RenderBox(bbox, g_Options.color_esp_weapons);


	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);

	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;


	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, g_Options.color_esp_weapons);
}
//--------------------------------------------------------------------------------
void Visuals::RenderDefuseKit(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	Render::Get().RenderBox(bbox, g_Options.color_esp_defuse);

	auto name = "Defuse Kit";
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;
	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, g_Options.color_esp_defuse);
}
//--------------------------------------------------------------------------------
void Visuals::RenderPlantedC4(C_BaseEntity* ent)
{
	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;


	Render::Get().RenderBox(bbox, g_Options.color_esp_c4);


	int bombTimer = std::ceil(ent->m_flC4Blow() - g_GlobalVars->curtime);
	std::string timer = std::to_string(bombTimer);

	auto name = (bombTimer < 0.f) ? "Bomb" : timer;
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name.c_str());
	int w = bbox.right - bbox.left;

	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, g_Options.color_esp_c4);
}
//--------------------------------------------------------------------------------
void Visuals::RenderItemEsp(C_BaseEntity* ent)
{
	std::string itemstr = "Undefined";
	const model_t * itemModel = ent->GetModel();
	if (!itemModel)
		return;
	studiohdr_t * hdr = g_MdlInfo->GetStudiomodel(itemModel);
	if (!hdr)
		return;
	itemstr = hdr->szName;
	if (ent->GetClientClass()->m_ClassID == ClassId_CBumpMine)
		itemstr = "";
	else if (itemstr.find("case_pistol") != std::string::npos)
		itemstr = "Pistol Case";
	else if (itemstr.find("case_light_weapon") != std::string::npos)
		itemstr = "Light Case";
	else if (itemstr.find("case_heavy_weapon") != std::string::npos)
		itemstr = "Heavy Case";
	else if (itemstr.find("case_explosive") != std::string::npos)
		itemstr = "Explosive Case";
	else if (itemstr.find("case_tools") != std::string::npos)
		itemstr = "Tools Case";
	else if (itemstr.find("random") != std::string::npos)
		itemstr = "Airdrop";
	else if (itemstr.find("dz_armor_helmet") != std::string::npos)
		itemstr = "Full Armor";
	else if (itemstr.find("dz_helmet") != std::string::npos)
		itemstr = "Helmet";
	else if (itemstr.find("dz_armor") != std::string::npos)
		itemstr = "Armor";
	else if (itemstr.find("upgrade_tablet") != std::string::npos)
		itemstr = "Tablet Upgrade";
	else if (itemstr.find("briefcase") != std::string::npos)
		itemstr = "Briefcase";
	else if (itemstr.find("parachutepack") != std::string::npos)
		itemstr = "Parachute";
	else if (itemstr.find("dufflebag") != std::string::npos)
		itemstr = "Cash Dufflebag";
	else if (itemstr.find("ammobox") != std::string::npos)
		itemstr = "Ammobox";
	else if (itemstr.find("dronegun") != std::string::npos)
		itemstr = "Turrel";
	else if (itemstr.find("exojump") != std::string::npos)
		itemstr = "Exojump";
	else if (itemstr.find("healthshot") != std::string::npos)
		itemstr = "Healthshot";
	else {
		/*May be you will search some missing items..*/
		/*static std::vector<std::string> unk_loot;
		if (std::find(unk_loot.begin(), unk_loot.end(), itemstr) == unk_loot.end()) {
			Utils::ConsolePrint(itemstr.c_str());
			unk_loot.push_back(itemstr);
		}*/
		return;
	}
	
	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, itemstr.c_str());
	int w = bbox.right - bbox.left;


	//Render::Get().RenderBox(bbox, g_Options.color_esp_item);
	Render::Get().RenderText(itemstr, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, g_Options.color_esp_item);
}
//--------------------------------------------------------------------------------
void Visuals::ThirdPerson() {
	if (!g_LocalPlayer)
		return;

	if (g_Options.misc_thirdperson && g_LocalPlayer->IsAlive())
	{
		if (!g_Input->m_fCameraInThirdPerson)
		{
			g_Input->m_fCameraInThirdPerson = true;
		}

		float dist = g_Options.misc_thirdperson_dist;

		QAngle *view = g_LocalPlayer->GetVAngles();
		trace_t tr;
		Ray_t ray;

		Vector desiredCamOffset = Vector(cos(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(-view->pitch)) * dist
		);

		//cast a ray from the Current camera Origin to the Desired 3rd person Camera origin
		ray.Init(g_LocalPlayer->GetEyePos(), (g_LocalPlayer->GetEyePos() - desiredCamOffset));
		CTraceFilter traceFilter;
		traceFilter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		Vector diff = g_LocalPlayer->GetEyePos() - tr.endpos;

		float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));// Pythagorean

		bool horOK = distance2D > (dist - 2.0f);
		bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

		float cameraDistance;

		if (horOK && vertOK)  // If we are clear of obstacles
		{
			cameraDistance = dist; // go ahead and set the distance to the setting
		}
		else
		{
			if (vertOK) // if the Vertical Axis is OK
			{
				cameraDistance = distance2D * 0.95f;
			}
			else// otherwise we need to move closer to not go into the floor/ceiling
			{
				cameraDistance = abs(diff.z) * 0.95f;
			}
		}
		g_Input->m_fCameraInThirdPerson = true;

		g_Input->m_vecCameraOffset.z = cameraDistance;
	}
	else
	{
		g_Input->m_fCameraInThirdPerson = false;
	}
}


void Visuals::AddToDrawList() {
	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
		auto entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;
		
		if (entity == g_LocalPlayer && !g_Input->m_fCameraInThirdPerson)
			continue;

		if (i <= g_GlobalVars->maxClients) {
			auto player = Player();
			if (player.Begin((C_BasePlayer*)entity)) {
				if (g_Options.esp_player_snaplines) player.RenderSnapline();
				if (g_Options.esp_player_boxes)     player.RenderBox();
				if (g_Options.esp_player_skeleton)  player.RenderSkeleton();
				if (g_Options.esp_player_weapons)   player.RenderWeaponName();
				if (g_Options.esp_player_names)     player.RenderName();
				if (g_Options.esp_player_health)    player.RenderHealth();
				if (g_Options.esp_player_armour)    player.RenderArmour();
			}
		}
		else if (g_Options.esp_dropped_weapons && entity->IsWeapon())
			RenderWeapon(static_cast<C_BaseCombatWeapon*>(entity));
		else if (g_Options.esp_dropped_weapons && entity->IsDefuseKit())
			RenderDefuseKit(entity);
		else if (entity->IsPlantedC4() && g_Options.esp_planted_c4)
			RenderPlantedC4(entity);
		else if (entity->IsLoot() && g_Options.esp_items)
			RenderItemEsp(entity);
	}


	if (g_Options.esp_crosshair)
		RenderCrosshair();

}
