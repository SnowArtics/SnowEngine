#include "snRenderer.h"
#include "snConstantBuffer.h"
#include "snResources.h"
#include "snTexture.h"
#include "snMaterial.h"

namespace renderer {
	using namespace sn;
	using namespace sn::graphics;

	sn::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[(UINT)eSamplerType::End] = {};

	//
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};

	//
	sn::Camera* mainCamera = nullptr;
	std::vector<sn::Camera*> cameras = {};
	std::vector<DebugMesh> debugMeshs = {};

	void LoadMesh() {
		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indexes = {};

		//RECT
		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		// Vertex Buffer
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(vertexes.data(), vertexes.size());

		//여기서 인덱스 버퍼를 초기화해준다.
		//인덱스 버퍼는 그리는 순서를 전달해 주는 것이므로, 정수 데이터만 보내면 된다.
		//버텍스 버퍼는 속성이 여러가지니까 구조체를 보내줌
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		// Index Buffer
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		// Rect Debug Mesh
		std::shared_ptr<Mesh> rectDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugRect", rectDebug);
		rectDebug->CreateVertexBuffer(vertexes.data(), vertexes.size());
		rectDebug->CreateIndexBuffer(indexes.data(), indexes.size());

		// Circle Debug Mesh
		vertexes.clear();
		indexes.clear();

		Vertex center = {};
		center.pos = Vector3(0.0f, 0.0f, 0.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes.push_back(center);

		int iSlice = 40;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;

		for (int i = 0; i < iSlice; ++i)
		{
			center.pos = Vector3(fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, 0.0f);
			center.color = Vector4(0.0f, 1.0f, 0.0f, 1.f);
			vertexes.push_back(center);
		}

		//for (UINT i = 0; i < (UINT)iSlice; ++i)
		//{
		//	indexes.push_back(0);
		//	if (i == iSlice - 1)
		//	{
		//		indexes.push_back(1);
		//	}
		//	else
		//	{
		//		indexes.push_back(i + 2);
		//	}
		//	indexes.push_back(i + 1);
		//}

		for (int i = 0; i < vertexes.size() - 2; ++i)
		{
			indexes.push_back(i + 1);
		}
		indexes.push_back(1);

		std::shared_ptr<Mesh> circleDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugCircle", circleDebug);
		circleDebug->CreateVertexBuffer(vertexes.data(), vertexes.size());
		circleDebug->CreateIndexBuffer(indexes.data(), indexes.size());
	}

	void LoadBuffer()
	{
		// Transform Constant Buffer
		constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));			

		// Grid Buffer
		constantBuffer[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffer[(UINT)eCBType::Grid]->Create(sizeof(TransformCB));
	
		// Grid Buffer
		constantBuffer[(UINT)eCBType::Animator] = new ConstantBuffer(eCBType::Animator);
		constantBuffer[(UINT)eCBType::Animator]->Create(sizeof(AnimatorCB));
	}

	void LoadShader()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");
		sn::Resources::Insert(L"TriangleShader", shader);

		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		sn::Resources::Insert(L"SpriteShader", spriteShader);

		std::shared_ptr<Shader> spriteAniShader = std::make_shared<Shader>();
		spriteAniShader->Create(eShaderStage::VS, L"SpriteAnimationVS.hlsl", "main");
		spriteAniShader->Create(eShaderStage::PS, L"SpriteAnimationPS.hlsl", "main");
		sn::Resources::Insert(L"SpriteAnimationShader", spriteAniShader);

		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		gridShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		gridShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");
		sn::Resources::Insert(L"GridShader", gridShader);

		std::shared_ptr<Shader> debugShader = std::make_shared<Shader>();
		debugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		debugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		debugShader->SetRSState(eRSType::WireframeNone);
		//debugShader->SetDSState(eDSType::NoWrite);
		sn::Resources::Insert(L"DebugShader", debugShader);
	}

	void LoadMaterial()
	{
		std::shared_ptr<Shader> spriteShader
			= Resources::Find<Shader>(L"SpriteShader");
		
		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		Resources::Insert(L"SpriteMaterial", material);

		texture = Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");
		material = std::make_shared<Material>();
		material->SetShader(spriteShader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteMaterial02", material);

		std::shared_ptr<Shader> animSpriteShader
			= Resources::Find<Shader>(L"SpriteAnimationShader");
		material = std::make_shared<Material>();
		material->SetShader(animSpriteShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"SpriteAnimaionMaterial", material);
		
		std::shared_ptr<Shader> gridShader
			= Resources::Find<Shader>(L"GridShader");

		material = std::make_shared<Material>();
		material->SetShader(gridShader);
		Resources::Insert(L"GridMaterial", material);

		std::shared_ptr<Shader> debugShader
			= Resources::Find<Shader>(L"DebugShader");

		material = std::make_shared<Material>();
		material->SetShader(debugShader);
		Resources::Insert(L"DebugMaterial", material);
#pragma region Title
		{
			//타이틀화면 배경 이미지 1 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackground_1", L"..\\Resources\\Texture\\Title\\Main_menu_1_1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"TitleBackgroundMaterial01", spriteMaterial);
		}
		{
			//타이틀화면 배경 이미지 2 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackground_2", L"..\\Resources\\Texture\\Title\\Main_menu_2_1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"TitleBackgroundMaterial02", spriteMaterial);
		}
		{
			//타이틀화면 로고 이미지 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Logo01", L"..\\Resources\\Texture\\UI\\Logo\\logo.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"LogoMaterial01", spriteMaterial);
		}
#pragma endregion
#pragma region village
		{
			//마을 배경 이미지 1 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"VillageBackground_1", L"..\\Resources\\Texture\\Village\\Village_Background.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"VillageBackgroundMaterial01", spriteMaterial);
		}
		{
			//윌의 집 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Build_WiilsHome01", L"..\\Resources\\Texture\\Village\\Build_WillsHome.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_WiilsHomeMaterial01", spriteMaterial);
		}
		{
			//윌의 왼쪽 집 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Build_Top01", L"..\\Resources\\Texture\\Village\\Build_Top1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_Top1Material01", spriteMaterial);

			texture
				= Resources::Load<Texture>(L"Build_Awning01", L"..\\Resources\\Texture\\Village\\Build_Awning.png");
			spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_AwningMaterial01", spriteMaterial);
		}
		{
			//대장간 건물 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Build_Forge01", L"..\\Resources\\Texture\\Village\\Build_Forge.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_ForgeMaterial01", spriteMaterial);
		}
		{
			//마을 간판 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BuildBoard", L"..\\Resources\\Texture\\Village\\BuildBoard.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"BuildBoardMaterial01", spriteMaterial);
		}
#pragma endregion
#pragma region dungeon
		{
			//던전 배경 이미지 1 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBackground_1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial01", spriteMaterial);
		}
		{
			//던전 배경 이미지 2 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBackground_2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial02", spriteMaterial);
		}
		{
			//던전 배경 이미지 3 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBackground_3", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial03", spriteMaterial);
		}
		{
			//보스룸 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBoss_1", L"..\\Resources\\Texture\\Dungeon\\Background\\bossRoom.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBossRoom01", spriteMaterial);
		}
#pragma region dungeonObject
		{
			//fountion
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"fountain", L"..\\Resources\\Texture\\Dungeon\\Object\\fountain.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"FountainMaterial", spriteMaterial);
		}
		{
			//lathe
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"lathe", L"..\\Resources\\Texture\\Dungeon\\Object\\lathe.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"LatheMaterial", spriteMaterial);
		}
		{
			//pillar
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"pillar", L"..\\Resources\\Texture\\Dungeon\\Object\\pillar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PillarMaterial", spriteMaterial);
		}
		{
			//pot_slime
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"pot", L"..\\Resources\\Texture\\Dungeon\\Object\\pot.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PotMetrial", spriteMaterial);
		}
		{
			//fountion
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"pot_slime", L"..\\Resources\\Texture\\Dungeon\\Object\\pot_slime.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Pot_slimeMaterial", spriteMaterial);
		}
		{
			//skull1
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"skull1", L"..\\Resources\\Texture\\Dungeon\\Object\\skull1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Skull1Material", spriteMaterial);
		}
		{
			//skull2
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"skull2", L"..\\Resources\\Texture\\Dungeon\\Object\\skull2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Skull2Material", spriteMaterial);
		}
		{
			//skull3
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"skull3", L"..\\Resources\\Texture\\Dungeon\\Object\\skull3.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Skull3Material", spriteMaterial);
		}
		{
			//smallRock
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"smallRock", L"..\\Resources\\Texture\\Dungeon\\Object\\smallRock.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"SmallRockMaterial", spriteMaterial);
		}
		{
			//smallRock_slime
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"smallRock_slime", L"..\\Resources\\Texture\\Dungeon\\Object\\smallRock_slime.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"SmallRock_slimeMaterial", spriteMaterial);
		}
