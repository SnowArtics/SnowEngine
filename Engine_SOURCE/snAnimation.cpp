#include "snAnimation.h"
#include "snTime.h"
#include "snAnimator.h"
#include "snRenderer.h"
#include "snConstantBuffer.h"

namespace sn
{
	Animation::Animation()
		: Resource(enums::eResourceType::Animation)
		, mAtlas(nullptr)
		, mAnimator(nullptr)
		, mSprites{}
		, mIndex(-1)
		, mTime(0.0f)
		, mbComplete(false)
	{

	}

	Animation::~Animation()
	{
	}
	void Animation::Update()
	{
	}
	void Animation::LateUpdate()
	{
		if (mbComplete)
			return;

		mTime += Time::DeltaTime();

		if (mSprites[mIndex].duration <= mTime)
		{
			mIndex++;
			mTime = 0.0f;

			if (mSprites.size() <= mIndex)
			{
				mIndex = mSprites.size() - 1;
				mbComplete = true;
			}
		}
	}
	void Animation::Render()
	{
	}
	void Animation::Create(std::wstring name
		, std::shared_ptr<graphics::Texture> atlas
		, Vector2 leftTop //0,0
		, Vector2 size     //120,130
		, UINT columnLength //3
		, float atlasSize
		, float duration
		, Vector2 offset)
	{
		SetKey(name);
		mAtlas = atlas;

		float width = (float)atlas->GetWidth();//1200
		float height = (float)atlas->GetHeight();//1040

		for (size_t i = 0; i < columnLength; i++)//3
		{
			Sprite sprite = {};
			sprite.leftTop.x = (leftTop.x + (i * size.x)) / width;  //UV좌표계로 들어감
			sprite.leftTop.y = leftTop.y / height;  //UV좌표계로 들어감
			sprite.size.x = size.x / width;    //UV좌표계로 들어감
			sprite.size.y = size.y / height;   //UV좌표계로 들어감
			sprite.offset = offset;
			sprite.atlasSize = Vector2(atlasSize / width, atlasSize / height); //앞에 넣은 숫자에 따라 atlasSize가 UV좌표계로 커지거나 작아지거나 함.
			sprite.duration = duration;

			mSprites.push_back(sprite);
		}
	}

	void Animation::Binds()
	{
		// texture bind
		mAtlas->BindShaderResource(graphics::eShaderStage::PS, 12);

		// AnimationCB
		renderer::AnimatorCB data = {};

		data.spriteLeftTop = mSprites[mIndex].leftTop; //UV좌표계의 왼쪽 위
		data.spriteSize = mSprites[mIndex].size;			//UV좌표계의 애니메이션 한 프레임의 크기
		data.spriteOffset = mSprites[mIndex].offset;		//UV좌표계의 애니메이션 오프셋
		data.atlasSize = mSprites[mIndex].atlasSize;		//UV좌표계의 아틀라스 크기
		data.animationType = 1;

		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Animator];
		cb->SetData(&data);

		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);

	}
	void Animation::Reset()
	{
		mTime = 0.0f;
		mbComplete = false;
		mIndex = 0;
	}
}