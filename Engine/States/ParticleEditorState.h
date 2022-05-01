#pragma once
#include <vector>

#include "State.h"
#include "../Graphics/ParticleEmitter.h"

namespace maoutch
{
	template <typename T>
	struct EmitterValue;

	class ParticleEditorState : public State
	{
	public:
		ParticleEditorState();
		~ParticleEditorState() override;

		void Init() override;
		void ProcessInputs() override;

		void Update(float dt) override;
		void ImGui(float dt) override;

		void AddTexturePosition();
		void RemoveTexturePosition();

		template <typename T>
		void EmitterValueEditor(const std::string& editorName, const std::string& warningText, EmitterValue<T>& emitter);
		void EmitterColorValueEditor();
		void EmitterDirectionValueEditor();
		void SaveLoadEditor();

	private:
		float _startColor[4];
		float _endColor[4];
		int _textureRectSize[2];
		int _colorEaseType;
		int _directionEaseType;
		float _xDirection[2];
		float _yDirection[2];
		bool _destroyAfterPlaying;
		std::vector<int*> _terturePositions = {};
		ParticleEmitter* _particle = nullptr;
	};
}