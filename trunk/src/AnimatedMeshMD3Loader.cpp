/**
 * FILE:    AnimatedMeshMD3Loader.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD3Loader.cpp 117 2007-10-06 00:31:15Z jpaterso $
 * PURPOSE: Implementation of the AnimatedMeshMD3Loader class.
**/

#include "AnimatedMeshMD3Loader.h"
#include "AnimatedMeshMD3.h"
#include "Logger.h"
#include "ByteConverter.h"
#include "string.h"
#include "IFile.h"
#include "FileSystem.h"
#include "array.h"
#include "Vertex3.h"
#include "Device.h"
#include "IRenderer.h"
#include "ITexture.h"

#include <stdio.h>

#define MD3_MAGIC   0x33504449 // 'IDP3' in little-endian
#define MD3_VERSION 0x0000000F // 15

namespace fire_engine
{

AnimatedMeshMD3Loader::AnimatedMeshMD3Loader()
{
}

AnimatedMeshMD3Loader::~AnimatedMeshMD3Loader()
{
}

AnimatedMeshMD3 * AnimatedMeshMD3Loader::load(const string& filename, io::IFileProvider * fileProvider) const
{
	md3_header_t header;
	md3_bone_frame_t * bone_frames = nullptr;
	md3_tag_t * tags = nullptr;
	md3_mesh_header_t * mesh_headers =nullptr;
	MeshBufferMD3 ** meshes = nullptr;
	array<MD3QuaternionTag*> * tagArray = nullptr;
	AnimatedMeshMD3 * mesh = nullptr;
	io::IFile * file = io::FileSystem::Get()->openReadFile(filename, false, io::EFOF_READ|io::EFOF_BINARY, fileProvider);

	if (file == nullptr)
	{
		Logger::Get()->log(ES_HIGH, "AnimatedMeshMD3Loader",
			"Could not open %s for reading", filename.c_str());
		return 0;
	}

	file->read(&header, sizeof(md3_header_t));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	header.magic = ByteConverter::ByteSwap(header.magic);
	header.version = ByteConverter::ByteSwap(header.version);
	header.num_bone_frames = ByteConverter::ByteSwap(header.num_bone_frames);
	header.num_tags = ByteConverter::ByteSwap(header.num_tags);
	header.num_meshes = ByteConverter::ByteSwap(header.num_meshes);
	header.num_max_skins = ByteConverter::ByteSwap(header.num_max_skins);
	header.header_size = ByteConverter::ByteSwap(header.header_size);
	header.offset_tags = ByteConverter::ByteSwap(header.offset_tags);
	header.offset_meshes = ByteConverter::ByteSwap(header.offset_meshes);
	header.offset_eof = ByteConverter::ByteSwap(header.offset_eof);
#endif

#if defined(_FIRE_ENGINE_DEBUG_MD3_)
	printf("Statistics for %s\n", filename.c_str());
	printf("---------------");
	for (s32 i = 0; i < filename.length(); i++)
		putchar('-');
	putchar('\n');
	printf("Magic Number:              %d\n", header.magic);
	printf("Version:                   %d\n", header.version);
	printf("Name:                      %s\n", header.name);
	printf("Number of bone frames:     %d\n", header.num_bone_frames);
	printf("Number of tags:            %d\n", header.num_tags);
	printf("Number of meshes:          %d\n", header.num_meshes);
	printf("Maximum number of skins:   %d\n", header.num_max_skins);
	printf("Size of MD3 header:        %d\n", header.header_size);
	printf("Offset of tags:            %d\n", header.offset_tags);
	printf("Offset of meshes:          %d\n", header.offset_meshes);
	printf("Offset of EOF (file size): %d\n\n", header.offset_eof);
#endif

	if (header.magic != MD3_MAGIC || header.version != MD3_VERSION)
	{
		Logger::Get()->log(ES_HIGH, "AnimatedMeshMD2Loader",
			"Invalid header in %s: expected (%d, %d) as (magic, version), got (%d, %d)",
			file->getFilename().c_str(), MD3_MAGIC, MD3_VERSION, header.magic, header.version);
		return 0;
	}

	bone_frames = new md3_bone_frame_t[header.num_bone_frames];
	file->read(bone_frames, header.num_bone_frames*sizeof(md3_bone_frame_t));

	tags = new md3_tag_t[header.num_tags*header.num_bone_frames];
	file->seek(io::EFSP_START, header.offset_tags);
	file->read(tags, header.num_tags*header.num_bone_frames*sizeof(md3_tag_t));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	for (s32 i = 0; i < header.num_bone_frames; i++)
	{
		bone_frames[i].min_bounds[0] = ByteConverter::ByteSwap(bone_frames[i].min_bounds[0]);
		bone_frames[i].min_bounds[1] = ByteConverter::ByteSwap(bone_frames[i].min_bounds[1]);
		bone_frames[i].min_bounds[2] = ByteConverter::ByteSwap(bone_frames[i].min_bounds[2]);
		bone_frames[i].max_bounds[0] = ByteConverter::ByteSwap(bone_frames[i].max_bounds[0]);
		bone_frames[i].max_bounds[1] = ByteConverter::ByteSwap(bone_frames[i].max_bounds[1]);
		bone_frames[i].max_bounds[2] = ByteConverter::ByteSwap(bone_frames[i].max_bounds[2]);
		bone_frames[i].local_origin[0] = ByteConverter::ByteSwap(bone_frames[i].local_origin[0]);
		bone_frames[i].local_origin[1] = ByteConverter::ByteSwap(bone_frames[i].local_origin[1]);
		bone_frames[i].local_origin[2] = ByteConverter::ByteSwap(bone_frames[i].local_origin[2]);
		bone_frames[i].bounding_sphere_radius = ByteConverter::ByteSwap(bone_frames[i].bounding_sphere_radius);
	}
	for (s32 i = 0; i < header.num_tags*header.num_bone_frames; i++)
	{
		tags[i].origin[0] = ByteConverter::ByteSwap(tags[i].origin[0]);
		tags[i].origin[1] = ByteConverter::ByteSwap(tags[i].origin[1]);
		tags[i].origin[2] = ByteConverter::ByteSwap(tags[i].origin[2]);
		for (s32 j = 0; i < 9; j++)
			tags[i].orientation[j] = ByteConverter::ByteSwap(tags[i].orientation[j]);
	}
#endif

	/* Create the tags
	 They are stored sorted on KEYFRAME, not on the tag name, so for tags A, B, C,
	 it will look like A1 B1 C1 A2 B2 C2 ... */
	tagArray = new array<MD3QuaternionTag*>(header.num_tags);
	for (s32 i = 0; i < header.num_tags; i++)
	{
		MD3QuaternionTag * tags_for_frames = new MD3QuaternionTag[header.num_bone_frames];
		for (s32 j = 0; j < header.num_bone_frames; j++)
		{
			tags_for_frames[j].Position.set(tags[j*header.num_tags+i].origin);
			tags_for_frames[j].Name = tags[j*header.num_tags+i].name;
			// the rotation matrix is stored in column-major format
			tags_for_frames[j].Rotation = matrix4f::BuildFrom3x3(tags[j*header.num_tags+i].orientation, true);
		}
		tagArray->push_back(tags_for_frames);
	}

	meshes = new MeshBufferMD3*[header.num_meshes];
	Vertex3 * vertices = nullptr;
	array<u32> * indices = nullptr;
	md3_vertex_t * frame_vertices = nullptr;
	md3_triangle_t * triangles = nullptr;
	md3_tex_coords_t * tex_coords = nullptr;
	md3_shader_t * shaders = nullptr;
	ITexture * tex = nullptr;
	s32 offset = header.offset_meshes;
	mesh_headers = new md3_mesh_header_t[header.num_meshes];
	for (s32 i = 0; i < header.num_meshes; i++)
	{
		file->seek(io::EFSP_START, offset);
		file->read(&mesh_headers[i], sizeof(md3_mesh_header_t));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
		mesh_headers[i].magic = ByteConverter::ByteSwap(mesh_headers[i].magic);
		mesh_headers[i].num_frames = ByteConverter::ByteSwap(mesh_headers[i].num_frames);
		mesh_headers[i].num_shaders = ByteConverter::ByteSwap(mesh_headers[i].num_shaders);
		mesh_headers[i].num_verts = ByteConverter::ByteSwap(mesh_headers[i].num_verts);
		mesh_headers[i].num_triangles = ByteConverter::ByteSwap(mesh_headers[i].num_triangles);
		mesh_headers[i].offset_triangles = ByteConverter::ByteSwap(mesh_headers[i].offset_triangles);
		mesh_headers[i].offset_shaders = ByteConverter::ByteSwap(mesh_headers[i].offset_shaders);
		mesh_headers[i].offset_tex_coords = ByteConverter::ByteSwap(mesh_headers[i].offset_tex_coords);
		mesh_headers[i].offset_vertices = ByteConverter::ByteSwap(mesh_headers[i].offset_vertices);
		mesh_headers[i].offset_end = ByteConverter::ByteSwap(mesh_headers[i].offset_end);
#endif

#if defined(_FIRE_ENGINE_DEBUG_MD3_)
		printf("Loaded mesh with %d triangles\n", mesh_headers[i].num_triangles);
#endif

		file->seek(io::EFSP_START, offset+mesh_headers[i].offset_triangles);
		triangles = new md3_triangle_t[mesh_headers[i].num_triangles];
		file->read(triangles, mesh_headers[i].num_triangles*sizeof(md3_triangle_t));

		file->seek(io::EFSP_START, offset+mesh_headers[i].offset_shaders);
		shaders = new md3_shader_t[mesh_headers[i].num_shaders];
		file->read(shaders, mesh_headers[i].num_shaders*sizeof(md3_shader_t));

		file->seek(io::EFSP_START, offset+mesh_headers[i].offset_tex_coords);
		tex_coords = new md3_tex_coords_t[mesh_headers[i].offset_tex_coords];
		file->read(tex_coords, mesh_headers[i].num_verts*sizeof(md3_tex_coords_t));

		file->seek(io::EFSP_START, offset+mesh_headers[i].offset_vertices);
		frame_vertices = new md3_vertex_t[mesh_headers[i].num_frames*mesh_headers[i].num_verts];
		file->read(frame_vertices, mesh_headers[i].num_frames*mesh_headers[i].num_verts*sizeof(md3_vertex_t));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
		for (s32 j = 0; j < mesh_headers[i].num_triangles; j++)
		{
			triangles[j][0] = ByteConverter::ByteSwap(triangles[j][0]);
			triangles[j][1] = ByteConverter::ByteSwap(triangles[j][1]);
			triangles[j][2] = ByteConverter::ByteSwap(triangles[j][2]);
		}
		for (s32 j = 0; j < mesh_headers[i].num_shaders; j++)
			shaders[j].shader_index = ByteConverter::ByteSwap(shaders[j].shader_index);
		for (s32 j = 0; j < mesh_headers[i].num_verts; j++)
		{
			tex_coords[j][0] = ByteConverter::ByteSwap(tex_coords[j][0]);
			tex_coords[j][1] = ByteConverter::ByteSwap(tex_coords[j][1]);
			for (s32 k = 0; k < mesh_headers[i].num_frames; k++)
			{
				frame_vertices[j*k].x = ByteConverter::ByteSwap(frame_vertices[j*k].x);
				frame_vertices[j*k].y = ByteConverter::ByteSwap(frame_vertices[j*k].y);
				frame_vertices[j*k].z = ByteConverter::ByteSwap(frame_vertices[j*k].z);
				frame_vertices[j*k].normal = ByteConverter::ByteSwap(frame_vertices[j*k].normal);
			}
		}
#endif

		// Now we have all the mesh information, create a new MeshBufferMD3
		vertices = new Vertex3[mesh_headers[i].num_frames*mesh_headers[i].num_verts];
		f32 scale = 1.0f / 64.0f;
		for (s32 j = 0; j < mesh_headers[i].num_frames*mesh_headers[i].num_verts; j++)
		{
			// Convert from spherical coordinates to 3-dimensional x, y, z coordinates
			f32 lat = ((frame_vertices[j].normal >> 8) & 0xFF)*Math32::TWO_PI*Math32::ONE_OVER_TWO_FIVE_FIVE;
			f32 lng = (frame_vertices[j].normal & 0xFF)*Math32::TWO_PI*Math32::ONE_OVER_TWO_FIVE_FIVE;
			vertices[j].setPosition(frame_vertices[j].x*scale, frame_vertices[j].y*scale, frame_vertices[j].z*scale);
			vertices[j].setNormal(Math32::Cos(lat)*Math32::Sin(lng), Math32::Sin(lat)*Math32::Sin(lng),
				Math32::Cos(lng));
			vertices[j].setTextureCoordinates(tex_coords[j%mesh_headers[i].num_verts][0],
				tex_coords[j%mesh_headers[i].num_verts][1]);
		}

		indices = new array<u32>(mesh_headers[i].num_triangles*3);
		for (s32 j = 0; j < mesh_headers[i].num_triangles; j++)
		{
			// MD3 uses clock-wise triangles, so invert them
			indices->push_back(triangles[j][2]);
			indices->push_back(triangles[j][1]);
			indices->push_back(triangles[j][0]);
		}

		// We need the IRenderer to create the textures for us
		if (strlen(shaders[0].name) > 0)
		{
			tex = Device::Get()->getRenderer()->createTexture(shaders[0].name, fileProvider);
		}
		else
		{
			tex = nullptr;
		}
		meshes[i] = new MeshBufferMD3(vertices, indices, tex, mesh_headers[i].num_verts, mesh_headers[i].num_frames);
		if (tex)
		{
			tex->drop();
		}
		offset = offset+mesh_headers[i].offset_end;

		delete [] triangles;
		delete [] shaders;
		delete [] tex_coords;
		delete [] frame_vertices;
	}

	delete [] bone_frames;
	delete [] tags;
	delete [] mesh_headers;

	if (file->fail())
	{
		Logger::Get()->log(ES_HIGH, "AnimatedMeshMD3Loader",
			"An error occurred while loading %s", file->getFilename().c_str());
		for (s32 i = 0; i < header.num_meshes; i++)
		{
			delete meshes[i];
		}
		delete [] meshes;
		mesh = nullptr;
	}
	else
	{
		mesh = new AnimatedMeshMD3(meshes, header.num_meshes, header.num_bone_frames, tagArray);
	}

	delete file;

	for (s32 i = 0; i < header.num_meshes; i++)
	{
		meshes[i]->drop();
	}

	return mesh;
}

}
