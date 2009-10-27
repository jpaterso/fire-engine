/**
 * FILE:    Q3MapLoader.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: Q3MapLoader.cpp 119 2007-12-03 02:12:08Z jpaterso $
**/

#include "Q3MapLoader.h"
#include "ByteConverter.h"
#include "Device.h"
#include "FileSystem.h"
#include "FileUtils.h"
#include "IFile.h"
#include "IRenderer.h"
#include "ITexture.h"
#include "Logger.h"
#include "plane3.h"
#include "Q3Map.h"
#include "string.h"

#define Q3_MAGIC_ID      0x50534249 // "IBSP" in little endian
#define Q3_MAGIC_VERSION 0x2E       // 46

namespace fire_engine
{

Q3MapLoader::~Q3MapLoader()
{
}

Q3Map * Q3MapLoader::load(const string& filename) const
{
	q3::bsp_header_t header;
	q3::bsp_lump_t lumps[q3::EBL_LUMP_COUNT];
	q3::bsp_entities_t entities;
	q3::bsp_texture_t * q3textures = nullptr;
	q3::bsp_plane_t * q3planes = nullptr;
	q3::bsp_node_t * nodes = nullptr;
	q3::bsp_leaf_t * leafs = nullptr;
	q3::bsp_leaf_face_t * leaf_faces = nullptr;
	q3::bsp_leaf_brush_t * leaf_brushes = nullptr;
	q3::bsp_model_t * models = nullptr;
	q3::bsp_brush_t * brushes = nullptr;
	q3::bsp_brush_side_t * brush_sides = nullptr;
	q3::bsp_vertex_t * vertices = nullptr;
	q3::bsp_mesh_vertex_t * mesh_vertices = nullptr;
	q3::bsp_effect_t * effects = nullptr;
	q3::bsp_face_t * faces = nullptr;
	q3::bsp_lightmap_t * lightmaps = nullptr;
	q3::bsp_light_volume_t * light_volumes = nullptr;
	q3::bsp_visibility_data_t visibility_data;

	io::IFile * file = io::FileSystem::Get()->openReadFile(filename, false, false,
		io::EFOF_READ|io::EFOF_BINARY);

	if (file == nullptr)
	{
		Logger::Get()->log(ES_HIGH, "Q3MapLoader", "Could not open %s for reading", filename.c_str());
		return nullptr;
	}

	/* read in header */
	file->read(&header, sizeof(q3::bsp_header_t));
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
	header.id = ByteConverter::ByteSwap(header.id);
	header.version = ByteConverter::ByteSwap(header.version);
#endif
	if (header.id != Q3_MAGIC_ID || header.version != Q3_MAGIC_VERSION)
	{
		Logger::Get()->log(ES_HIGH, "Q3MapLoader", "Invalid header in file %s", filename.c_str());
		delete file;
		return nullptr;
	}

	/* read in lump information */
	file->read(lumps, q3::EBL_LUMP_COUNT*sizeof(q3::bsp_lump_t));
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
	for (s32 i = 0; i < q3::EBL_LUMP_COUNT; i++)
	{
		lumps[i].offset = ByteConverter::ByteSwap(lumps[i].offset);
		lumps[i].size = ByteConverter::ByteSwap(lumps[i].size);
	}
#endif

    entities.descriptions = new c8[lumps[q3::EBL_ENTITIES].size];
    file->seek(io::EFSP_START, lumps[q3::EBL_ENTITIES].offset);
    file->read(entities.descriptions, lumps[q3::EBL_ENTITIES].size);
    //TODO Do we need to add '\0' at the end

	/* read in texture information */
	s32 num_textures = lumps[q3::EBL_TEXTURES].size/sizeof(q3::bsp_texture_t);
	q3textures = new q3::bsp_texture_t[num_textures];
	file->seek(io::EFSP_START, lumps[q3::EBL_TEXTURES].offset);
	file->read(q3textures, lumps[q3::EBL_TEXTURES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
	for (s32 i = 0; i < num_textures; i++)
	{
		q3textures[i].flags = ByteConverter::ByteSwap(q3textures[i].flags);
		q3textures[i].contents = ByteConverter::ByteSwap(q3textures[i].contents);
	}
#endif

	// Create textures
	ITexture ** textures = loadTextures(q3textures, num_textures);

	/* read in plane information */
	//TODO do something with plane information
	s32 num_planes = lumps[q3::EBL_PLANES].size/sizeof(q3::bsp_plane_t);
	q3planes = new q3::bsp_plane_t[num_planes];
	file->seek(io::EFSP_START, lumps[q3::EBL_PLANES].offset);
	file->read(q3planes, lumps[q3::EBL_PLANES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_planes; i++)
    {
        for (s32 j = 0; j < 3; j++)
        {
            q3planes[i].normal[j] = ByteConverter::ByteSwap(q3planes[i].normal[j]);
        }
        q3planes[i].dist = ByteConverter::ByteSwap(q3planes[i].dist);
    }
#endif

	// Create planes
	plane3f * planes = new plane3f[num_planes];
	for (s32 i = 0; i < num_planes; i++)
	{
	    vector3f normal(q3planes[i].normal);
	    swizzle(normal);
		planes[i] = plane3f(q3planes[i].dist, normal);
	}


    /* read in node information */
    //TODO do something with node information
    s32 num_nodes = lumps[q3::EBL_NODES].size/sizeof(q3::bsp_node_t);
    nodes = new q3::bsp_node_t[num_nodes];
    file->seek(io::EFSP_START, lumps[q3::EBL_NODES].offset);
    file->read(nodes, lumps[q3::EBL_NODES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_nodes; i++)
    {
        nodes[i].plane_index = ByteConverter::ByteSwap(nodes[i].plane_index);
        nodes[i].child_indices[0] = ByteConverter::ByteSwap(nodes[i].child_indices[0]);
        nodes[i].child_indices[1] = ByteConverter::ByteSwap(nodes[i].child_indices[1]);
        for (s32 j = 0; j < 3; j++)
        {
            nodes[i].bb_mins[j] = ByteConverter::ByteSwap(nodes[i].bb_mins[j]);
            nodes[i].bb_maxs[j] = ByteConverter::ByteSwap(nodes[i].bb_maxs[j]);
        }
    }
#endif

	/* read in leaf information */
	//TODO do something with leaf information
    s32 num_leafs = lumps[q3::EBL_LEAFS].size/sizeof(q3::bsp_leaf_t);
    leafs = new q3::bsp_leaf_t[num_leafs];
    file->seek(io::EFSP_START, lumps[q3::EBL_LEAFS].offset);
    file->read(leafs, lumps[q3::EBL_LEAFS].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_leafs; i++)
    {
        leafs[i].cluster = ByteConverter::ByteSwap(leafs[i].cluster);
        leafs[i].area = ByteConverter::ByteSwap(leafs[i].area);
        for (s32 j = 0; j < 3; j++)
        {
            leafs[i].bb_mins[j] = ByteConverter::ByteSwap(leafs[i].bb_mins[j]);
            leafs[i].bb_maxs[j] = ByteConverter::ByteSwap(leafs[i].bb_maxs[j]);
        }
        leafs[i].leaf_face = ByteConverter::ByteSwap(leafs[i].leaf_face);
        leafs[i].num_leafs = ByteConverter::ByteSwap(leafs[i].num_leafs);
        leafs[i].leafbrush = ByteConverter::ByteSwap(leafs[i].leafbrush);
        leafs[i].num_leafbrushes = ByteConverter::ByteSwap(leafs[i].num_leafbrushes);
    }
#endif

    /* read in leaf face information */
    s32 num_leaf_faces = lumps[q3::EBL_LEAF_FACES].size/sizeof(q3::bsp_leaf_face_t);
    leaf_faces = new q3::bsp_leaf_face_t[num_leaf_faces];
    file->seek(io::EFSP_START, lumps[q3::EBL_LEAF_FACES].offset);
    file->read(leaf_faces, lumps[q3::EBL_LEAF_FACES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_leaf_faces; i++)
    {
        leaf_faces[i].face_index = ByteConverter::ByteSwap(leaf_faces[i].face_index);
    }
#endif

    /* read in leaf brush information */
    s32 num_leaf_brushes = lumps[q3::EBL_LEAF_BRUSHES].size/sizeof(q3::bsp_leaf_brush_t);
    leaf_brushes = new q3::bsp_leaf_brush_t[num_leaf_brushes];
    file->seek(io::EFSP_START, lumps[q3::EBL_LEAF_BRUSHES].offset);
    file->read(leaf_brushes, lumps[q3::EBL_LEAF_BRUSHES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_leaf_brushes; i++)
    {
        leaf_brushes[i].brush_index = ByteConverter::ByteSwap(leaf_brushes[i].brush_index);
    }
#endif

    /* read in model information */
    s32 num_models = lumps[q3::EBL_MODELS].size/sizeof(q3::bsp_model_t);
    models = new q3::bsp_model_t[num_models];
    file->seek(io::EFSP_START, lumps[q3::EBL_MODELS].offset);
    file->read(models, lumps[q3::EBL_MODELS].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_models; i++)
    {
        for (s32 j = 0; j < 3; j++)
        {
            models[i].bb_mins[j] = ByteConverter::ByteSwap(models[i].bb_mins[j]);
            models[i].bb_maxs[j] = ByteConverter::ByteSwap(models[i].bb_maxs[j]);
        }
        models[i].face_index = ByteConverter::ByteSwap(models[i].face_index);
        models[i].num_faces = ByteConverter::ByteSwap(models[i].num_faces);
        models[i].brush_index = ByteConverter::ByteSwap(models[i].brush_index);
        models[i].num_brushes = ByteConverter::ByteSwap(models[i].num_brushes);
    }
#endif

    /* read in brush information */
    s32 num_brushes = lumps[q3::EBL_BRUSHES].size/sizeof(q3::bsp_brush_t);
    brushes = new q3::bsp_brush_t[num_brushes];
    file->seek(io::EFSP_START, lumps[q3::EBL_BRUSHES].offset);
    file->read(brushes, lumps[q3::EBL_BRUSHES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_brushes; i++)
    {
        brushes[i].brushside_index = ByteConverter::ByteSwap(brushes[i].brushside_index);
        brushes[i].num_brushsides = ByteConverter::ByteSwap(brushes[i].num_brushsides);
        brushes[i].texture_index = ByteConverter::ByteSwap(brushes[i].texture_index);
    }
#endif

    s32 num_brush_sides = lumps[q3::EBL_BRUSH_SIDES].size/sizeof(q3::bsp_brush_side_t);
    brush_sides = new q3::bsp_brush_side_t[num_brush_sides];
    file->seek(io::EFSP_START, lumps[q3::EBL_BRUSH_SIDES].offset);
    file->read(brush_sides, lumps[q3::EBL_BRUSH_SIDES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_brush_sides; i++)
    {
        brush_sides[i].plane_index = ByteConverter::ByteSwap(brush_sides[i].plane_index);
        brush_sides[i].texture_index = ByteConverter::ByteSwap(brush_sides[i].texture_index);
    }
#endif

    /* read in vertex information */
    s32 num_vertices = lumps[q3::EBL_VERTICES].size/sizeof(q3::bsp_vertex_t);
    vertices = new q3::bsp_vertex_t[num_vertices];
    file->seek(io::EFSP_START, lumps[q3::EBL_VERTICES].offset);
    file->read(vertices, lumps[q3::EBL_VERTICES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_vertices; i++)
    {
        for (s32 j = 0; j < 3; j++)
        {
            vertices[i].position[j] = ByteConverter::ByteSwap(vertices[i].position[j]);
        }
        vertices[i].tex_coords[0] = ByteConverter::ByteSwap(vertices[i].tex_coords[0]);
        vertices[i].tex_coords[1] = ByteConverter::ByteSwap(vertices[i].tex_coords[1]);
        vertices[i].lightmap_coords[0] = ByteConverter::ByteSwap(vertices[i].lightmap_coords[0]);
        vertices[i].lightmap_coords[1] = ByteConverter::ByteSwap(vertices[i].lightmap_coords[1]);
        for (s32 j = 0; j < 3; j++)
        {
            vertices[i].normal[j] = ByteConverter::ByteSwap(vertices[i].normal[j]);
        }
        for (s32 j = 0; j < 4; j++)
        {
            vertices[i].color[j] = ByteConverter::ByteSwap(vertices[i].color[j]);
        }
    }
#endif

    /* read in mesh vertex information */
    s32 num_mesh_vertices = lumps[q3::EBL_MESH_VERTICES].size/sizeof(q3::bsp_mesh_vertex_t);
    mesh_vertices = new q3::bsp_mesh_vertex_t[num_mesh_vertices];
    file->seek(io::EFSP_START, lumps[q3::EBL_MESH_VERTICES].offset);
    file->read(mesh_vertices, lumps[q3::EBL_MESH_VERTICES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_mesh_vertices; i++)
    {
        mesh_vertices[i].vertex_offset = ByteConverter::ByteSwap(mesh_vertices[i].vertex_offset);
    }
#endif

    /* read in effects information */
    s32 num_effects = lumps[q3::EBL_EFFECTS].size/sizeof(q3::bsp_effect_t);
    effects = new q3::bsp_effect_t[num_effects];
    file->seek(io::EFSP_START, lumps[q3::EBL_EFFECTS].offset);
    file->read(effects, lumps[q3::EBL_EFFECTS].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_effects; i++)
    {
        effects[i].brush_index = ByteConverter::ByteSwap(effects[i].brush_index);
        effects[i].unknown = ByteConverter::ByteSwap(effects[i].unknown);
    }
#endif

    /* read in face information */

    s32 num_faces = lumps[q3::EBL_FACES].size/sizeof(q3::bsp_face_t);
    faces = new q3::bsp_face_t[num_faces];
    file->seek(io::EFSP_START, lumps[q3::EBL_FACES].offset);
    file->read(faces, lumps[q3::EBL_FACES].size);
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    for (s32 i = 0; i < num_faces; i++)
    {
        faces[i].tex_id = ByteConverter::ByteSwap(faces[i].tex_id);
        faces[i].effect = ByteConverter::ByteSwap(faces[i].effect);
        faces[i].type = ByteConverter::ByteSwap(faces[i].type);
        faces[i].vert_index = ByteConverter::ByteSwap(faces[i].vert_index);
        faces[i].vert_count = ByteConverter::ByteSwap(faces[i].vert_count);
        faces[i].mesh_vert_index = ByteConverter::ByteSwap(faces[i].mesh_vert_index);
        faces[i].mesh_vert_count = ByteConverter::ByteSwap(faces[i].mesh_vert_count);
        faces[i].lightmap_id = ByteConverter::ByteSwap(faces[i].lightmap_id);
        faces[i].lightmap_corner[0] = ByteConverter::ByteSwap(faces[i].lightmap_corner[0]);
        faces[i].lightmap_corner[1] = ByteConverter::ByteSwap(faces[i].lightmap_corner[1]);
        faces[i].lightmap_size[0] = ByteConverter::ByteSwap(faces[i].lightmap_size[0]);
        faces[i].lightmap_size[1] = ByteConverter::ByteSwap(faces[i].lightmap_size[1]);
        for (s32 j = 0; j < 3; j++)
        {
            faces[i].lightmap_pos[j] = ByteConverter::ByteSwap(faces[i].lightmap_pos[j]);
        }
        for (s32 j = 0; j < 2; j++)
        {
            for (s32 k = 0; k < 3; k++)
            {
                faces[i].lightmap_bits[j][k] = ByteConverter::ByteSwap(faces[i].lightmap_bits[j][k]);
            }
        }
        for (s32 j = 0; j < 3; j++)
        {
            faces[i].normal[j] = ByteConverter::ByteSwap(faces[i].normal[j]);
        }
        faces[i].size[0] = ByteConverter::ByteSwap(faces[i].size[0]);
        faces[i].size[1] = ByteConverter::ByteSwap(faces[i].size[1]);
    }
#endif

    /* read in lightmap information */
    s32 num_lightmaps = lumps[q3::EBL_LIGHTMAPS].size/sizeof(q3::bsp_lightmap_t);
    lightmaps = new q3::bsp_lightmap_t[num_lightmaps];
    file->seek(io::EFSP_START, lumps[q3::EBL_LIGHTMAPS].offset);
    file->read(lightmaps, lumps[q3::EBL_LIGHTMAPS].size);

    /* read in light volume information */
    s32 num_light_volumes = lumps[q3::EBL_LIGHT_VOLUMES].size/sizeof(q3::bsp_light_volume_t);
    light_volumes = new q3::bsp_light_volume_t[num_light_volumes];
    file->seek(io::EFSP_START, lumps[q3::EBL_LIGHT_VOLUMES].offset);
    file->read(light_volumes, lumps[q3::EBL_LIGHT_VOLUMES].size);

    /* read in cluster visibility information */
    file->seek(io::EFSP_START, lumps[q3::EBL_VISIBILITY_DATA].offset);
    file->read(&visibility_data.num_vectors, sizeof(s32));
    file->read(&visibility_data.size_vector, sizeof(s32));
#if defined(__FIRE_ENGINE_BIG_ENDIAN__)
    visibility_data.num_vectors = ByteConverter::ByteSwap(visibility_data.num_vectors);
    visibility_data.size_vector = ByteConverter::ByteSwap(visibility_data.size_vector);
#endif
    visibility_data.vectors = new u8[visibility_data.num_vectors*visibility_data.size_vector];
    file->read(visibility_data.vectors, visibility_data.num_vectors*visibility_data.size_vector);

	delete file;
	return 0x00;
}

void Q3MapLoader::swizzle(vector3f& vector) const
{
	f32 temp = vector.getY();
    vector.setY(vector.getZ());
    vector.setZ(-temp);
}

void Q3MapLoader::swizzle(vector2f& vector) const
{
}

ITexture ** Q3MapLoader::loadTextures(const q3::bsp_texture_t *q3textures, s32 num_textures) const
{
	ITexture ** textures = new ITexture*[num_textures];
	IRenderer * renderer = Device::Get()->getRenderer();
	for (s32 i = 0; i < num_textures; i++)
	{
		string filename = io::FileUtils::StripExtension(q3textures[i].name);
		if (io::FileSystem::Get()->exists(filename + ".jpg"))
		{
			textures[i] = renderer->createTexture(filename + ".jpg");
		}
		else if (io::FileSystem::Get()->exists(filename + ".tga"))
		{
			textures[i] = renderer->createTexture(filename + ".tga");
		}
		if (textures[i] == nullptr)
		{
			Logger::Get()->log(ES_HIGH, "Q3MapLoader", "Could not load texture %s", q3textures[i].name);
			//TODO: add default texture of somesort to avoid crashing
		}
	}
	return textures;
}

}
