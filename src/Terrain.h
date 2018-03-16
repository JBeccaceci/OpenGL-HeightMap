//
//  Terrain.h
//  APPTest
//
//  Created by Juan Beccaceci on 30/5/17.
//  Copyright © 2017 Juan Beccaceci. All rights reserved.
//
#ifndef _HEIGHT_MAP_H
#define _HEIGHT_MAP_H

////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

////////////////////////////////////////////////////////////
/// Render height
////////////////////////////////////////////////////////////
#define HEIGHT_MAP 1
#define HEIGHT_MAP_RAW 1

////////////////////////////////////////////////////////////
// Generic offset evaluator macro
////////////////////////////////////////////////////////////
#ifndef OffsetOf
#		define OffsetOf(Type, Member) __builtin_offsetof(Type, Member)
#endif

////////////////////////////////////////////////////////////
/// HeightMap data
////////////////////////////////////////////////////////////
struct baseTerrain
{
    struct vertexData * Vertex;
    struct textureData * Texture;
    int hID;
    int mapSize;
    unsigned int numVertex;
    unsigned int numIndex;
    unsigned char * vHeight;
    int * hIndex;
};

////////////////////////////////////////////////////////////
/// Vertex data
////////////////////////////////////////////////////////////
struct vertexData
{
    float X;
    float Y;
    float Z;
};

////////////////////////////////////////////////////////////
/// Vertex data
////////////////////////////////////////////////////////////
struct textureData
{
    float tU;
    float tV;
};

////////////////////////////////////////////////////////////
/// Create a new terrain instance
////////////////////////////////////////////////////////////
struct baseTerrain * TerrainCreate();

////////////////////////////////////////////////////////////
/// HeightMap load
////////////////////////////////////////////////////////////
int HeightMap_Load(struct baseTerrain * Terrain, int terraintID);

////////////////////////////////////////////////////////////
/// Normalize heightMap
////////////////////////////////////////////////////////////
void Normal_Height(struct baseTerrain * Terrain);

////////////////////////////////////////////////////////////
/// Create heightMap vertex
////////////////////////////////////////////////////////////
void hMap_Vertex(struct baseTerrain * Terrain);

////////////////////////////////////////////////////////////
/// Create heightMap index
////////////////////////////////////////////////////////////
void hMap_Index(struct baseTerrain * Terrain);

////////////////////////////////////////////////////////////
/// Initialize terrain
////////////////////////////////////////////////////////////
int Initialize_Terrain();

////////////////////////////////////////////////////////////
/// Generate a terrain buffers
////////////////////////////////////////////////////////////
void Terrain_Buffers(struct baseTerrain * Terrain);

////////////////////////////////////////////////////////////
/// Terrain render
////////////////////////////////////////////////////////////
void Terrain_Render();

#endif