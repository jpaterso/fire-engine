/**
 * FILE:    AnimatedMeshMD3Loader.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD3Loader.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A Loader for Quake III .MD3 files.
**/

#ifndef ANIMATEDMESHMD3LOADER_H_INCLUDED
#define ANIMATEDMESHMD3LOADER_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "ILoader.h"

namespace fire_engine
{

class AnimatedMeshMD3;
class string;

/** A class for loading Quake III .MD3 files. */
class _FIRE_ENGINE_API_ AnimatedMeshMD3Loader : public ILoader<AnimatedMeshMD3>
{
private:
	/** A header for an .MD3 file. */
	typedef struct
	{
		s32 magic;
		s32 version;
		c8  name[68];
		s32 num_bone_frames;
		s32 num_tags;
		s32 num_meshes;
		s32 num_max_skins;
		s32 header_size;
		s32 offset_tags;
		s32 offset_meshes;
		s32 offset_eof;
	} md3_header_t;

	/** Simple 3 component vector, used in .MD3 files. */
	typedef f32 md3_vec3_t[3];

	/** The information related to a bone frame in an .MD3 file. */
	typedef struct
	{
		md3_vec3_t min_bounds;
		md3_vec3_t max_bounds;
		md3_vec3_t local_origin;
		f32        bounding_sphere_radius;
		c8         name[16];
	} md3_bone_frame_t;

	/** A tag in an .MD3 file. */
	typedef struct
	{
		c8         name[64];
		md3_vec3_t origin;
		f32        orientation[9];
	} md3_tag_t;

	/** The information that precedes a mesh in an .MD3 file. */
	typedef struct
	{
		s32 magic;
		c8  name[68];
		s32 num_frames;
		s32 num_shaders;
		s32 num_verts;
		s32 num_triangles;
		s32 offset_triangles;
		s32 offset_shaders;
		s32 offset_tex_coords;
		s32 offset_vertices;
		s32 offset_end;
	} md3_mesh_header_t;

	/** The information about a shader in an .MD3 file. */
	typedef struct
	{
		c8  name[68];
		s32 shader_index;
	} md3_shader_t;

	/** A triangle, containing three vertex indices. */
	typedef s32 md3_triangle_t[3];

	/** The (u, v) texture coordinates. */
	typedef f32 md3_tex_coords_t[2];

	/** A vertex, stored in spherical coordinates. */
	typedef struct
	{
		s16 x, y, z;
		s16 normal;
	} md3_vertex_t;

public:
	/** Constructor. */
	AnimatedMeshMD3Loader();

	/** Destructor. */
	virtual ~AnimatedMeshMD3Loader();

	/** Load and create an AnimatedMeshMD3 from an .MD3 file */
	virtual AnimatedMeshMD3 * load(const string& filename) const;
private:
};

} // namespace fire_engine

#endif // ANIMATEDMESHMD3LOADER_H_INCLUDED
