#pragma once
#include "snEntity.h"
#include "snGraphics.h"

namespace sn::graphics
{
	class StructedBuffer : public GpuBuffer
	{
	public:
		StructedBuffer();
		~StructedBuffer();

		//구조화 버퍼를 만들 때 쓰는 함수, 만들 때 없을 수도 있으니까 설정해준다.
		bool Create(UINT size, UINT stride, eSRVType type);
		//만든 구조화 버퍼에 데이터를 세팅해준다. 배열로 세팅할 수도 있으니까 몇개 만들 건지 파라미터 생성
		void SetData(void* data, UINT stride);
		//GPU에 묶어준다.
		void Bind(eShaderStage stage, UINT slot);

		//Get 함수 생성
		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		//상수버퍼를 GPU에 묶을 수 있었던 이유는 ShaderResource View가 존재하기 때문이다.
		//기존에 GraphicDevice에 존재하던걸 이용해서 묶어줬다. 구조화 버퍼용을 하나 생성해서
		//묶어줄 것이다.
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		eSRVType mType;

		UINT mSize;//구조체 버퍼의 크기
		UINT mStride;//구조체 버퍼를 배열로 보낼 때 크기
	};
}
