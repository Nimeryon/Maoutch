#include <SFML/Graphics.hpp>
#include <filesystem>

#include "imgui.h"
#include "../InputHandler.h"
#include "ParticleEditorState.h"
#include "../Graphics/Particle.h"
#include "../../Tools/Colors.h"
#include "../../Tools/String.h"
#include "../AssetLoader.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

namespace maoutch
{
	std::vector<std::string> filePaths = {};
	char fileName[255] = {};
	static const char* emitterTypes[]{"Fixed", "Random", "Lerp", "RandomLerp"};

	ParticleEditorState::ParticleEditorState() :
		_startColor{ 1, 1, 0, 1 },
		_endColor{1, 0, 0, 1},
		_textureRectSize{16, 16},
		_xDirection{-1, 1},
		_yDirection{-1, 1}
	{
		for (const auto& filePath : std::filesystem::directory_iterator(particlesPath))
			filePaths.push_back(filePath.path().string());
	}
	ParticleEditorState::~ParticleEditorState()
	{
		delete _particle;
	}

	void ParticleEditorState::Init()
	{
		_particle = new ParticleEmitter();
		_particle->SetTextureRectSize(Vector2i(_textureRectSize[0], _textureRectSize[1]));
	}
	void ParticleEditorState::ProcessInputs()
	{
		if (InputHandler::GetInstance()->IsKeyDown(sf::Keyboard::Space)) {
			if (_particle->IsPlaying())
				_particle->Stop();
			else _particle->Play();
		}
	}

	void ParticleEditorState::Update(float dt)
	{
		_particle->SetPosition(InputHandler::GetInstance()->GetMousePosition());
	}
	
	void ParticleEditorState::ImGui(float dt)
	{
		if (!ImGui::Begin("Particle Editor") || _particle == nullptr)
		{
			ImGui::End();
			return;
		}

		ImGui::Text("FPS:");
		ImGui::SameLine();
		ImGui::Text(&std::to_string(1.f / dt)[0]);

		// Emitter
		if (ImGui::TreeNode("Emitter Parameters"))
		{
			ImGui::Text("Current Particle Count:");
			ImGui::SameLine();
			ImGui::Text(&std::to_string(_particle->GetCurrentParticleCount())[0]);

			ImGui::Text("Current Emitter Lifetime:");
			ImGui::SameLine();
			ImGui::Text(&std::to_string(*_particle->GetCurrentLifetime())[0]);

			ImGui::Checkbox("Play On Awake", _particle->PlayOnAwake());
			ImGui::Checkbox("Is Looping", _particle->IsLooping());
			ImGui::Checkbox("Is Playing", _particle->IsPlaying());
			ImGui::Checkbox("Is Textured", _particle->IsTextured());

			if (ImGui::TreeNode("Parameters"))
			{

				if (ImGui::DragInt("Max Particle Count", _particle->GetMaxParticleCount(), 10, 0, 50000))
					_particle->ReserveParticlesSpace();
				ImGui::DragFloat("Emitter Lifetime", _particle->GetLifetime(), .1f, 0, 1000);
				ImGui::DragFloat("Emitter Emission Time", _particle->GetEmissionSpeed(), .00001f, .00001f, 10, "%.5f");

				ImGui::Separator();
				ImGui::TreePop();
			}

			if (_particle->IsTextured() && ImGui::TreeNode("Texture"))
			{
				if (ImGui::BeginCombo("Textures", &_particle->GetTextureName()[0]))
				{
					for (const auto& textureValue : AssetLoader::GetInstance()->GetTexturesMap())
						if (ImGui::Selectable(&textureValue.first[0]))
							_particle->SetTextureName(textureValue.first);
					ImGui::EndCombo();
				}

				if (ImGui::DragInt2("Texture Rect Size", _textureRectSize, 1, 1))
					_particle->SetTextureRectSize(Vector2i(_textureRectSize[0], _textureRectSize[1]));

				ImGui::Text("Texture Positions");
				if (ImGui::Button("Add Texture Position")) { AddTexturePosition(); }
				ImGui::SameLine();
				if (ImGui::Button("Remove Texture Position")) { RemoveTexturePosition(); }
				for (int i = 0; i < _terturePositions.size(); ++i)
				{
					if (ImGui::SliderInt2(&("Position " + std::to_string(i))[0], _terturePositions[i], 0, 64))
						_particle->GetTexturePositions()[i] = Vector2i(_terturePositions[i][0], _terturePositions[i][1]);
				}

				ImGui::Separator();
				ImGui::TreePop();
			}

			ImGui::Separator();
			ImGui::TreePop();
		}

		// Particle
		if (ImGui::TreeNode("Particles Parameters"))
		{
			if (ImGui::TreeNode("LifeTime"))
			{
				ImGui::DragFloat("Min lifetime", _particle->GetMinParticleLifetime(), .1f, 0, 1000);
				ImGui::DragFloat("Max lifetime", _particle->GetMaxParticleLifetime(), .1f, 0, 1000);

				ImGui::Separator();
				ImGui::TreePop();
			}

			EmitterDirectionValueEditor();
			EmitterColorValueEditor();
			EmitterValueEditor("Size", "", *_particle->GetParticleSize());
			EmitterValueEditor("Speed", "", *_particle->GetParticleSpeed());
			EmitterValueEditor("Friction", "", *_particle->GetParticleFriction());
			EmitterValueEditor("Gravity", "", *_particle->GetParticleGravity());

			if (ImGui::TreeNode("Start Rotation"))
			{
				ImGui::DragFloat("Min Start Rotation", _particle->GetMinParticleStartRotation(), 1, -360, 360);
				ImGui::DragFloat("Max Start Rotation", _particle->GetMaxParticleStartRotation(), 1, -360, 360);

				ImGui::Separator();
				ImGui::TreePop();
			}

			EmitterValueEditor("Rotation Speed", "Warming: Impact FrameRate | (for disabling: Fixed 0)", *_particle->GetParticleRotationSpeed());

			ImGui::Separator();
			ImGui::TreePop();
		}

		SaveLoadEditor();

		if (ImGui::Button("Reset")) { _particle->Reset(); }
		ImGui::SameLine();
		if (ImGui::Button("Play")) { _particle->Play(); }
		ImGui::SameLine();
		if (ImGui::Button("Stop")) { _particle->Stop(); }

		ImGui::End();
	}

