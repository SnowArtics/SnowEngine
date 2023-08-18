#pragma once

namespace sn::enums
{
	enum class eComponentType
	{
		None,
		Transform,
		FSM,
		Mesh,
		Collider2D,
		Particle,
		MeshRenderer,
		Animator,
		Script,
		Light,
		Camera,
		AudioListener,
		AudioSource,
		AI,
		End,
	};

	enum class eLayerType
	{
		Grid,
		Background,
		BackObject,
		Obstacle,
		DesObstacle,
		Door,
		Monster,
		Player,
		Light,
		UI,

		Camera,
		End = 16,
	};

	enum class eResourceType
	{
		Texture,
		Shader,
		Material,
		Mesh,
		Animation,
		ComputeShader,
		AudioClip,
		End,
	};

	enum class eColliderType
	{
		Rect,
		Circle,
		Sphere,
		Cube,
		End,
	};

	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};

	enum class eEventType {
		CREATE_OBJECT,
		DELETE_OBJECT,
		SCENE_CHANGE,
		CHANGE_AI_STATE,




		END,
	};
}