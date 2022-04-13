#include "Drawable.hpp"

Drawable::Drawable(int textureListIndex, int vertNum, int normNum, int texNum, int indexNum)
: GameObject(), /*vertices(0), normals(0), texCoords(0), indices(0),*/ numIndices(indexNum), numVertices(vertNum), numNormals(normNum), numTexCoords(texNum)
{
    this->textureListIndex = textureListIndex;
    /*if(vertNum > 0)
        vertices = new float[vertNum * 3];
    if(normNum > 0)
        normals = new float[normNum * 3];
    if(texNum > 0)
        texCoords = new float[texNum * 2];
    if(indexNum > 0)
        indices = new int[indexNum];*/
}

Drawable::Drawable(const Drawable& obj)
: GameObject(), /*vertices(0), normals(0), texCoords(0), indices(0),*/ numIndices(obj.numIndices), numVertices(obj.numVertices), numNormals(obj.numNormals), numTexCoords(obj.numTexCoords)
{
    this->textureListIndex = obj.textureListIndex;
    if(obj.vertices.size() != 0)
    {
        for(int i = 0; i < obj.vertices.size(); i++)
            vertices.push_back(obj.vertices[i]);
    }
    if(obj.normals.size() != 0)
    {
        for(int i = 0; i < obj.normals.size(); i++)
            normals.push_back(obj.normals[i]);
    }
    if(obj.texCoords.size() != 0)
    {
        for(int i = 0; i < obj.texCoords.size(); i++)
            texCoords.push_back(obj.texCoords[i]);
    }
    if(obj.indices.size() != 0)
    {
        for(int i = 0; i < obj.indices.size(); i++)
            indices.push_back(obj.indices[i]);
    }
}

Drawable::~Drawable()
{
    /*if(vertices)
        delete vertices;
    if(normals)
        delete normals;
    if(texCoords)
        delete texCoords;*/
    vertices.clear();
    normals.clear();
    texCoords.clear();
}

void Drawable::assignAnimator(Animator* anim){
    this->anim = anim;
    this->anim->assignTransform(localTransform);
}

void Drawable::updateTransform(){
    if(anim != NULL)
        anim->update();
    
    transformMatrix = globalTransform->getMatrix() * localTransform->getMatrix();
}

void Drawable::changeTexture(int textID)
{
    textureListIndex = textID;
}

glm::mat4 Drawable::draw(glm::mat4 mvp){
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof ( GLfloat ), vertices.data() );
    glEnableVertexAttribArray ( 0 );

    glVertexAttrib4f ( 1, 1.0f, 0.0f, 0.0f, 1.0f );

    glVertexAttribPointer ( 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof ( GLfloat ), normals.data() );
    glEnableVertexAttribArray ( 2 );

    glVertexAttribPointer ( 3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof ( GLfloat ), texCoords.data() );
    glEnableVertexAttribArray ( 3 );
    
    glm::mat4 transform;
    
    //object's properties
    transform = mvp * transformMatrix;
    
    return transform;
}

int Drawable::getNumVertices() { return vertices.size(); }
int Drawable::getNumNormals() { return normals.size(); }
int Drawable::getNumTexCoords() { return texCoords.size(); }
int Drawable::getNumIndices() { return indices.size(); }
int* Drawable::getIndices() { return indices.data(); }
float* Drawable::getVertices() { return vertices.data(); }
float* Drawable::getNormals() { return normals.data(); }
float* Drawable::getTextureCoords() { return texCoords.data(); }
int Drawable::getTextureListIndex() { return textureListIndex; }
