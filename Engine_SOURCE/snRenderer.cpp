#include "snRenderer.h"
#include "snConstantBuffer.h"
#include "snResources.h"
#include "snTexture.h"
#include "snMaterial.h"
#include "snStructedBuffer.h"
#include "snPaintShader.h"
#include "snParticleShader.h"

namespace renderer {
	using namespace sn;
	using namespace sn::graphics;

	sn::graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[(UINT)eSamplerType::End] = {};

	//
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};

	// light
	std::vector<Light*> lights = {};
	StructedBuffer* lightsBuffer = nullptr;

	//
	sn::Camera* mainCamera = nullptr;
	std::vector<sn::Camera*> cameras = {};
	std::vector<DebugMesh> debugMeshs = {};

	void LoadMesh() {
		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indexes = {};

		// PointMesh
		Vertex v = {};
		v.pos = Vector3(0.0f, 0.0f, 0.0f);
		vertexes.push_back(v);
		indexes.push_back(0);
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());
		Resources::Insert(L"PointMesh", mesh);


		vertexes.clear();
		indexes.clear();

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
		mesh = std::make_shared<Mesh>();
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

		// Editor Buffer
		constantBuffer[(UINT)eCBType::Editor] = new ConstantBuffer(eCBType::Editor);
		constantBuffer[(UINT)eCBType::Editor]->Create(sizeof(EditorCB));

		//ParticleCB
		constantBuffer[(UINT)eCBType::Particle] = new ConstantBuffer(eCBType::Particle);
		constantBuffer[(UINT)eCBType::Particle]->Create(sizeof(ParticleCB));

		//NoiseCB
		constantBuffer[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffer[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

		//MonsterCB
		constantBuffer[(UINT)eCBType::Monster] = new ConstantBuffer(eCBType::Monster);
		constantBuffer[(UINT)eCBType::Monster]->Create(sizeof(MonsterCB));

		// light structed buffer
		lightsBuffer = new StructedBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 2, eViewType::SRV, nullptr, true);
	}

	void LoadShader()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");
		sn::Resources::Insert(L"TriangleShader", shader);

		shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		sn::Resources::Insert(L"SpriteShader", shader);

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


		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		sn::Resources::Insert(L"PaintShader", paintShader);

		std::shared_ptr<ParticleShader> psSystemShader = std::make_shared<ParticleShader>();
		psSystemShader->Create(L"ParticleCS.hlsl", "main");
		sn::Resources::Insert(L"ParticleSystemShader", psSystemShader);

		std::shared_ptr<Shader> paritcleShader = std::make_shared<Shader>();
		paritcleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
		paritcleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
		paritcleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
		paritcleShader->SetRSState(eRSType::SolidNone);
		paritcleShader->SetDSState(eDSType::NoWrite);
		paritcleShader->SetBSState(eBSType::AlphaBlend);
		paritcleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		sn::Resources::Insert(L"ParticleShader", paritcleShader);
	}

	void LoadTexture() {
		//paint texture
		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		sn::Resources::Insert(L"PaintTexuture", uavTexture);

		std::shared_ptr<Texture> particle = std::make_shared<Texture>();
		Resources::Load<Texture>(L"CartoonSmoke", L"..\\Resources\\particle\\CartoonSmoke.png");

		Resources::Load<Texture>(L"Noise01", L"..\\Resources\\noise\\noise_01.png");
		Resources::Load<Texture>(L"Noise02", L"..\\Resources\\noise\\noise_02.png");
		Resources::Load<Texture>(L"Noise03", L"..\\Resources\\noise\\noise_03.png");

		Resources::Load<Texture>(L"Moonlighter_Effect01", L"..\\Resources\\particle\\Moonlighter_Effect01.png");
	}

	void LoadMaterial()
	{
		std::shared_ptr<Shader> shader
			= Resources::Find<Shader>(L"SpriteShader");
		
		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"Link", L"..\\Resources\\Texture\\Link.png");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		Resources::Insert(L"SpriteMaterial", material);

		//texture = Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");
		texture = Resources::Find<Texture>(L"PaintTexuture");
		material = std::make_shared<Material>();
		material->SetShader(shader);
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

		std::shared_ptr<Shader> particleShader
			= Resources::Find<Shader>(L"ParticleShader");
		material = std::make_shared<Material>();
		material->SetShader(particleShader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		std::shared_ptr<Texture> particleTexx
			= Resources::Find<Texture>(L"Moonlighter_Effect01");
		material->SetTexture(particleTexx);
		Resources::Insert(L"ParticleMaterial", material);

		//std::shared_ptr<Shader> debugShader
		//	= Resources::Find<Shader>(L"DebugShader");

		//material = std::make_shared<Material>();
		//material->SetShader(debugShader);
		//Resources::Insert(L"PaintMaterial", material)
#pragma region Title
		{
			//타이틀화면 배경 이미지 1 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackground_1", L"..\\Resources\\Texture\\Title\\Main_menu_1_1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"TitleBackgroundMaterial01", spriteMaterial);
		}
		{
			//타이틀화면 배경 이미지 2 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackground_2", L"..\\Resources\\Texture\\Title\\Main_menu_2_1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"TitleBackgroundMaterial02", spriteMaterial);
		}
		{
			//타이틀화면 로고 이미지 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Logo01", L"..\\Resources\\Texture\\UI\\Logo\\logo.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
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
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"VillageBackgroundMaterial01", spriteMaterial);
		}
		{
			//윌의 집 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Build_WiilsHome01", L"..\\Resources\\Texture\\Village\\Build_WillsHome.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_WiilsHomeMaterial01", spriteMaterial);
		}
		{
			//윌의 왼쪽 집 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Build_Top01", L"..\\Resources\\Texture\\Village\\Build_Top1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_Top1Material01", spriteMaterial);

			texture
				= Resources::Load<Texture>(L"Build_Awning01", L"..\\Resources\\Texture\\Village\\Build_Awning.png");
			spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_AwningMaterial01", spriteMaterial);
		}
		{
			//대장간 건물 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Build_Forge01", L"..\\Resources\\Texture\\Village\\Build_Forge.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
			Resources::Insert(L"Build_ForgeMaterial01", spriteMaterial);
		}
		{
			//마을 간판 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BuildBoard", L"..\\Resources\\Texture\\Village\\BuildBoard.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
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
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial01", spriteMaterial);
		}
		{
			//던전 배경 이미지 2 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBackground_2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial02", spriteMaterial);
		}
		{
			//던전 배경 이미지 3 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBackground_3", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial03", spriteMaterial);
		}
		{
			//보스룸 메테리얼 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DungeonBoss_1", L"..\\Resources\\Texture\\Dungeon\\Background\\bossRoom.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBossRoom01", spriteMaterial);
		}
