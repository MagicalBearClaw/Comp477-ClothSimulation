#pragma once
#include "../stdafx.h"
#include "Mesh.h"

class Plane : public Mesh
{

public: 
    Plane(float length, float width, int lengthSegments, int widthSegments, std::string textureFileName);

protected:
    virtual void CreateVertexBuffer() override;
    virtual void CreateIndexBuffer() override;
private:
     float width;
     float length;
     int widthSegments;
     int lengthSegments;
     float gridWidth;
     float gridHeight;
};