	void ParticleEditorState::AddTexturePosition()
	{
		_terturePositions.emplace_back(new int[2]{0, 0});
		_particle->AddTexturePosition({ 0, 0 });
	}
	void ParticleEditorState::RemoveTexturePosition()
	{
		_terturePositions.pop_back();
		_particle->RemoveTexturePosition();
	}

	template <typename T>
	void ParticleEditorState::EmitterValueEditor(const std::string& editorName, const std::string& warningText, EmitterValue<T>& emitter)
	{
		if (ImGui::TreeNode(&editorName[0]))
		{
			if (!warningText.empty()) ImGui::TextColored(ImVec4(1, 1, 0, 1), &warningText[0]);
			if (emitter.type == 0)
			{
				ImGui::DragFloat(&("Fixed " + editorName)[0], &emitter.minXValue);
			}
			else if (emitter.type == 3)
			{
				ImGui::DragFloat(&("Min Start " + editorName)[0], &emitter.minXValue);
				ImGui::DragFloat(&("Max Start " + editorName)[0], &emitter.maxXValue);
				ImGui::DragFloat(&("Min End " + editorName)[0], &emitter.minYValue);
				ImGui::DragFloat(&("Max End " + editorName)[0], &emitter.maxYValue);
			}
			else
			{
				ImGui::DragFloat(&("Start " + editorName)[0], &emitter.minXValue);
				ImGui::DragFloat(&("End " + editorName)[0], &emitter.maxXValue);
			}
			ImGui::Combo(&(editorName + " Type")[0], &emitter.type, emitterTypes, IM_ARRAYSIZE(emitterTypes));

			ImGui::Separator();
			ImGui::TreePop();
		}
	}
	void ParticleEditorState::EmitterColorValueEditor()
	{
		if (ImGui::TreeNode("Color"))
		{
			if (_particle->GetParticleColors()->type == 0)
			{
				if (ImGui::ColorEdit4("Fixed Color", _startColor))
					_particle->GetParticleColors()->startColor = colors::FromFloat4(_startColor);
			}
			else if (_particle->GetParticleColors()->type == 2)
			{
				if (ImGui::ColorEdit4("Start Color", _startColor))
					_particle->GetParticleColors()->startColor = colors::FromFloat4(_startColor);
				if (ImGui::ColorEdit4("End Color", _endColor))
					_particle->GetParticleColors()->endColor = colors::FromFloat4(_endColor);
			}

			if (ImGui::Combo("Color Type", &_particle->GetParticleColors()->type, emitterTypes, IM_ARRAYSIZE(emitterTypes) - 1))
			{
				_particle->GetParticleColors()->startColor = colors::FromFloat4(_startColor);
				_particle->GetParticleColors()->endColor = colors::FromFloat4(_endColor);
			}

			ImGui::Separator();
			ImGui::TreePop();
		}
	}
	void ParticleEditorState::EmitterDirectionValueEditor()
	{
		if (ImGui::TreeNode("Direction"))
		{
			if (_particle->GetParticleDirection()->type == 0)
			{
				if (ImGui::DragFloat2("Fixed Direction", _xDirection, .001f, -1, 1))
					_particle->GetParticleDirection()->xVector = Vector2(_xDirection[0], _xDirection[1]);
			}
			else
			{
				if (ImGui::DragFloat2(_particle->GetParticleDirection()->type == 1 ? "X Direction" : "Start Direction", _xDirection, .001f, -1, 1))
					_particle->GetParticleDirection()->xVector = Vector2(_xDirection[0], _xDirection[1]);
				if (ImGui::DragFloat2(_particle->GetParticleDirection()->type == 1 ? "Y Direction" : "End Direction", _yDirection, .001f, -1, 1))
					_particle->GetParticleDirection()->yVector = Vector2(_yDirection[0], _yDirection[1]);
			}
			ImGui::Combo("Direction Type", &_particle->GetParticleDirection()->type, emitterTypes, IM_ARRAYSIZE(emitterTypes) - 1);

			ImGui::Separator();
			ImGui::TreePop();
		}
	}
	void ParticleEditorState::SaveLoadEditor()
	{
		if (ImGui::TreeNode("Save / Load"))
		{
			if (ImGui::BeginCombo("Particle Systems", "Default"))
			{
				for (std::string filePath : filePaths)
					if (ImGui::Selectable(filePath.c_str()))
					{
						_particle->SetupFromFile(filePath);
						colors::ToFloat4(_startColor, _particle->GetParticleColors()->startColor);
						colors::ToFloat4(_endColor, _particle->GetParticleColors()->endColor);

						// Clear file input name
						for (int i = 0; i < 255; ++i)
							fileName[i] = (char)0;

						// Update file list
						std::string newName = string::Replace(filePath, particlesPath, "");
						for (int i = 0; i < newName.size(); ++i)
							fileName[i] = newName[i];

						// Update rectSize
						_textureRectSize[0] = _particle->GetTextureRectSize().x;
						_textureRectSize[1] = _particle->GetTextureRectSize().y;

						// Update directions
						_xDirection[0] = _particle->GetParticleDirection()->xVector.x;
						_xDirection[1] = _particle->GetParticleDirection()->xVector.y;
						_yDirection[0] = _particle->GetParticleDirection()->yVector.x;
						_yDirection[1] = _particle->GetParticleDirection()->yVector.y;

						// Update positions
						_terturePositions.clear();
						for (int i = 0; i < _particle->GetTexturePositions().size(); ++i)
							_terturePositions.emplace_back(new int[2]{ _particle->GetTexturePositions()[i].x, _particle->GetTexturePositions()[i].y });

						_particle->Reset();
					}
				ImGui::EndCombo();
			}

			ImGui::InputText("Particle Name", fileName, 255);
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				std::string name = std::string(fileName);
				if (name != "")
				{
					_particle->SaveToFile(name);

					filePaths.clear();
					for (const auto& filePath : std::filesystem::directory_iterator(particlesPath))
						filePaths.push_back(filePath.path().string());
				}
			}

			ImGui::Separator();
			ImGui::TreePop();
		}
	}
}