#pragma region dungeonBackground1
		{
			//던전 배경 0-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial0", spriteMaterial);
		}
		{
			//던전 배경 0-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background0-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background0-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial00-0", spriteMaterial);
		}
		{
			//던전 배경 1-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background1-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background1-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial01-0", spriteMaterial);
		}
		{
			//던전 배경 2-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background2-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background2-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial02-0", spriteMaterial);
		}
		{
			//던전 배경 3-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background3-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background3-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial03-0", spriteMaterial);
		}
		{
			//던전 배경 4-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background4-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background4-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial04-0", spriteMaterial);
		}
		{
			//던전 배경 5-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background5-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background5-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial05-0", spriteMaterial);
		}
		{
			//던전 배경 6-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background6-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background6-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial06-0", spriteMaterial);
		}
		{
			//던전 배경 7-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background7-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background7-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial07-0", spriteMaterial);
		}
		{
			//던전 배경 8-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background8-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background8-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial08-0", spriteMaterial);
		}
		{
			//던전 배경 9-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background9-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background9-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial09-0", spriteMaterial);
		}
		{
			//던전 배경 10-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background10-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background10-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial10-0", spriteMaterial);
		}
		{
			//던전 배경 11-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background11-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background11-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial11-0", spriteMaterial);
		}
		{
			//던전 배경 12-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background12-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background12-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial12-0", spriteMaterial);
		}
		{
			//던전 배경 13-0생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background13-0", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background13-0.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial13-0", spriteMaterial);
		}
