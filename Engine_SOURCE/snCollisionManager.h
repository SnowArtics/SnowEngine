#pragma once
#include "SnowEngine.h"

namespace sn
{
#define LAYER_MAX (UINT)eLayerType::End 
	using namespace enums;

	class Collider2D;
	class CollisionManager
	{
	public:
		union ColliderID
		{
			struct
			{
				UINT left;
				UINT right;
			};
			UINT64 id;
		};

		static void Initialize();
		static void Update();
		static void LayerCollision(eLayerType left, eLayerType right);
		static void ColliderCollision(Collider2D* left, Collider2D* right);
		static bool Intersect(Collider2D* left, Collider2D* right);

		static void SetLayer(eLayerType left, eLayerType right, bool enable);
		static void Clear();
		
		static bool RectToRectCollision(Collider2D* left, Collider2D* right);
		static bool CircleToCircleCollision(Collider2D* left, Collider2D* right);
		static bool CircleToRectCollision(Collider2D* left, Collider2D* right);
		static float Magnitude(float x, float y, float z);

	private:

		static std::bitset<LAYER_MAX> mMatrix[LAYER_MAX];
		static std::map<UINT64, bool> mCollisionMap;
	};
}