#pragma endregion
#pragma endregion
#pragma region dungeonEntrance
		{
			//던전 입구 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonEntrance01", L"..\\Resources\\Texture\\DungeonEntrance\\Dungeon_Entrance.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonEntranceMaterial01", spriteMaterial);
		}
#pragma endregion
#pragma region shop
		{
			//Shop Background 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"ShopBackground01", L"..\\Resources\\Texture\\Shop\\shop_background.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"ShopBackgroundMaterial01", spriteMaterial);
		}
#pragma endregion
#pragma region UI
		{
			//UI Base01 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIBase01", L"..\\Resources\\Texture\\UI\\Base\\UI_base01.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIBaseMaterial01", spriteMaterial);
		}
		{
			//UI Base02 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIBase02", L"..\\Resources\\Texture\\UI\\Base\\UI_base02.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIBaseMaterial02", spriteMaterial);
		}
		{
			//UI Gold 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIGold01", L"..\\Resources\\Texture\\UI\\Base\\UI_gold.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIGoldMaterial01", spriteMaterial);
		}
		{
			//UI weapon1 space 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIWeapon01", L"..\\Resources\\Texture\\UI\\Base\\UI_weapon_1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIWeaponMaterial01", spriteMaterial);
		}
		{
			//UI weapon2 space 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIWeapon02", L"..\\Resources\\Texture\\UI\\Base\\UI_weapon_2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIWeaponMaterial02", spriteMaterial);
		}
		{
			//UI base weapon sword and shield 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIWeapon02", L"..\\Resources\\Texture\\UI\\Base\\Sword.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UISword01", spriteMaterial);
		}
		{
			//inventory base 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIInventoryBase01", L"..\\Resources\\Texture\\UI\\Inventory\\Inventory_base.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIInventoryBaseMaterial01", spriteMaterial);
		}
