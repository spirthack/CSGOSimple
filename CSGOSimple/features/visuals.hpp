#pragma once

#include "../singleton.hpp"

#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/csgostructs.hpp"

struct DrawInfo {
	virtual void Draw() = 0;
	Color color;
};

struct Box_t : DrawInfo {
	float x1, y1, x2, y2;
	float th;
	bool filled = false;
	float rounding;
	Box_t(float x1, float y1, float x2, float y2, Color clr, float thickness = 1.f, bool filled = false, float rounding = 0.f) {
		this->x1 = x1, this->x2 = x2, this->y1 = y1, this->y2 = y2,
			this->th = thickness, this->filled = filled,
			this->color = clr, this->rounding = rounding;
	}
	virtual void Draw() {
		if (filled)
			Render::Get().RenderBoxFilled(x1, y1, x2, y2, color, th, rounding);
		else
			Render::Get().RenderBox(x1, y1, x2, y2, color, th, rounding);
	}
};

struct Text_t : DrawInfo {
	float x, y;
	float size;
	std::string text;
	bool center = false;

	Text_t(float x, float y, float size, std::string text, Color clr, bool center = false) {
		this->x = x, this->y = y, this->size = size,
			this->text = text, this->center = center,
			this->color = clr;
	}
	virtual void Draw() {
		Render::Get().RenderText(text, x, y, size, color, center);
	}
};

struct Line_t : DrawInfo {
	float x1;
	float y1;
	float x2;
	float y2;
	float thickness = 1.f;
	Line_t(float x1, float y1, float x2, float y2, Color clr, float th = 1.f) {
		this->x1 = x1, this->y1 = y1, this->x2 = x2,
			this->y2 = y2, this->thickness = th,
			this->color = clr;
	}
	virtual void Draw() {
		Render::Get().RenderLine(x1, y1, x2, y2, color, thickness);
	}
};

struct Circle_t : DrawInfo {
	float x, y;
	float radius;
	int points;
	float thickness = 1.f;
	bool filled = false;
	Circle_t(float x, float y, float r, int p, Color clr, float th = 1.f, bool filled = false) {
		this->x = x, this->y = y, this->radius = r,
			this->points = p, this->thickness = th, this->filled,
			this->color = clr;
	}
	virtual void Draw() {
		if (filled)
			Render::Get().RenderCircleFilled(x, y, radius, color);
		else
			Render::Get().RenderCircle(x, y, radius, color);
	}
};

struct Circle3D_t : DrawInfo {
	Vector position;
	float points;
	float radius;
	Circle3D_t(Vector pos, float p, float r, Color clr) {
		this->position = pos, this->points = p, this->radius = r,
			this->color = clr;
	}
	virtual void Draw() {
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
			Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

			Vector start2d, end2d;
			if (!Math::WorldToScreen(start, start2d) || !Math::WorldToScreen(end, end2d))
				return;

			Render::Get().RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
		}
	}
};
class RenderList {
	std::vector<DrawInfo*> renderQueue;
public:
	void Clear() {
		renderQueue.clear();
	}
	void Add(DrawInfo* f) {
		renderQueue.push_back(f);
	}
	void Render() {
		for (auto& f : renderQueue)
			f->Draw();
	}
};

class Visuals : public Singleton<Visuals>
{
	friend class Singleton<Visuals>;

	CRITICAL_SECTION cs;

	Visuals();
	~Visuals();
public:
    class Player
    {
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_visible;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		} esp_ctx;
	public:
        bool Begin(C_BasePlayer* pl);

        void RenderBox();
        void RenderName();
        void RenderHealth();
        void RenderArmour();
        void RenderWeapon();
        void RenderSnapline();
	};

	void RenderCrosshair();
	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void ThirdPerson();

public:
	void OnPaintTraverse();

	void Render();
};
