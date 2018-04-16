#include "Pellet.hpp"

#include "EVA/ResourceManagers.hpp"
#include "EVA/Input.hpp"

#include "Game.hpp"

std::shared_ptr<EVA::Mesh> Pellet::m_Mesh;
std::shared_ptr<EVA::Material> Pellet::m_Material;

Pellet::Pellet(EVA::GameObject* gameObject, Game* game, const glm::ivec2 tile) 
	: Component(gameObject), m_Tile(tile), m_Game(game)
{
	auto mr = gameObject->AddComponent<EVA::MeshRenderer>();
	mr->Set(m_Mesh, m_Material);

	transform->SetScale(0.2f);
	transform->SetPosition(TileMap::GetTilePosition(m_Tile) + EVA::YAXIS * transform->scale.y);
	
}

void Pellet::Update(const float deltaTime)
{
	if (m_Game->pacman->currentTile == m_Tile)
	{
		OnPickup();
		gameObject->Destroy();
		m_Mesh->isDirty = true;
	}
}

void Pellet::OnPickup()
{
	m_Game->AddScore(m_Score);
}

void Pellet::Init()
{
	// Pellets
	m_Mesh = EVA::ModelManager::Primitive(EVA::PrimitiveType::Sphere)->GetMesh(0);
	m_Mesh->isStatic = true;

	m_Material = std::make_shared<EVA::Material>();
	m_Material->enableInstancing = true;
	m_Material->tintDiffuse = { 1.0f, 1.0f, 0.0f, 1.0f };
	m_Material->shader = EVA::ShaderManager::GetShader("standard_instanced");
}

void Energizer::OnPickup()
{
	Pellet::OnPickup();
	m_Game->ActivateEnergizer(time);
}