#pragma endregion
#pragma region dungeonBackground2
		{
			//던전 배경 0-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background0-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background0-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial00-1", spriteMaterial);
		}
		{
			//던전 배경 1-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background1-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background1-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial01-1", spriteMaterial);
		}
		{
			//던전 배경 2-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background2-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background2-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial02-1", spriteMaterial);
		}
		{
			//던전 배경 3-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background3-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background3-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial03-1", spriteMaterial);
		}
		{
			//던전 배경 4-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background4-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background4-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial04-1", spriteMaterial);
		}
		{
			//던전 배경 5-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background5-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background5-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial05-1", spriteMaterial);
		}
		{
			//던전 배경 6-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background6-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background6-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial06-1", spriteMaterial);
		}
		{
			//던전 배경 7-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background7-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background7-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial07-1", spriteMaterial);
		}
		{
			//던전 배경 8-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background8-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background8-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial08-1", spriteMaterial);
		}
		{
			//던전 배경 9-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background9-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background9-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial09-1", spriteMaterial);
		}
		{
			//던전 배경 10-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background10-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background10-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial10-1", spriteMaterial);
		}
		{
			//던전 배경 11-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background11-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background11-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial11-1", spriteMaterial);
		}
		{
			//던전 배경 12-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background12-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background12-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial12-1", spriteMaterial);
		}
		{
			//던전 배경 13-1생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background13-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background13-1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial13-1", spriteMaterial);
		}
#pragma endregion
#pragma region dungeonBackground3
		{
			//던전 배경 0-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background0-1", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background0-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial00-2", spriteMaterial);
		}
		{
			//던전 배경 1-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background1-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background1-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial01-2", spriteMaterial);
		}
		{
			//던전 배경 2-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background2-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background2-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial02-2", spriteMaterial);
		}
		{
			//던전 배경 3-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background3-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background3-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial03-2", spriteMaterial);
		}
		{
			//던전 배경 4-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background4-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background4-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial04-2", spriteMaterial);
		}
		{
			//던전 배경 5-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background5-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background5-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial05-2", spriteMaterial);
		}
		{
			//던전 배경 6-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background6-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background6-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial06-2", spriteMaterial);
		}
		{
			//던전 배경 7-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background7-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background7-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial07-2", spriteMaterial);
		}
		{
			//던전 배경 8-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background8-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background8-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial08-2", spriteMaterial);
		}
		{
			//던전 배경 9-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background9-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background9-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial09-2", spriteMaterial);
		}
		{
			//던전 배경 10-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background10-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background10-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial10-2", spriteMaterial);
		}
		{
			//던전 배경 11-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background11-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background11-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial11-2", spriteMaterial);
		}
		{
			//던전 배경 12-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background12-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background12-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial12-2", spriteMaterial);
		}
		{
			//던전 배경 13-2생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"dungeon_background13-2", L"..\\Resources\\Texture\\Dungeon\\Background\\dungeon_background13-2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"DungeonBackgroundMaterial13-2", spriteMaterial);
		}
#pragma endregion
#pragma region dungeonObject
		{
			//fountion
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"fountain", L"..\\Resources\\Texture\\Dungeon\\Object\\fountain.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"FountainMaterial", spriteMaterial);
		}
		{
			//lathe
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"lathe", L"..\\Resources\\Texture\\Dungeon\\Object\\lathe.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"LatheMaterial", spriteMaterial);
		}
		{
			//pillar
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"pillar", L"..\\Resources\\Texture\\Dungeon\\Object\\pillar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PillarMaterial", spriteMaterial);
		}
		{
			//pot_slime
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"pot", L"..\\Resources\\Texture\\Dungeon\\Object\\pot.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PotMetrial", spriteMaterial);
		}
		{
			//fountion
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"pot_slime", L"..\\Resources\\Texture\\Dungeon\\Object\\pot_slime.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Pot_slimeMaterial", spriteMaterial);
		}
		{
			//skull1
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"skull1", L"..\\Resources\\Texture\\Dungeon\\Object\\skull1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Skull1Material", spriteMaterial);
		}
		{
			//skull2
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"skull2", L"..\\Resources\\Texture\\Dungeon\\Object\\skull2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Skull2Material", spriteMaterial);
		}
		{
			//skull3
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"skull3", L"..\\Resources\\Texture\\Dungeon\\Object\\skull3.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"Skull3Material", spriteMaterial);
		}
		{
			//smallRock
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"smallRock", L"..\\Resources\\Texture\\Dungeon\\Object\\smallRock.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"SmallRockMaterial", spriteMaterial);
		}
		{
			//smallRock_slime
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"smallRock_slime", L"..\\Resources\\Texture\\Dungeon\\Object\\smallRock_slime.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
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
			spriteMaterial->SetShader(shader);
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
			spriteMaterial->SetShader(shader);
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
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIBaseMaterial01", spriteMaterial);
		}
		{
			//UI Base02 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIBase02", L"..\\Resources\\Texture\\UI\\Base\\UI_base02.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIBaseMaterial02", spriteMaterial);
		}
		{
			//UI Gold 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIGold01", L"..\\Resources\\Texture\\UI\\Base\\UI_gold.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIGoldMaterial01", spriteMaterial);
		}
		{
			//UI weapon1 space 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIWeapon01", L"..\\Resources\\Texture\\UI\\Base\\UI_weapon_1.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIWeaponMaterial01", spriteMaterial);
		}
		{
			//UI weapon2 space 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIWeapon02", L"..\\Resources\\Texture\\UI\\Base\\UI_weapon_2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIWeaponMaterial02", spriteMaterial);
		}
		{
			//UI base weapon sword and shield 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIWeapon02", L"..\\Resources\\Texture\\UI\\Base\\Sword.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UISword01", spriteMaterial);
		}
		{
			//inventory base 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"UIInventoryBase01", L"..\\Resources\\Texture\\UI\\Inventory\\Inventory_base.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"UIInventoryBaseMaterial01", spriteMaterial);
		}
		{
			//MonsterHP UI1 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Monster_HP_BackBar", L"..\\Resources\\Texture\\Dungeon\\HPBar\\backBar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"MonsterHPBackBar", spriteMaterial);
		}
		{
			//MonsterHP UI2 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Monster_HP_MiddleBar", L"..\\Resources\\Texture\\Dungeon\\HPBar\\middleBar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"MonsterHPMiddleBar", spriteMaterial);
		}
		{
			//MonsterHP UI3 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Monster_HP_FrontBar", L"..\\Resources\\Texture\\Dungeon\\HPBar\\frontBar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"MonsterHPFrontBar", spriteMaterial);
		}
		{
			//BossMonsterHP Basket 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BOSS_Monster_HP_Bracket", L"..\\Resources\\Texture\\Dungeon\\BossHPBar\\Boss_bar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"BossMonsterHPBracket", spriteMaterial);
		}
		{
			//BossMonster Wave 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BOSS_Monster_Wave", L"..\\Resources\\Texture\\Dungeon\\Enemy\\GolemMiniBoss\\Smash_Wave.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"BossMonsterWave", spriteMaterial);
		}
