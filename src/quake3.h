/**
 * RCS ID:  $Id$
 * All structures associated with Quake III bsp files.
**/

#ifndef QUAKE3_H_INCLUDED
#define QUAKE3_H_INCLUDED

namespace fire_engine
{
namespace q3
{
/** A Quake III BSP file contains 17 "lumps" of information. */
enum EBSP_LUMPS
{
	EBL_ENTITIES = 0x00,
	EBL_TEXTURES,
	EBL_PLANES,
	EBL_NODES,
	EBL_LEAFS,
	EBL_LEAF_FACES,
	EBL_LEAF_BRUSHES,
	EBL_MODELS,
	EBL_BRUSHES,
	EBL_BRUSH_SIDES,
	EBL_VERTICES,
	EBL_MESH_VERTICES,
	EBL_EFFECTS,
	EBL_FACES,
	EBL_LIGHTMAPS,
	EBL_LIGHT_VOLUMES,
	EBL_VISIBILITY_DATA,
	EBL_LUMP_COUNT
};

/** Each lump contains it's offset from the start of the file and it's size
 in bytes. */
typedef struct
{
	s32 offset;
	s32 size;
} bsp_lump_t;

typedef struct
{
	s32 id;
	s32 version;
} bsp_header_t;

typedef struct
{
	c8 * descriptions;
} bsp_entities_t;

typedef struct
{
	c8 name[64];
	s32 flags;
	s32 contents;
} bsp_texture_t;

typedef struct
{
	f32 normal[3];
	f32 dist;
} bsp_plane_t;

typedef struct
{
	s32 plane_index;
	s32 child_indices[2];
	s32 bb_mins[3];
	s32 bb_maxs[3];
} bsp_node_t;

typedef struct
{
	s32 cluster;
	s32 area;
	s32 bb_mins[3];
	s32 bb_maxs[3];
	s32 leaf_face;
	s32 num_leafs;
	s32 leafbrush;
	s32 num_leafbrushes;
} bsp_leaf_t;

typedef struct
{
	s32 face_index;
} bsp_leaf_face_t;

typedef struct
{
	s32 brush_index;
} bsp_leaf_brush_t;

typedef struct
{
	f32 bb_mins[3];
	f32 bb_maxs[3];
	s32 face_index;
	s32 num_faces;
	s32 brush_index;
	s32 num_brushes;
} bsp_model_t;

typedef struct
{
	s32 brushside_index;
	s32 num_brushsides;
	s32 texture_index;
} bsp_brush_t;

typedef struct
{
	s32 plane_index;
	s32 texture_index;
} bsp_brush_side_t;

/** Quake III vertex definition. Note the added lightmap coordinates. */
typedef struct
{
	f32 position[3];
	f32 tex_coords[2];
	f32 lightmap_coords[2];
	f32 normal[3];
	u8 color[4];
} bsp_vertex_t;

typedef struct
{
	s32 vertex_offset;
} bsp_mesh_vertex_t;

typedef struct
{
	c8 name[64];
	s32 brush_index;
	s32 unknown;
} bsp_effect_t;

typedef struct
{
	s32 tex_id;
	s32 effect;
	s32 type;
	s32 vert_index;
	s32 vert_count;
	s32 mesh_vert_index;
	s32 mesh_vert_count;
	s32 lightmap_id;
	s32 lightmap_corner[2];
	s32 lightmap_size[2];
	f32 lightmap_pos[3];
	f32 lightmap_bits[2][3];
	f32 normal[3];
	s32 size[2];
} bsp_face_t;

/** Lightmap 128x128 RGB data. */
typedef struct
{
	u8 map[128][128][3];
} bsp_lightmap_t;

typedef struct
{
	u8 ambient[3];
	u8 directional[3];
	u8 direction[2];
} bsp_light_volume_t;

/** Cluster visibility data. */
typedef struct
{
	s32 num_vectors;
	s32 size_vector;
	u8 * vectors;
} bsp_visibility_data_t;
}
}

#endif //QUAKE3_H_INCLUDED
