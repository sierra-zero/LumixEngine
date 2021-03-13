#pragma once


#include "engine/lumix.h"
#include "engine/array.h"
#include "engine/resource.h"
#include "engine/resource_manager.h"
#include "engine/stream.h"


namespace Lumix
{


struct DVec3;
struct Material;
struct Renderer;


struct ParticleEmitterResource final : Resource
{
	struct Header {
		static constexpr u32 MAGIC = '_LPA';
		const u32 magic = MAGIC;
		u32 version = 0;
	};
	
	struct DataStream {
		enum Type : u8 {
			NONE,
			CHANNEL,
			CONST,
			OUT,
			REGISTER,
			LITERAL
		};

		Type type = NONE;
		u8 index;
		float value;
	};

	enum class InstructionType : u8{
		END,
		ADD,
		COS,
		SIN,
		FREE0,
		SUB,
		FREE1,
		MUL,
		MULTIPLY_ADD,
		LT,
		MOV,
		RAND,
		KILL,
		EMIT,
		GT,
		MIX,
		GRADIENT
	};

	static const ResourceType TYPE;

	ParticleEmitterResource(const Path& path, ResourceManager& manager, Renderer& renderer, IAllocator& allocator);

	ResourceType getType() const override { return TYPE; }
	void unload() override;
	bool load(u64 size, const u8* mem) override;
	const OutputMemoryStream& getInstructions() const { return m_instructions; }
	int getEmitOffset() const { return m_emit_offset; }
	int getOutputOffset() const { return m_output_offset; }
	int getChannelsCount() const { return m_channels_count; }
	int getRegistersCount() const { return m_registers_count; }
	int getOutputsCount() const { return m_outputs_count; }
	Material* getMaterial() const { return m_material; }
	void setMaterial(const Path& path);
	void overrideData(OutputMemoryStream&& instructions,
		int emit_offset,
		int output_offset,
		int channels_count,
		int registers_count,
		int outputs_count
	);

private:
	OutputMemoryStream m_instructions;
	u32 m_emit_offset;
	u32 m_output_offset;
	u32 m_channels_count;
	u32 m_registers_count;
	u32 m_outputs_count;
	Material* m_material;
};


struct ResourceManagerHub;


struct LUMIX_RENDERER_API ParticleEmitter
{
public:
	ParticleEmitter(EntityPtr entity, IAllocator& allocator);
	~ParticleEmitter();

	void serialize(OutputMemoryStream& blob);
	void deserialize(InputMemoryStream& blob, ResourceManagerHub& manager);
	void update(float dt);
	void emit(const float* args);
	void fillInstanceData(float* data);
	int getInstanceDataSizeBytes() const;
	ParticleEmitterResource* getResource() const { return m_resource; }
	void setResource(ParticleEmitterResource* res);
	int getInstancesCount() const { return m_instances_count; }
	float* getChannelData(int idx) const { return m_channels[idx].data; }
	void reset() { m_particles_count = 0; }

	EntityPtr m_entity;
	u32 m_emit_rate = 10;
	u32 m_particles_count = 0;
	float m_constants[16];

private:
	struct Channel
	{
		alignas(16) float* data = nullptr;
		u32 name = 0;
	};

	float readSingleValue(InputMemoryStream& blob) const;

	IAllocator& m_allocator;
	OutputMemoryStream m_emit_buffer;
	Channel m_channels[16];
	int m_capacity = 0;
	int m_instances_count = 0;
	float m_emit_timer = 0;
	ParticleEmitterResource* m_resource = nullptr;
};


} // namespace Lumix