#pragma endregion
#pragma region Player
		{
			//PlayerHPBar Red 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Player_HP_Bar_Red", L"..\\Resources\\Texture\\Player\\Player_HPBar_Red.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PlayerHPBarRed", spriteMaterial);
		}
		{
			//PlayerHPBar White 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Player_HP_Bar_White", L"..\\Resources\\Texture\\Player\\Player_HPBar_White.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PlayerHPBarWhite", spriteMaterial);
		}
		{
			//PlayerHPBar White 생성
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"Player_Hit_Screen_Red", L"..\\Resources\\Texture\\Player\\player_hit_screen.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(shader);
			spriteMaterial->SetTexture(texture);
			Resources::Insert(L"PlayerHitScreen", spriteMaterial);
		}
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

		shader = sn::Resources::Find<Shader>(L"ParticleShader");
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
		LoadTexture();
		LoadMaterial();
	}

	void PushDebugMeshInfo(DebugMesh mesh)
	{
		debugMeshs.push_back(mesh);
	}

	void BindLights()
	{
		std::vector<LightAttribute> lightsAttributes = {};
		for (Light* light : lights)
		{
			LightAttribute attribute = light->GetAttribute();
			lightsAttributes.push_back(attribute);
		}

		lightsBuffer->SetData(lightsAttributes.data(), lightsAttributes.size());
		lightsBuffer->BindSRV(eShaderStage::VS, 13);
		lightsBuffer->BindSRV(eShaderStage::PS, 13);
	}

	void BindNoiseTexture()
	{
		std::shared_ptr<Texture> texture
			= Resources::Find<Texture>(L"Noise01");

		texture->BindShaderResource(eShaderStage::VS, 15);
		texture->BindShaderResource(eShaderStage::HS, 15);
		texture->BindShaderResource(eShaderStage::DS, 15);
		texture->BindShaderResource(eShaderStage::GS, 15);
		texture->BindShaderResource(eShaderStage::PS, 15);
		texture->BindShaderResource(eShaderStage::CS, 15);

		ConstantBuffer* cb = constantBuffer[(UINT)eCBType::Noise];
		NoiseCB data = {};
		data.size.x = texture->GetWidth();
		data.size.y = texture->GetHeight();

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void Render()
	{
		BindNoiseTexture();
		BindLights();

		for (Camera* cam : cameras)
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras.clear();
		lights.clear();
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

		delete lightsBuffer;
		lightsBuffer = nullptr;
	}
}