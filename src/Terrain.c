//
//  Terrain.c
//  
//
//  Created by Juan Beccaceci on 30/5/17.
//  Copyright © 2017 Juan Beccaceci. All rights reserved.
//
#include <OpenGL/gl.h>
#include <math.h>
#include "Terrain.h"
#include "Texture.h"
#include "bitMap.h"

unsigned int vertexArray;
unsigned int vertexBuffer;
unsigned int m_GLTex0Buffer;
unsigned int indexBuffer;
unsigned int indexCount;

////////////////////////////////////////////////////////////
/// Terrain texture
////////////////////////////////////////////////////////////
struct TextureData * Texture;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

////////////////////////////////////////////////////////////
/// Create a new terrain instance
////////////////////////////////////////////////////////////
struct baseTerrain * TerrainCreate()
{
    struct baseTerrain * newTerrain = malloc(sizeof(struct baseTerrain));
    
    if (newTerrain)
    {
        newTerrain->hID = 0;
        newTerrain->mapSize = 0;
        newTerrain->numVertex = 0;
        newTerrain->numIndex = 0;
        newTerrain->vHeight = NULL;
        newTerrain->hIndex = NULL;
        newTerrain->Vertex = NULL;
        newTerrain->Texture = NULL;
        
        return newTerrain;
    }
    
    return NULL;
}

////////////////////////////////////////////////////////////
/// HeightMap load
////////////////////////////////////////////////////////////
int HeightMap_Load(struct baseTerrain * Terrain, int terraintID)
{
    FILE * tmpFile;
    long fileSize     = 0;
    int heightMapSize = 0;
    int Result        = 0;
    
    if (Terrain)
    {
        if ((tmpFile = fopen("heightMap.raw", "rb")))
        {
            //	Get fileSize
            fseek(tmpFile, 0, SEEK_END);
            fileSize = ftell(tmpFile);
            fseek(tmpFile, 0, SEEK_SET);
            
            //	Allocate memory
            Terrain->vHeight = malloc(sizeof(unsigned char) * fileSize);
            
            //	Read data
            fread((char *)Terrain->vHeight, 1, fileSize, tmpFile);
            
            //	HeightMap size
            heightMapSize = (int)sqrt( (float)fileSize);
            Terrain->mapSize = heightMapSize - 1;
            
            //	Calculate numVertex
            Terrain->numVertex = Terrain->mapSize * Terrain->mapSize;
            
            //  Calculate numIndex
            Terrain->numIndex  = (Terrain->mapSize - 1) * (Terrain->mapSize - 1) * 6;
            
//            printf("Vertices: %u - Indices: %u", Terrain->mapSize, Terrain->numIndex);
            
            //HARDCODE---->
            indexCount = Terrain->numIndex;
            //HARDCODE---->
        
            //  Allocate vertex data
            Terrain->Vertex = malloc(sizeof(struct vertexData) * Terrain->numVertex);
            
            //  Allocate index
            Terrain->hIndex = malloc(sizeof(unsigned int) * Terrain->numIndex);
            
            //  Allocate texture coord
            Terrain->Texture = malloc(sizeof(struct textureData) * Terrain->numVertex);
            
            //Close file
            fclose(tmpFile);
            
            //	Res value
            Result = 1;
        }
        
    }
    
    return Result;
}

////////////////////////////////////////////////////////////
/// Load heightMap BMP
////////////////////////////////////////////////////////////
int HeightMap_Load_BMP(struct baseTerrain * Terrain, int terraintID)
{
    int Result = 0;
    long imgWidth, imgHeight, imgSize, fileSize;
    
    FILE * tmpFile;
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;
    
    //  Open file
    if ((tmpFile = fopen("heightMap.bmp", "rb")))
    {
        //	Get fileSize
        fseek(tmpFile, 0, SEEK_END);
        fileSize = ftell(tmpFile);
        fseek(tmpFile, 0, SEEK_SET);
        
        //  Read bitMap header
        fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, tmpFile);
    
        //  Read info header
        fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, tmpFile);
    
        //  Terrain size
        imgWidth = bitmapInfoHeader.biWidth;
        imgHeight = bitmapInfoHeader.biHeight;
    
        //  Check dimension
        if (imgWidth != imgHeight)
        {
            return Result;
        }
    
        //  Image size
        imgSize = imgWidth * imgHeight * 3;
    
        //  Set terrain height
        Terrain->mapSize = (int) imgWidth;
        
        //	Calculate numVertex
        Terrain->numVertex = Terrain->mapSize * Terrain->mapSize;
    
        //  Calculate numIndex
        Terrain->numIndex  = (Terrain->mapSize - 1) * (Terrain->mapSize - 1) * 6;

        //HARDCODE---->
        indexCount = Terrain->numIndex;
        //HARDCODE---->
        
        //  Allocate vertex data
        Terrain->Vertex  = malloc(sizeof(struct vertexData) * Terrain->numVertex);
    
        //  Allocate index
        Terrain->hIndex  = malloc(sizeof(unsigned int) * Terrain->numIndex);
    
        //  Allocate texture coord
        Terrain->Texture = malloc(sizeof(struct textureData) * Terrain->numVertex);
    
        //  Allocate buffer
        Terrain->vHeight = malloc(sizeof(unsigned char) * imgSize);
    
        // Move to the beginning of the bitmap data.
        fseek(tmpFile, bitmapFileHeader.bfOffBits, SEEK_SET);
    
        // Read in the bitmap image data.
        fread((char *)Terrain->vHeight, 1, imgSize, tmpFile);
    
        //Close file
        fclose(tmpFile);
        
        //  Load OK
        Result = 1;
    }
    
    return Result;
}

