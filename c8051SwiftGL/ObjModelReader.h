#ifndef ObjModelReader_h
#define ObjModelReader_h

#include <stdio.h>
#include <vector>

using namespace std;

class ObjModelReader {
public:
    static void readObjFile(const char *objFileName,
                            vector<int>& indices,
                            vector<float>& vertices,
                            vector<float>& normals,
                            vector<float>& texCoords);
};

#endif /* ObjModelReader_h */
