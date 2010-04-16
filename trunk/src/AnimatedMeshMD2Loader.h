/**
 * FILE:    AnimatedMeshMD2Loader.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD2Loader.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A class for loading .MD2 files, and creating AnimatedMeshMD2 objects
**/

#ifndef ANIMATEDMESHMD2LOADER_H_INCLUDED
#define ANIMATEDMESHMD2LOADER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ILoader.h"
#include "AnimatedMeshMD2.h"
#include "vector3.h"

#define __FIRE_ENGINE_MAX_MD2_NORMALS 0xA2 // 162

namespace fire_engine
{

class String;

namespace io
{
class IFile;
class IFileProvider;
}

class  _FIRE_ENGINE_API_ AnimatedMeshMD2Loader : public ILoader<AnimatedMeshMD2>
{
private:
	typedef struct
	{
		s32 magic;
		s32 version;
		s32 skin_width;
		s32 skin_height;
		s32 framesize;
		s32 num_skins;
		s32 num_vertices;
		s32 num_tex_coords;
		s32 num_triangles;
		s32 num_gl_commands;
		s32 num_frames;
		s32 offset_skins;
		s32 offset_tex_coords;
		s32 offset_triangles;
		s32 offset_frames;
		s32 offset_gl_commands;
		s32 offset_end_file;
	} MD2Header;

	typedef f32 md2_vec3_t[3];

	typedef struct
	{
		u8 v[3];
		u8 normal_index;
	} md2_vertex_t;

	typedef struct
	{
		u16 vertex[3];
		u16 tex_coords[3];
	} md2_triangle_t;

	typedef struct
	{
		u16 s, t;
	} md2_tex_coords_t;

	typedef struct
	{
		md2_vec3_t scale;
		md2_vec3_t translate;
		c8 name[16];
		md2_vertex_t * vertices;
	} md2_frame_t;

	typedef struct
	{
		f32 s;
		f32 t;
		s32 index;
	} md2_gl_command_t;

	typedef c8 md2_skin_t[68];

public:
	AnimatedMeshMD2Loader();

	virtual ~AnimatedMeshMD2Loader();

	//! Implementation for ILoader
	virtual AnimatedMeshMD2 * load(const String& filename, io::IFileProvider * fileProvider) const;

private:
	static vector3f m_normal_list[__FIRE_ENGINE_MAX_MD2_NORMALS];

	AnimatedMeshMD2 * fromMD2Structures(MD2Header md2h, md2_skin_t * skins,
		md2_triangle_t * triangles, md2_tex_coords_t * tex_coords, md2_frame_t * frames) const;
};

}

#endif // ANIMATEDMESHMD2LOADER_H_INCLUDED