////////////////////////////////////////////////////////////
/// Normalize heightMap
////////////////////////////////////////////////////////////
void Normal_Height(struct baseTerrain * Terrain)
{
    int i, j;
    int Index;
    
    if (Terrain)
    {
        for (i = 0; i < Terrain->mapSize; ++i)
        {
            for (j = 0; j < Terrain->mapSize; ++j)
            {
                Index = i * Terrain->mapSize + j;
//                Terrain->vHeight[Index] /= 15.0f;
            }
        }
    }
}

////////////////////////////////////////////////////////////
/// Create heightMap vertex
////////////////////////////////////////////////////////////
void hMap_Vertex(struct baseTerrain * Terrain)
{
    int i, j;
    int Index = 0;
    
    if (Terrain)
    {
        for (i = 0; i < Terrain->mapSize; ++i)
        {
            for (j = 0; j < Terrain->mapSize; ++j)
            {
                Index = i * Terrain->mapSize + j;
                
                Terrain->Vertex[Index].X = j;
                #if HEIGHT_MAP
                    Terrain->Vertex[Index].Y = (float)Terrain->vHeight[Index];
                #else
                    Terrain->Vertex[Index].Y = 1;
                #endif
                Terrain->Vertex[Index].Z = i;
                
                //  Texture coords
                Terrain->Texture[Index].tU = (float) j / (Terrain->mapSize - 1);
                Terrain->Texture[Index].tV = (float) i / (Terrain->mapSize - 1);
            }
        }
    }
}

////////////////////////////////////////////////////////////
/// Create heightMap index
////////////////////////////////////////////////////////////
void hMap_Index(struct baseTerrain * Terrain)
{
    int i, j;
    int Index = 0;
    
    //	Check terrain
    if (Terrain)
    {
        for (i = 0; i < Terrain->mapSize - 1; ++i)
        {
            for (j = 0; j < Terrain->mapSize - 1; ++j)
            {
                //	First triangle
                Terrain->hIndex[Index] = i * Terrain->mapSize + j;
                Terrain->hIndex[Index + 1] = i * Terrain->mapSize + (j + 1);
                Terrain->hIndex[Index + 2] = (i + 1) * Terrain->mapSize + j;
                
                //  Second triangle
                Terrain->hIndex[Index + 3] = Terrain->hIndex[Index + 2];
                Terrain->hIndex[Index + 4] = Terrain->hIndex[Index + 1];
                Terrain->hIndex[Index + 5] = (i + 1) * Terrain->mapSize + (j + 1);
                Index += 6;
            }
        }
    }
}

////////////////////////////////////////////////////////////
/// Initialize terrain
////////////////////////////////////////////////////////////
int Initialize_Terrain()
{
    //  New instance
    struct baseTerrain * Terrain = TerrainCreate();
    
#if HEIGHT_MAP_RAW
    //  Load a heightMap
    if (!HeightMap_Load(Terrain, 1))
    {
        return 0;
    }
#else
    //  Load a heightMap
    if (!HeightMap_Load_BMP(Terrain, 1))
    {
        return 0;
    }
#endif
    
    //  Create a new texture
    Texture = CreateTexture();
    
    //  Load texture
    if (!TextureLoad(Texture, "Texture1.tga", 0, 1))
    {
        return 0;
    }
    
    //  Normalize heightMap
    Normal_Height(Terrain);
    
    //  Create a vertex
    hMap_Vertex(Terrain);
    
    //  Create a index
    hMap_Index(Terrain);
    
    //  Generate buffers
    Terrain_Buffers(Terrain);

    return 1;
}

////////////////////////////////////////////////////////////
/// Generate a terrain buffers
////////////////////////////////////////////////////////////
void Terrain_Buffers(struct baseTerrain * Terrain)
{
    
    //  OpenGL config
    glClearDepth(1.0f);
    glDisable(GL_FOG);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_BACK, GL_LINE);
    
    
    //  Enable vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //  Create vertex buffer
    glGenVertexArraysAPPLE(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, Terrain->numVertex * sizeof(struct vertexData), Terrain->Vertex, GL_STATIC_DRAW);
    
    //  Vertex format
    
    
    glGenBuffersARB(1, &m_GLTex0Buffer);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex0Buffer );
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, Terrain->numVertex * sizeof(struct textureData), Terrain->Texture, GL_STATIC_DRAW_ARB);
    
    
    //  Create index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Terrain->numIndex * sizeof(unsigned int), Terrain->hIndex, GL_STATIC_DRAW);
}

////////////////////////////////////////////////////////////
/// Terrain render
////////////////////////////////////////////////////////////
void Terrain_Render()
{
//    glActiveTextureARB( GL_TEXTURE0_ARB );
    glMatrixMode( GL_TEXTURE );
    glPushMatrix();
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, (GLuint) Texture->pTexture );
    
    glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, (GLuint) Texture->pTexture );
    glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(0) );
    
    // Render the vertex buffer using the index buffer.
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

}




