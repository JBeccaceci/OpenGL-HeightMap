//
//  Texture.c
//  APPTest
//
//  Created by Juan Beccaceci on 31/5/17.
//  Copyright © 2017 Juan Beccaceci. All rights reserved.
//
////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <OpenGL/gl.h>
#include "Texture.h"

////////////////////////////////////////////////////////////
/// Create texture
////////////////////////////////////////////////////////////
struct TextureData * CreateTexture()
{
    struct TextureData * newTexture = (struct TextureData *) malloc(sizeof(struct TextureData));
    
    newTexture->Loaded    = 0;
    newTexture->TextureID = 0;
    newTexture->TGAHeader = NULL;

    return newTexture;
}

////////////////////////////////////////////////////////////
/// Load texture
////////////////////////////////////////////////////////////
int TextureLoad(struct TextureData * Texture, char * Name, int textureID, int Wrap)
{
    int Result = 0;
    
    if (Texture)
    {
        FILE * TGAFile;
        
        //  Allocate header
        Texture->TGAHeader = malloc(sizeof(struct TargaHeader));
        
        //  Load file
        TGAFile = fopen(Name, "rb");
        if (!TGAFile)
        {
            return Result;
        }
        
        //  Read data
        fread(Texture->TGAHeader, sizeof(struct TargaHeader), 1, TGAFile);
        
        //  Check texture bits
        if (Texture->TGAHeader->Bpp != 32)
        {
            return Result;
        }
        
        //  Calculate size
        Texture->Size = (Texture->TGAHeader->Height * Texture->TGAHeader->Width) * 4;
        
        //  Allocate texture data
        Texture->pTexture = (unsigned char *) malloc(sizeof(unsigned char) * Texture->Size);
        
        //  Read texture data
        fread(Texture->pTexture, 1, Texture->Size, TGAFile);
        
        //  OpenGL Texture
        glActiveTexture(GL_TEXTURE0 + textureID);
        
        //  Generate texture
        glGenTextures(1, &Texture->TextureID);
        
        //  Load the image data into the texture unit.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture->TGAHeader->Width, Texture->TGAHeader->Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Texture->pTexture);
        
        //  Set the texture color to either wrap around or clamp to the edge.
        if(Wrap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        }
        
        //  Set the texture filtering.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        //  Generate mipmaps for the texture.
        glGenerateMipmap(GL_TEXTURE_2D);
        
        //  Load ok
        Texture->Loaded = 1;
        Result = 1;
    }
    
    return Result;
}

////////////////////////////////////////////////////////////
/// Destroy texture
////////////////////////////////////////////////////////////
void TextureDestroy(struct TextureData * Texture)
{
    if (Texture)
    {
        if (Texture->Loaded)
        {
            glDeleteTextures(1, &Texture->TextureID);
            Texture->Loaded = 0;
            
            //  Deallocate memory
            free(Texture);
        }
    }
}