#pragma endregion
#pragma region Player


#pragma endregion
	}

	void SetupState() {
#pragma region InputLayout
		// Input layout 정점 구조 정보를 넘겨줘야한다.
		D3D11_INPUT_ELEMENT_DESC arrLayout[3] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 28;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;


		std::shared_ptr<Shader> shader = sn::Resources::Find<Shader>(L"TriangleShader");
		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = sn::Resources::Find<Shader>(L"SpriteShader");
		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = sn::Resources::Find<Shader>(L"GridShader");
		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = sn::Resources::Find<Shader>(L"DebugShader");
		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = sn::Resources::Find<Shader>(L"SpriteAnimationShader");
		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
#pragma endregion
#pragma region Sampler State
		//Sampler State
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 0, samplerState[(UINT)eSamplerType::Point].GetAddressOf());

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSampler(eShaderStage::PS, 1, samplerState[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizeState(&rasterizerDesc
			, rasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion

#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

		//less
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::Less].GetAddressOf());

		//lessEqual
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::LessEqual].GetAddressOf());

		//Greater
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

		//No Write
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		//None
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&depthStencilDesc
			, depthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion

#pragma region Blend State
		D3D11_BLEND_DESC blendDesc = {};

		//default
		blendStates[(UINT)eBSType::Default] = nullptr;

		// Alpha Blend
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&blendDesc
			, blendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		// one one
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&blendDesc
			, blendStates[(UINT)eBSType::OneOne].GetAddressOf());

#pragma endregion
	}

	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadShader();
		SetupState();
		LoadMaterial();
	}

	void PushDebugMeshInfo(DebugMesh mesh)
	{
		debugMeshs.push_back(mesh);
	}

	void Render()
	{
		for (Camera* cam : cameras)
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras.clear();
	}

	void Release()
	{
		//버퍼, Blob, Shader 관련 객체 포인터들은 다이렉트X가 제공해주는 Release() 함수가 있다. 그걸 쓴다.		
		for (ConstantBuffer* buff : constantBuffer)
		{
			if (buff == nullptr)
				continue;

			delete buff;
			buff = nullptr;
		}
	}
}