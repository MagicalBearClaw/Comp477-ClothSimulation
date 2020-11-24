#include "../stdafx.h"
#include "Plane.h"

Plane::Plane(float length, float width, int lengthSegments, int widthSegments, std::string textureFileName) : Mesh(textureFileName),
			length(length), width(width), lengthSegments(lengthSegments), widthSegments(widthSegments)
{
    gridWidth = (float)length / (float)lengthSegments;
    gridHeight = (float)width / (float)widthSegments;

    numberofVerticesWithDuplicates = (lengthSegments + 1) * (widthSegments + 1);
    numberOfFaces = lengthSegments * widthSegments * 2;
    numberOfVerticies = numberofVerticesWithDuplicates;

    CreateVertexBuffer();
    CreateIndexBuffer();

    triangleNormals.resize(numberOfFaces);
    triangleAreas = new float[numberOfFaces];
    RecalculateNormals();
    Update();
}

void Plane::CreateVertexBuffer()
{
    //vertices = new VertexPositionNormalTexture[(lengthSegments + 1) * (widthSegments + 1)];
    //for (int x = 0; x < lengthSegments + 1; x++)
    //{
    //    for (int y = 0; y < widthSegments + 1; y++)
    //    {
    //        vertices[x + y * (lengthSegments + 1)].Position = new Vector3(x * gridWidth - length / 2, 0, -y * gridHeight + width / 2);
    //        vertices[x + y * (lengthSegments + 1)].TextureCoordinate = new Vector2((float)x / lengthSegments, (float)y / widthSegments);
    //        vertices[x + y * (lengthSegments + 1)].Normal = new Vector3(0, 1, 0);
    //    }
    //}
    //VertexBuffer vb = new VertexBuffer(game.GraphicsDevice, sizeof(float) * 8 * (lengthSegments + 1) * (widthSegments + 1), BufferUsage.WriteOnly);
    //vb.SetData<VertexPositionNormalTexture>(vertices);
    //this.vertexBuffer = vb;

    ////create the vertex mapping:
    ////for plane, there are no dup vertices, so the pseudo vertex id is just the real vertex id
    //for (int i = 0; i < numVertices; i++)
    //{
    //    vertexMappingPseudoToReal.Add(new List<int>{ i });
    //}
}

void Plane::CreateIndexBuffer()
{
    const int numberOfIndices = lengthSegments * widthSegments * 6;
    std::vector<int> indices(numberOfIndices);

    for (int x = 0; x < lengthSegments; x++)
    {
        for (int y = 0; y < widthSegments; y++)
        {
            //specify the indices for the first tri
            indices[(x + y * lengthSegments) * 6] = (x + 1) + (y + 1) * (lengthSegments + 1);
            indices[(x + y * lengthSegments) * 6 + 1] = (x + 1) + y * (lengthSegments + 1);
            indices[(x + y * lengthSegments) * 6 + 2] = x + y * (lengthSegments + 1);

            //store triangle vertex info for future use
            int triangleVertex[3];


            triangleVertex[0] = indices[(x + y * lengthSegments) * 6];
            triangleVertex[1] = indices[(x + y * lengthSegments) * 6 + 1];
            triangleVertex[2] = indices[(x + y * lengthSegments) * 6 + 2];

            //triangleVertexInfo.push_back(a);

            //specify the indices for the second tri
            indices[(x + y * lengthSegments) * 6 + 3] = (x + 1) + (y + 1) * (lengthSegments + 1);
            indices[(x + y * lengthSegments) * 6 + 4] = x + y * (lengthSegments + 1);
            indices[(x + y * lengthSegments) * 6 + 5] = x + (y + 1) * (lengthSegments + 1);

            //store triangleVertex info for future use
            //triangleVertex = new int[3];
            //triangleVertex[0] = indices[(x + y * lengthSegments) * 6 + 3];
            //triangleVertex[1] = indices[(x + y * lengthSegments) * 6 + 4];
            //triangleVertex[2] = indices[(x + y * lengthSegments) * 6 + 5];
            //triangleVertexInfo.Add(triangleVertex);
        }
    }

    //IndexBuffer ib = new IndexBuffer(game.GraphicsDevice, typeof(int), this.lengthSegments * this.widthSegments * 6, BufferUsage.WriteOnly);
    //ib.SetData<int>(indices);
    //this.indexBuffer = ib;
}
