#pragma once
#include <memory>
#include <iostream>
#include "KGEngineManager.h"
#include "KGScheduler.h"
#include "KGShaderInterface.h"
#include "KGTimer.h"
#include "KGInput.h"
#include "KGScene.h"
#include "KGShaderObject.h"
#include "KGLabel.h"
#include "KGSprite.h"
#include "KGSize.h"
#include "KGPoint.h"

namespace KG2D
{
	extern shared_ptr<KGEngineManager> EngineManager;
	extern shared_ptr<KGTimer>	Timer;
	extern shared_ptr<KGShaderInterface> Shader;
	extern shared_ptr<KGInput> Input;
}