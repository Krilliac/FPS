#pragma once
struct ModelVertex {
    float Position[3];
    float Normal[3];
    float TexCoord[2];
    ModelVertex() : Position{0,0,0}, Normal{0,0,0}, TexCoord{0,0} {}
};
