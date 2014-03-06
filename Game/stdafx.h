//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "XINPUT9_1_0.LIB")
#pragma comment(lib, "dxguid.lib")

//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES
#define INITGUID

#include "targetver.h"
#include <windows.h>
#include <d3d.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Xinput.h>

//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <algorithm>
#include <set>
#include <memory>

#pragma warning(disable:4201 4100 4189 4127)

//////////////////////////////////////////////////////////////////////

#include "RapidXML/rapidxml.hpp"

//////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable:4127 4100)
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#pragma warning(pop)

//////////////////////////////////////////////////////////////////////

#include "Aligned.h"
#include "aligned_allocator.h"
#include "Types.h"
#include "Constants.h"
#include "Util.h"
#include "LinkedList.h"
#include "List.h"
#include "Refcount.h"
#include "File.h"
#include "UTF8.h"
#include "ParameterSet.h"
#include "Vec2.h"
#include "Vector.h"
#include "Point.h"
#include "Size.h"
#include "Matrix.h"
#include "Timer.h"
#include "Color.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Graphics.h"
#include "Joypad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "App.h"
#include "Texture.h"
#include "VertexElement.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ModelNode.h"
#include "Model.h"
#include "ImmediateContext.h"
#include "SpriteList.h"
#include "Font.h"
#include "Console.h"
#include "Physics.h"

//////////////////////////////////////////////////////////////////////

#include "Vehicle.h"

#include "Debug.h"
#include "PhysicsDebug.h"
#include "Axes.h"
#include "Bezier.h"
#include "Track.h"

#include "Box2D/Box2D.h"
