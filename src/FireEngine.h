/**
 *  $Id: FireEngine.h 119 2007-12-03 02:12:08Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  Main include file for the Fire Engine
**/

#ifndef	__FIRE_ENGINE_H
#define	__FIRE_ENGINE_H

#include "Types.h"
#include "CompileConfig.h"

#include "aabbox.h"
#include "AnimatedMeshMD2.h"
#include "AnimatedMeshMD3.h"
#include "AnimatedMeshMD2Loader.h"
#include "AnimatedMeshMD3Loader.h"
#include "AnimatedModel.h"
#include "AnimatedModelMD3.h"
#include "Array.h"
#include "Bezier.h"
#include "ByteConverter.h"
#include "Camera.h"
#include "CameraFPS.h"
#include "CMeshBuffer.h"
#include "color.h"
#include "ColorConverter.h"
#include "Counter.h"
#include "Device.h"
#include "dimension2.h"
#include "DirectoryFileProvider.h"
#include "File.h"
#include "FileSystem.h"
#include "FileUtils.h"
#include "HashTable.h"
#include "HighResolutionTimer.h"
#include "IAnimatedMesh.h"
#include "IEventReceiver.h"
#include "IFile.h"
#include "IFileProvider.h"
#include "Image.h"
#include "ImageLoaderBMP.h"
#include "ImageLoaderPCX.h"
#include "ImageLoaderTGA.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "INode.h"
#include "ISpaceNode.h"
#include "Object.h"
#include "IModel.h"
#include "InputEvent.h"
#include "IRenderer.h"
#include "IResizable.h"
#include "Item.h"
#include "ITexture.h"
#include "IWindowManager.h"
#include "KeyEvent.h"
#include "Light.h"
#include "LightSpaceNode.h"
#include "line3.h"
#include "List.h"
#include "Logger.h"
#include "Material.h"
#include "Math.h"
#include "matrix4.h"
#include "MediaManager.h"
#include "MemoryFile.h"
#include "MeshModifier.h"
#include "MouseEvent.h"
#include "Octree.h"
#include "plane3.h"
#include "quaternion.h"
#include "Q3Map.h"
#include "Q3MapLoader.h"
#include "SceneManager.h"
#include "ISpaceNode.h"
#include "SkyBox.h"
#include "Stack.h"
#include "String.h"
#include "Timer.h"
#include "triangle3.h"
#include "vector2.h"
#include "vector3.h"
#include "Vertex3.h"
#include "ViewFrustum.h"
#include "ZipFileReader.h"

// Run-time specific headers
#if defined(_FIRE_ENGINE_WIN32_)
#	include <windows.h>
#	include "WindowManagerWin32.h"
#endif

#if defined(_FIRE_ENGINE_COMPILE_WITH_OPENGL_)
#	include <GL/gl.h>
#	include <GL/glext.h>
#	include <GL/glu.h>
#	include "OpenGLRenderer.h"
#	include "OpenGLTexture.h"
#endif

#endif
