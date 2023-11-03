#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "mtl_loader.h"
#include "group.h"

#include <string>
#include <vector>
#include <limits>
#include <memory>

class Object {

public:
    class ObjectMaterial {
    public:
        using ptr = std::shared_ptr<ObjectMaterial>;

        int index=-1;

        std::string name="";

        int groupStart = 0;

        int groupEnd = -1;

        int groupCount = -1;

        bool inherited = false;

        bool smooth=false;

        std::vector<ObjectMaterial::ptr> objMtlLib;

        std::string stringLib;

        ObjectMaterial() {}

        ~ObjectMaterial() = default;

        //clone
        ObjectMaterial::ptr clone(int _index);
    };

    class ObjectGeometry {
    public:
        std::vector<float> vertices;

        std::vector<float> normals;

        std::vector<float> colors;

        std::vector<float> uvs;

        std::string type;

        ObjectGeometry() {}

        ~ObjectGeometry() = default;
    };
    using sptr = std::shared_ptr<Object>;

    std::string name = "";

    bool fromDeclaration = true;

    ObjectGeometry geometry;

    std::vector<ObjectMaterial::ptr> materials;

    bool smooth = true;

    Object() {}

    ~Object() = default;

    ObjectMaterial::ptr& currentMaterial();

    ObjectMaterial::ptr startMaterial(const std::string& name,std::vector<std::string>& libraries);

    ObjectMaterial::ptr& finalize(bool end);

};

class ObjectState {

    int parseVertexIndex(int value, size_t len);

    int parseNormalIndex(int value, size_t len);

    int parseUVIndex(int value, size_t len);

    void addVertex(int a, int b, int c);

    void addColor(int a, int b, int c);

    void addUV(int a, int b, int c);

    void addNormal(int a, int b, int c);

public:
    Object::sptr object;

    std::vector<Object::sptr> objects;

    std::vector<float> vertices;

    std::vector<float> normals;

    std::vector<float> colors;

    std::vector<float> uvs;

    std::vector<std::string> materialLibraries;

    ObjectState();

    ~ObjectState() = default;

    void startObject(const std::string& name, bool fromDeclaration);

    void finalize();

    void addVertexPoint(int a);

    void addVertexLine(int a);

    void addUVLine(int a);

    void addPointGeometry(const std::vector<std::string>& _vertices);

    void addPointGeometry(const std::vector<float>& _vertices);

    void addLineGeometry(const std::vector<std::string>& _vertices, const std::vector<std::string>& _uvs = std::vector<std::string>());

    void addLineGeometry(const std::vector<float>& _vertices, std::vector<float> _uvs = std::vector<float>());

    void addFace(int a, int b, int c, int ua, int ub, int uc,int na,int nb,int nc);

    void addFace(int a, int b, int c, int d, int ua, int ub, int uc, int ud, int na, int nb, int nc, int nd);



};

class OBJLoader {
private:

    float parseFloat(std::string value);
    int parseInt(std::string value);
public:

    MTLLoader::MaterialCreator materials;

    OBJLoader() {}

    ~OBJLoader() = default;

    void parseState(ObjectState& state);

    void setMaterials(const MTLLoader::MaterialCreator& materials);

    Group::sptr parse(const std::string& path);

    Group::sptr load(const std::string& path);
};

#endif
