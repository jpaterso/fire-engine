/**
 * FILE:    AnimatedMeshMD2Loader.cpp
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: AnimatedMeshMD2Loader.cpp 116 2007-10-05 13:22:45Z jpaterso $
 * PURPOSE: A class for loading .MD2 files, and creating AnimatedMeshMD2 objects
**/

#include "AnimatedMeshMD2Loader.h"
#include "ByteConverter.h"
#include "Logger.h"
#include "string.h"
#include "IFile.h"
#include "FileSystem.h"
#include <stdio.h>

#define MD2_MAGIC          0x32504449 // 'IDP2'
#define MD2_VERSION        0x00000008
#define MD2_MAX_TRIANGLES  0x00001000 // 4096
#define MD2_MAX_VERTICES   0x00000800 // 2048
#define MD2_MAX_TEX_COORDS 0x00000800 // 2048
#define MD2_MAX_FRAMES     0x00000200 // 512
#define MD2_MAX_SKINS      0x00000020 // 32

namespace fire_engine
{

vector3f AnimatedMeshMD2Loader::m_normal_list[__FIRE_ENGINE_MAX_MD2_NORMALS] =
{
	vector3f(-0.525731f, 0.000000f, 0.850651f, true),
	vector3f(-0.442863f, 0.238856f, 0.864188f, true),
	vector3f(-0.295242f, 0.000000f, 0.955423f, true),
	vector3f(-0.309017f, 0.500000f, 0.809017f, true),
	vector3f(-0.162460f, 0.262866f, 0.951056f, true),
	vector3f( 0.000000f, 0.000000f, 1.000000f, true),
	vector3f( 0.000000f, 0.850651f, 0.525731f, true),
	vector3f(-0.147621f, 0.716567f, 0.681718f, true),
	vector3f(0.147621f, 0.716567f, 0.681718f, true),
	vector3f(0.000000f, 0.525731f, 0.850651f, true),
	vector3f(0.309017f, 0.500000f, 0.809017f, true),
	vector3f(0.525731f, 0.000000f, 0.850651f, true),
	vector3f(0.295242f, 0.000000f, 0.955423f, true),
	vector3f(0.442863f, 0.238856f, 0.864188f, true),
	vector3f(0.162460f, 0.262866f, 0.951056f, true),
	vector3f(-0.681718f, 0.147621f, 0.716567f, true),
	vector3f(-0.809017f, 0.309017f, 0.500000f, true),
	vector3f(-0.587785f, 0.425325f, 0.688191f, true),
	vector3f(-0.850651f, 0.525731f, 0.000000f, true),
	vector3f(-0.864188f, 0.442863f, 0.238856f, true),
	vector3f(-0.716567f, 0.681718f, 0.147621f, true),
	vector3f(-0.688191f, 0.587785f, 0.425325f, true),
	vector3f(-0.500000f, 0.809017f, 0.309017f, true),
	vector3f(-0.238856f, 0.864188f, 0.442863f, true),
	vector3f(-0.425325f, 0.688191f, 0.587785f, true),
	vector3f(-0.716567f, 0.681718f, -0.147621f, true),
	vector3f(-0.500000f, 0.809017f, -0.309017f, true),
	vector3f(-0.525731f, 0.850651f, 0.000000f, true),
	vector3f(0.000000f, 0.850651f, -0.525731f, true),
	vector3f(-0.238856f, 0.864188f, -0.442863f, true),
	vector3f(0.000000f, 0.955423f, -0.295242f, true),
	vector3f(-0.262866f, 0.951056f, -0.162460f, true),
	vector3f(0.000000f, 1.000000f, 0.000000f, true),
	vector3f(0.000000f, 0.955423f, 0.295242f, true),
	vector3f(-0.262866f, 0.951056f, 0.162460f, true),
	vector3f(0.238856f, 0.864188f, 0.442863f, true),
	vector3f(0.262866f, 0.951056f, 0.162460f, true),
	vector3f(0.500000f, 0.809017f, 0.309017f, true),
	vector3f(0.238856f, 0.864188f, -0.442863f, true),
	vector3f(0.262866f, 0.951056f, -0.162460f, true),
	vector3f(0.500000f, 0.809017f, -0.309017f, true),
	vector3f(0.850651f, 0.525731f, 0.000000f, true),
	vector3f(0.716567f, 0.681718f, 0.147621f, true),
	vector3f(0.716567f, 0.681718f, -0.147621f, true),
	vector3f(0.525731f, 0.850651f, 0.000000f, true),
	vector3f(0.425325f, 0.688191f, 0.587785f, true),
	vector3f(0.864188f, 0.442863f, 0.238856f, true),
	vector3f(0.688191f, 0.587785f, 0.425325f, true),
	vector3f(0.809017f, 0.309017f, 0.500000f, true),
	vector3f(0.681718f, 0.147621f, 0.716567f, true),
	vector3f(0.587785f, 0.425325f, 0.688191f, true),
	vector3f(0.955423f, 0.295242f, 0.000000f, true),
	vector3f(1.000000f, 0.000000f, 0.000000f, true),
	vector3f(0.951056f, 0.162460f, 0.262866f, true),
	vector3f(0.850651f, -0.525731f, 0.000000f, true),
	vector3f(0.955423f, -0.295242f, 0.000000f, true),
	vector3f(0.864188f, -0.442863f, 0.238856f, true),
	vector3f(0.951056f, -0.162460f, 0.262866f, true),
	vector3f(0.809017f, -0.309017f, 0.500000f, true),
	vector3f(0.681718f, -0.147621f, 0.716567f, true),
	vector3f(0.850651f, 0.000000f, 0.525731f, true),
	vector3f(0.864188f, 0.442863f, -0.238856f, true),
	vector3f(0.809017f, 0.309017f, -0.500000f, true),
	vector3f(0.951056f, 0.162460f, -0.262866f, true),
	vector3f(0.525731f, 0.000000f, -0.850651f, true),
	vector3f(0.681718f, 0.147621f, -0.716567f, true),
	vector3f(0.681718f, -0.147621f, -0.716567f, true),
	vector3f(0.850651f, 0.000000f, -0.525731f, true),
	vector3f(0.809017f, -0.309017f, -0.500000f, true),
	vector3f(0.864188f, -0.442863f, -0.238856f, true),
	vector3f(0.951056f, -0.162460f, -0.262866f, true),
	vector3f(0.147621f, 0.716567f, -0.681718f, true),
	vector3f(0.309017f, 0.500000f, -0.809017f, true),
	vector3f(0.425325f, 0.688191f, -0.587785f, true),
	vector3f(0.442863f, 0.238856f, -0.864188f, true),
	vector3f(0.587785f, 0.425325f, -0.688191f, true),
	vector3f(0.688191f, 0.587785f, -0.425325f, true),
	vector3f(-0.147621f, 0.716567f, -0.681718f, true),
	vector3f(-0.309017f, 0.500000f, -0.809017f, true),
	vector3f(0.000000f, 0.525731f, -0.850651f, true),
	vector3f(-0.525731f, 0.000000f, -0.850651f, true),
	vector3f(-0.442863f, 0.238856f, -0.864188f, true),
	vector3f(-0.295242f, 0.000000f, -0.955423f, true),
	vector3f(-0.162460f, 0.262866f, -0.951056f, true),
	vector3f(0.000000f, 0.000000f, -1.000000f, true),
	vector3f(0.295242f, 0.000000f, -0.955423f, true),
	vector3f(0.162460f, 0.262866f, -0.951056f, true),
	vector3f(-0.442863f, -0.238856f, -0.864188f, true),
	vector3f(-0.309017f, -0.500000f, -0.809017f, true),
	vector3f(-0.162460f, -0.262866f, -0.951056f, true),
	vector3f( 0.000000f, -0.850651f, -0.525731f, true),
	vector3f(-0.147621f, -0.716567f, -0.681718f, true),
	vector3f(0.147621f, -0.716567f, -0.681718f, true),
	vector3f(0.000000f, -0.525731f, -0.850651f, true),
	vector3f(0.309017f, -0.500000f, -0.809017f, true),
	vector3f(0.442863f, -0.238856f, -0.864188f, true),
	vector3f(0.162460f, -0.262866f, -0.951056f, true),
	vector3f(0.238856f, -0.864188f, -0.442863f, true),
	vector3f(0.500000f, -0.809017f, -0.309017f, true),
	vector3f(0.425325f, -0.688191f, -0.587785f, true),
	vector3f(0.716567f, -0.681718f, -0.147621f, true),
	vector3f(0.688191f, -0.587785f, -0.425325f, true),
	vector3f(0.587785f, -0.425325f, -0.688191f, true),
	vector3f(0.000000f, -0.955423f, -0.295242f, true),
	vector3f(0.000000f, -1.000000f, 0.000000f, true),
	vector3f(0.262866f, -0.951056f, -0.162460f, true),
	vector3f(0.000000f, -0.850651f, 0.525731f, true),
	vector3f(0.000000f, -0.955423f, 0.295242f, true),
	vector3f(0.238856f, -0.864188f, 0.442863f, true),
	vector3f(0.262866f, -0.951056f, 0.162460f, true),
	vector3f(0.500000f, -0.809017f, 0.309017f, true),
	vector3f(0.716567f, -0.681718f, 0.147621f, true),
	vector3f(0.525731f, -0.850651f, 0.000000f, true),
	vector3f(-0.238856f, -0.864188f, -0.442863f, true),
	vector3f(-0.500000f, -0.809017f, -0.309017f, true),
	vector3f(-0.262866f, -0.951056f, -0.162460f, true),
	vector3f(-0.850651f, -0.525731f, 0.000000f, true),
	vector3f(-0.716567f, -0.681718f, -0.147621f, true),
	vector3f(-0.716567f, -0.681718f, 0.147621f, true),
	vector3f(-0.525731f, -0.850651f, 0.000000f, true),
	vector3f(-0.500000f, -0.809017f, 0.309017f, true),
	vector3f(-0.238856f, -0.864188f, 0.442863f, true),
	vector3f(-0.262866f, -0.951056f, 0.162460f, true),
	vector3f(-0.864188f, -0.442863f, 0.238856f, true),
	vector3f(-0.809017f, -0.309017f, 0.500000f, true),
	vector3f(-0.688191f, -0.587785f, 0.425325f, true),
	vector3f(-0.681718f, -0.147621f, 0.716567f, true),
	vector3f(-0.442863f, -0.238856f, 0.864188f, true),
	vector3f(-0.587785f, -0.425325f, 0.688191f, true),
	vector3f(-0.309017f, -0.500000f, 0.809017f, true),
	vector3f(-0.147621f, -0.716567f, 0.681718f, true),
	vector3f(-0.425325f, -0.688191f, 0.587785f, true),
	vector3f(-0.162460f, -0.262866f, 0.951056f, true),
	vector3f(0.442863f, -0.238856f, 0.864188f, true),
	vector3f(0.162460f, -0.262866f, 0.951056f, true),
	vector3f(0.309017f, -0.500000f, 0.809017f, true),
	vector3f(0.147621f, -0.716567f, 0.681718f, true),
	vector3f(0.000000f, -0.525731f, 0.850651f, true),
	vector3f(0.425325f, -0.688191f, 0.587785f, true),
	vector3f(0.587785f, -0.425325f, 0.688191f, true),
	vector3f(0.688191f, -0.587785f, 0.425325f, true),
	vector3f(-0.955423f, 0.295242f, 0.000000f, true),
	vector3f(-0.951056f, 0.162460f, 0.262866f, true),
	vector3f(-1.000000f, 0.000000f, 0.000000f, true),
	vector3f(-0.850651f, 0.000000f, 0.525731f, true),
	vector3f(-0.955423f, -0.295242f, 0.000000f, true),
	vector3f(-0.951056f, -0.162460f, 0.262866f, true),
	vector3f(-0.864188f, 0.442863f, -0.238856f, true),
	vector3f(-0.951056f, 0.162460f, -0.262866f, true),
	vector3f(-0.809017f, 0.309017f, -0.500000f, true),
	vector3f(-0.864188f, -0.442863f, -0.238856f, true),
	vector3f(-0.951056f, -0.162460f, -0.262866f, true),
	vector3f(-0.809017f, -0.309017f, -0.500000f, true),
	vector3f(-0.681718f, 0.147621f, -0.716567f, true),
	vector3f(-0.681718f, -0.147621f, -0.716567f, true),
	vector3f(-0.850651f, 0.000000f, -0.525731f, true),
	vector3f(-0.688191f, 0.587785f, -0.425325f, true),
	vector3f(-0.587785f, 0.425325f, -0.688191f, true),
	vector3f(-0.425325f, 0.688191f, -0.587785f, true),
	vector3f(-0.425325f, -0.688191f, -0.587785f, true),
	vector3f(-0.587785f, -0.425325f, -0.688191f, true),
	vector3f(-0.688191f, -0.587785f, -0.425325f, true)
};

AnimatedMeshMD2Loader::AnimatedMeshMD2Loader()
{
}

AnimatedMeshMD2Loader::~AnimatedMeshMD2Loader()
{
}

AnimatedMeshMD2 * AnimatedMeshMD2Loader::load(const string& filename, io::IFileProvider * fileProvider) const
{
	AnimatedMeshMD2 *  amesh      = 0;
	md2_skin_t *       skins      = 0;
	md2_triangle_t *   triangles  = 0;
	md2_tex_coords_t * tex_coords = 0;
	md2_frame_t *      frames     = 0;
	s32 *              glCommands = 0; //TODO fix GL commands, make it a proper type
	MD2Header          md2h;
	io::IFile * file = io::FileSystem::Get()->openReadFile(filename, false, io::EFOF_READ|io::EFOF_BINARY, fileProvider);

	if (file == 0)
	{
		Logger::Get()->log(ES_HIGH, "AnimatedMeshMD2Loader", "Could not open %s for reading", filename.c_str());
		return nullptr;
	}

	file->read(&md2h, sizeof(MD2Header));

#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
	md2h.magic = ByteConverter::ByteSwap(md2h.magic);
	md2h.version = ByteConverter::ByteSwap(md2h.version);
	md2h.skin_width = ByteConverter::ByteSwap(md2h.skin_width);
	md2h.skin_height = ByteConverter::ByteSwap(md2h.skin_height);
	md2h.framesize = ByteConverter::ByteSwap(md2h.framesize);
	md2h.num_skins = ByteConverter::ByteSwap(md2h.num_skins);
	md2h.num_vertices = ByteConverter::ByteSwap(md2h.num_vertices);
	md2h.num_tex_coords = ByteConverter::ByteSwap(md2h.num_tex_coords);
	md2h.num_triangles = ByteConverter::ByteSwap(md2h.num_triangles);
	md2h.num_gl_commands = ByteConverter::ByteSwap(md2h.num_gl_commands);
	md2h.num_frames = ByteConverter::ByteSwap(md2h.num_frames);
	md2h.offset_skins = ByteConverter::ByteSwap(md2h.offset_skins);
	md2h.offset_tex_coords = ByteConverter::ByteSwap(md2h.offset_tex_coords);
	md2h.offset_triangles = ByteConverter::ByteSwap(md2h.offset_triangles);
	md2h.offset_frames = ByteConverter::ByteSwap(md2h.offset_frames);
	md2h.offset_gl_commands = ByteConverter::ByteSwap(md2h.offset_gl_commands);
	md2h.offset_end_file = ByteConverter::ByteSwap(md2h.offset_end_file);
#endif

#if defined(_FIRE_ENGINE_DEBUG_MD2_)
	printf("Statistics for %s\n", filename.c_str());
	printf("---------------");
	for (s32 i = 0; i < filename.length(); i++)
		putchar('-');
	putchar('\n');
	printf("Magic Number:                  %x\n", md2h.magic);
	printf("Version:                       %d\n", md2h.version);
	printf("Skin Width (pixels):           %d\n", md2h.skin_width);
	printf("Skin Height (pixels):          %d\n", md2h.skin_height);
	printf("Frame size:                    %d\n", md2h.framesize);
	printf("Number of skins:               %d\n", md2h.num_skins);
	printf("Number of vertices:            %d\n", md2h.num_vertices);
	printf("Number of texture coordinates: %d\n", md2h.num_tex_coords);
	printf("Number of triangles:           %d\n", md2h.num_triangles);
	printf("Number of OpenGL commands:     %d\n", md2h.num_gl_commands);
	printf("Number of frames:              %d\n", md2h.num_frames);
	printf("Offset of skin list:           %d\n", md2h.offset_skins);
	printf("Offset of texture coordinates: %d\n", md2h.offset_tex_coords);
	printf("Offset of triangles:           %d\n", md2h.offset_triangles);
	printf("Offset of frames:              %d\n", md2h.offset_frames);
	printf("Offset of OpenGL commands:     %d\n", md2h.offset_gl_commands);
	printf("Offset to end of file (size):  %d\n\n", md2h.offset_end_file);
#endif

	if (md2h.magic != MD2_MAGIC || md2h.version != MD2_VERSION)
	{
		Logger::Get()->log(ES_HIGH, "AnimatedMeshMD2Loader",
			"Invalid header in %s: expected (%d, %d) as (magic, version), got (%d, %d)",
			file->getFilename().c_str(), MD2_MAGIC, MD2_VERSION, md2h.magic, md2h.version);
		return 0;
	}

	skins = new md2_skin_t[md2h.num_skins];
	file->seek(io::EFSP_START, md2h.offset_skins);
	file->read(skins, sizeof(md2_skin_t)*md2h.num_skins);

	tex_coords = new md2_tex_coords_t[md2h.num_tex_coords];
	file->seek(io::EFSP_START, md2h.offset_tex_coords);
	file->read(tex_coords, sizeof(md2_tex_coords_t)*md2h.num_tex_coords);

	triangles = new md2_triangle_t[md2h.num_triangles];
	file->seek(io::EFSP_START, md2h.offset_triangles);
	file->read(triangles, sizeof(md2_triangle_t)*md2h.num_triangles);

	glCommands = new s32[md2h.num_gl_commands];
	file->seek(io::EFSP_START, md2h.offset_gl_commands);
	file->read(glCommands, sizeof(s32)*md2h.num_gl_commands);

	frames = new md2_frame_t[md2h.num_frames];
	file->seek(io::EFSP_START, md2h.offset_frames);
	for (s32 i = 0; i < md2h.num_frames; i++)
	{
		frames[i].vertices = new md2_vertex_t[md2h.num_vertices];
		file->read(&frames[i].scale, sizeof(md2_vec3_t));
		file->read(&frames[i].translate, sizeof(md2_vec3_t));
#if defined(_FIRE_ENGINE_BIG_ENDIAN_)
		for (s32 j = 0; j < 3; j++)
		{
			frames[i].scale[j] = ByteConverter::ByteSwap(frames[i].scale[j]);
			frames[i].translate[j] = ByteConverter::ByteSwap(frames[i].translate[j]);
		}
#endif
		file->read(frames[i].name, sizeof(c8)*16);
		file->read(frames[i].vertices, sizeof(md2_vertex_t)*md2h.num_vertices);
	}

	if (file->fail()) // some i/o error occurred, abort!
	{
		Logger::Get()->log(ES_HIGH, "AnimatedMeshMD2Loader",
			"An error occurred while loading MD2 model %s, aborting", file->getFilename().c_str());
		amesh = 0x00;
	}
	else
		amesh = fromMD2Structures(md2h, skins, triangles, tex_coords, frames);

	// Clean-up, and return
	delete [] skins;
	delete [] tex_coords;
	delete [] triangles;
	delete [] glCommands;
	for (s32 i = 0; i < md2h.num_frames; i++)
		delete [] frames[i].vertices;
	delete [] frames;
	delete file;
	return amesh;
}

AnimatedMeshMD2 * AnimatedMeshMD2Loader::fromMD2Structures(MD2Header md2h, md2_skin_t * skins,
    md2_triangle_t * triangles, md2_tex_coords_t * tex_coords, md2_frame_t * frames) const
{
	array<u32> * mesh_triangles  = new array<u32>(md2h.num_triangles*3);
	Vertex3 * vertices           = new Vertex3[md2h.num_frames*md2h.num_vertices];
	f32 v[3];
	for (s32 i = 0; i < md2h.num_frames; i++)
	{
		vector3f scale     = vector3f(frames[i].scale);
		vector3f translate = vector3f(frames[i].translate);
		for (s32 j = 0; j < md2h.num_vertices; j++)
		{
			v[0] = frames[i].vertices[j].v[0]*scale.getX()+translate.getX();
			v[1] = frames[i].vertices[j].v[1]*scale.getY()+translate.getY();
			v[2] = frames[i].vertices[j].v[2]*scale.getZ()+translate.getZ();
			vertices[i*md2h.num_vertices+j].setPosition(vector3f(v));
			vertices[i*md2h.num_vertices+j].setNormal(m_normal_list[frames[i].vertices[j].normal_index]);
		}
	}
	for (s32 i = 0; i < md2h.num_triangles; i++)
	{
		// MD2 uses clockwise triangles: invert the order
		mesh_triangles->push_back(triangles[i].vertex[2]);
		mesh_triangles->push_back(triangles[i].vertex[1]);
		mesh_triangles->push_back(triangles[i].vertex[0]);
		for (s32 j = 0; j < md2h.num_frames; j++)
		{
			vector2f texcoord = vector2f(static_cast<f32>(tex_coords[triangles[i].tex_coords[2]].s)/md2h.skin_width,
				static_cast<f32>(tex_coords[triangles[i].tex_coords[2]].t)/md2h.skin_height);
			vertices[j*md2h.num_vertices+triangles[i].vertex[2]].setTextureCoordinates(texcoord);
			texcoord = vector2f(static_cast<f32>(tex_coords[triangles[i].tex_coords[1]].s)/md2h.skin_width,
				static_cast<f32>(tex_coords[triangles[i].tex_coords[1]].t)/md2h.skin_height);
			vertices[j*md2h.num_vertices+triangles[i].vertex[1]].setTextureCoordinates(texcoord);
			texcoord = vector2f(static_cast<f32>(tex_coords[triangles[i].tex_coords[0]].s)/md2h.skin_width,
				static_cast<f32>(tex_coords[triangles[i].tex_coords[0]].t)/md2h.skin_height);
			vertices[j*md2h.num_vertices+triangles[i].vertex[0]].setTextureCoordinates(texcoord);
		}
	}

	AnimatedMeshMD2 * ammd2 = new AnimatedMeshMD2("default md2 mesh", md2h.num_frames,
		md2h.num_vertices, vertices, mesh_triangles, 0);

	return ammd2;
}

}
