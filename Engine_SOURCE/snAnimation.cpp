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
			sprite.leftTop.x = (leftTop.x + (i * size.x)) / width;  //UV��ǥ��� ��
			sprite.leftTop.y = leftTop.y / height;  //UV��ǥ��� ��
			sprite.size.x = size.x / width;    //UV��ǥ��� ��
			sprite.size.y = size.y / height;   //UV��ǥ��� ��
			sprite.offset = offset;
			sprite.atlasSize = Vector2(atlasSize / width, atlasSize / height); //�տ� ���� ���ڿ� ���� atlasSize�� UV��ǥ��� Ŀ���ų� �۾����ų� ��.
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

		data.spriteLeftTop = mSprites[mIndex].leftTop; //UV��ǥ���� ���� ��
		data.spriteSize = mSprites[mIndex].size;			//UV��ǥ���� �ִϸ��̼� �� �������� ũ��
		data.spriteOffset = mSprites[mIndex].offset;		//UV��ǥ���� �ִϸ��̼� ������
		data.atlasSize = mSprites[mIndex].atlasSize;		//UV��ǥ���� ��Ʋ�� ũ��
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