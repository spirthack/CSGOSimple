#include "../../CSGOSimple/valve_sdk/csgostructs.hpp"
#include "../options.hpp"
namespace Animations
{
	struct AnimationRecord
	{
		AnimationLayer BackupLayers[15];
		bool OverrideAnimations;
	};

	extern AnimationRecord AnimationRecord_t[65];

	void AccurateLocalPlayer();
}