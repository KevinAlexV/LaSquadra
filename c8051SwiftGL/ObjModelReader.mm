#import "ObjModelReader.h"
#import <Foundation/Foundation.h>

void ObjModelReader::readObjFile(const char *objFileName,
                                 vector<int>& indices,
                                 vector<float>& vertices,
                                 vector<float>& normals,
                                 vector<float>& texCoords) {
    NSString *fileName = [NSString stringWithCString:objFileName encoding:NSASCIIStringEncoding];
    NSString *path = [[NSBundle mainBundle] pathForResource:fileName ofType:@"obj"];
    NSString *fileContent = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    NSArray *lines = [fileContent componentsSeparatedByString:@"\n"];
    
    NSMutableArray *positions = [NSMutableArray new];
    NSMutableArray *texCoords_ = [NSMutableArray new];
    NSMutableArray *normals_ = [NSMutableArray new];
    NSMutableArray *faces = [NSMutableArray new];
    NSMutableDictionary *facesMap = [NSMutableDictionary new];
    NSInteger position = 0;
    for (NSString *line in lines) {
        NSMutableArray *words = [[line componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] mutableCopy];
        [words removeObject:@""];
        
        if (words.count == 4 && [words[0] isEqualToString:@"v"]) {
            float x = [words[1] floatValue];
            float y = [words[2] floatValue];
            float z = [words[3] floatValue];
            NSArray *position = @[
                [NSNumber numberWithFloat:x],
                [NSNumber numberWithFloat:y],
                [NSNumber numberWithFloat:z]
            ];
            [positions addObject:position];
        } else if (words.count >= 3 && [words[0] isEqualToString:@"vt"]) {
            float x = [words[1] floatValue];
            float y = [words[2] floatValue];
            NSArray *texCoord = @[
                [NSNumber numberWithFloat:x],
                [NSNumber numberWithFloat:y]
            ];
            [texCoords_ addObject:texCoord];
        } else if (words.count == 4 && [words[0] isEqualToString:@"vn"]) {
            float x = [words[1] floatValue];
            float y = [words[2] floatValue];
            float z = [words[3] floatValue];
            NSArray *normal = @[
                [NSNumber numberWithFloat:x],
                [NSNumber numberWithFloat:y],
                [NSNumber numberWithFloat:z]
            ];
            [normals_ addObject:normal];
        } else if (words.count == 4 && [words[0] isEqualToString:@"f"]) {
            for (int i = 1; i < 4; i++) {
                [faces addObject:words[i]];
                
                if (facesMap[words[i]] == nil) {
                    facesMap[words[i]] = [NSNumber numberWithInteger:position];
                    position++;
                }
            }
        }
    }
    
    NSArray *faceMapKeys = [facesMap keysSortedByValueUsingSelector:@selector(compare:)];
    for (int i = 0; i < faceMapKeys.count; i++) {
        NSString *key = faceMapKeys[i];
        NSArray *index = [key componentsSeparatedByString:@"/"];
        int v = [index[0] intValue];
        int vt = [index[1] intValue];
        int vn = [index[2] intValue];
        NSArray *position = positions[v - 1];
        NSArray *texCoord = texCoords_[vt - 1];
        NSArray *normal = normals_[vn - 1];
        
        vertices.push_back([position[0] floatValue]);
        vertices.push_back([position[1] floatValue]);
        vertices.push_back([position[2] floatValue]);
        texCoords.push_back([texCoord[0] floatValue]);
        texCoords.push_back([texCoord[1] floatValue]);
        normals.push_back([normal[0] floatValue]);
        normals.push_back([normal[1] floatValue]);
        normals.push_back([normal[2] floatValue]);
    }

    for (int i = 0; i < faces.count; i++) {
        indices.push_back([facesMap[faces[i]] intValue]);
    }
}
