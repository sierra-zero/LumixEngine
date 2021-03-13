#pragma once
#include "engine/allocator.h"
#include "engine/lumix.h"
#include "engine/math.h"
#include "engine/plugin.h"


struct dtCrowdAgent;


namespace Lumix
{


struct IAllocator;
template <typename T> struct DelegateList;


struct NavmeshZone {
	enum Flags {
		AUTOLOAD = 1 << 0,
		DETAILED = 1 << 1
	};
	Vec3 extents;
	u64 guid;
	u32 flags;
};

struct NavmeshBuildJob {
	~NavmeshBuildJob() {}
	virtual bool isFinished() = 0;
	virtual float getProgress() = 0;
};

struct NavigationScene : IScene
{
	static UniquePtr<NavigationScene> create(Engine& engine, IPlugin& system, Universe& universe, IAllocator& allocator);
	static void reflect();

	virtual NavmeshZone& getZone(EntityRef entity) = 0;
	virtual bool isZoneAutoload(EntityRef entity) = 0;
	virtual void setZoneAutoload(EntityRef entity, bool value) = 0;
	virtual bool isZoneDetailed(EntityRef entity) = 0;
	virtual void setZoneDetailed(EntityRef entity, bool value) = 0;
	virtual bool isFinished(EntityRef entity) = 0;
	virtual bool navigate(EntityRef entity, const struct DVec3& dest, float speed, float stop_distance) = 0;
	virtual void cancelNavigation(EntityRef entity) = 0;
	virtual void setActorActive(EntityRef entity, bool active) = 0;
	virtual float getAgentSpeed(EntityRef entity) = 0;
	virtual float getAgentYawDiff(EntityRef entity) = 0;
	virtual void setAgentRadius(EntityRef entity, float radius) = 0;
	virtual float getAgentRadius(EntityRef entity) = 0;
	virtual void setAgentHeight(EntityRef entity, float height) = 0;
	virtual float getAgentHeight(EntityRef entity) = 0;
	virtual bool getAgentMoveEntity(EntityRef entity) = 0;
	virtual void setAgentMoveEntity(EntityRef entity, bool value) = 0;
	virtual NavmeshBuildJob* generateNavmesh(EntityRef zone) = 0;
	virtual void free(NavmeshBuildJob* job) = 0;
	virtual bool generateTileAt(EntityRef zone, const DVec3& pos, bool keep_data) = 0;
	virtual bool loadZone(EntityRef zone_entity) = 0;
	virtual bool saveZone(EntityRef zone_entity) = 0;
	virtual void debugDrawNavmesh(EntityRef zone, const DVec3& pos, bool inner_boundaries, bool outer_boundaries, bool portals) = 0;
	virtual void debugDrawCompactHeightfield(EntityRef zone) = 0;
	virtual void debugDrawHeightfield(EntityRef zone) = 0;
	virtual void debugDrawContours(EntityRef zone) = 0;
	virtual void debugDrawPath(EntityRef agent_entity) = 0;
	virtual const dtCrowdAgent* getDetourAgent(EntityRef entity) = 0;
	virtual bool isNavmeshReady(EntityRef zone) const = 0;
	virtual bool hasDebugDrawData(EntityRef zoneko) const = 0;
	virtual void setGeneratorParams(float cell_size,
		float cell_height,
		float agent_radius,
		float agent_height,
		float walkable_angle,
		float max_climb) = 0;

};


} // namespace Lumix
