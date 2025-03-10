//
// Created by SeanR on 1/10/2024.
//

#ifndef THREE_CPP_PROPERTY_BINDING_H
#define THREE_CPP_PROPERTY_BINDING_H

#include "object_3d.h"

#include <string>
#include <regex>
#include <functional>
#include <any>

#include "common_utils.h"

#include "animation_object_group.h"

class Composite {

    Composite( AnimationObjectGroup& targetGroup, std::string path, std::string optionalParsedPath ) {

//        const parsedPath = optionalParsedPath || PropertyBinding.parseTrackName( path );
//
//        this._targetGroup = targetGroup;
//        this._bindings = targetGroup.subscribe_( path, parsedPath );

    }

    void getValue( std::vector<float> array, int offset ) {

        bind(); // bind all binding

//        const firstValidIndex = this._targetGroup.nCachedObjects_,
//                binding = this._bindings[ firstValidIndex ];
//
//        // and only call .getValue on the first
//        if ( binding !== undefined ) binding.getValue( array, offset );

    }

    void setValue( std::vector<float> array, int offset ) {
//
//        const bindings = this._bindings;
//
//        for ( let i = this._targetGroup.nCachedObjects_, n = bindings.length; i !== n; ++ i ) {
//
//            bindings[ i ].setValue( array, offset );
//
//        }

    }

    void bind() {

//        const bindings = this._bindings;
//
//        for ( let i = this._targetGroup.nCachedObjects_, n = bindings.length; i !== n; ++ i ) {
//
//            bindings[ i ].bind();
//
//        }

    }

    void unbind() {

//        const bindings = this._bindings;
//
//        for ( let i = this._targetGroup.nCachedObjects_, n = bindings.length; i !== n; ++ i ) {
//
//            bindings[ i ].unbind();
//
//        }
//
    }

};

// Note: This class uses a State pattern on a per-method basis:
// 'bind' sets 'this.getValue' / 'setValue' and shadows the
// prototype version of these methods with one that represents
// the bound state. When the property is not found, the methods
// become no-ops.
class PropertyBinding {
private:
    std::any resolvedProperty;


public:
    struct ParsedPath{
        std::string directoryName; //matches[ 1 ], // (tschw) currently unused
        std::string nodeName; //matches[ 2 ],
        std::string objectName; //matches[ 3 ],
        std::string objectIndex; //matches[ 4 ],
        std::string propertyName; //matches[ 5 ], // required
        std::string propertyIndex; //matches[ 6 ]

        ParsedPath(std::string directoryName,std::string nodeName,std::string objectName,
                   std::string objectIndex,std::string propertyName,std::string propertyIndex):
                   directoryName(directoryName),nodeName(nodeName),objectName(objectName),objectIndex(objectIndex),
                   propertyName(propertyName),propertyIndex(propertyIndex){}
    };

    using sptr = std::shared_ptr<PropertyBinding>;

    enum class BindingType:int{
        Direct = 0,
        EntireArray = 1,
        ArrayElement = 2,
        HasFromToArray = 3
    };

    enum class Versioning:int{
        None = 0,
        NeedsUpdate = 1,
        MatrixWorldNeedsUpdate = 2
    };

    PropertyBinding( Object3D::sptr rootNode, std::string path, std::shared_ptr<ParsedPath> parsedPath = nullptr );

    static sptr create(Object3D::sptr rootNode, std::string path, std::shared_ptr<ParsedPath> parsedPath = nullptr ){
        return std::make_shared<PropertyBinding>(rootNode,path,parsedPath);
    }

    std::vector<float> &getValue(std::vector<float> &bufferArray, int offset){
        auto& source = this->resolvedProperty;

        if(source.has_value()){
            if(source.type() == typeid(Quaternion)) {
                auto quat = std::any_cast<Quaternion>(source);
                bufferArray.resize(quat.size());
                for ( size_t i = 0, n = quat.size(); i != n; ++ i ) {
                    bufferArray[ offset ++ ] = quat[ i ];
                }
            }
        }
        return bufferArray;
    }

    void setValue(std::vector<float> bufferArray, int offset){
        auto dest = this->resolvedProperty;

        if(dest.has_value()) {
            if(dest.type() == typeid(Quaternion)) {
                auto quat = std::any_cast<Quaternion>(dest);
                for (size_t i = 0, n = quat.length(); i != n; ++i) {
                    quat[i] = bufferArray[offset++];
                    //quat.setX(bufferArray[offset++]);
                }
            }
        }
    }
    void bind();
    void unbind();

    static Object3D::sptr findNode( Object3D::sptr root, std::string nodeName ) {

        if ( nodeName.empty() || nodeName == "" || nodeName == "." ||  nodeName == root->name /**|| nodeName == root.uuid*/) {
            return root;
        }

        // search into skeleton bones.
//        if ( root.skeleton ) {
//
//            const bone = root.skeleton.getBoneByName( nodeName );
//
//            if ( bone !== undefined ) {
//
//                return bone;
//
//            }
//
//        }

        // search into node subtree.
        if ( root->children.size()>0 ) {

            //std::function<Object3D::sptr(std::vector<Object3D::sptr>)> searchNodeSubtree = [&]( std::vector<Object3D::sptr> children ) -> Object3D::sptr {
//            auto searchNodeSubtree =
//                                [&]( auto&& self,std::vector<Object3D::sptr> children ) -> Object3D::sptr {
//
//                for ( size_t i = 0; i < children.size(); i ++ ) {
//
//                    auto childNode = children[ i ];
//
//                    if ( childNode->name == nodeName /*|| childNode->uuid == nodeName*/ ) {
//                        return childNode;
//                    }
//
//                    auto result = self( self,childNode->children );
//
//                    if ( result ) return result;
//
//                }
//
//                return nullptr;
//
//            };

            auto subTreeNode = searchSubNodeTree(nodeName,root->children );

            if ( subTreeNode ) {
                return subTreeNode;
            }

        }

        return nullptr;

    }

    static std::shared_ptr<ParsedPath> parseTrackName( std::string trackName );


    /**
     * Replaces spaces with underscores and removes unsupported characters from
     * node names, to ensure compatibility with parseTrackName().
     *
     * @param {string} name Node name to be sanitized.
     * @return {string}
     */
    static std::string sanitizeNodeName( std::string name );


    std::shared_ptr<ParsedPath> parsedPath;
private:
    Object3D::sptr rootNode;
    std::string path;

    Object3D::sptr node;

    static auto searchSubNodeTree( std::string nodeName,std::vector<Object3D::sptr> children ) -> Object3D::sptr {

        for ( size_t i = 0; i < children.size(); i ++ ) {

            auto childNode = children[ i ];

            if ( childNode->name == nodeName || childNode->uuid.str() == nodeName ) {
                return childNode;
            }

            auto result = searchSubNodeTree( nodeName,childNode->children );

            if ( result ) return result;

        }

        return nullptr;

    };


};
#endif //THREE_CPP_PROPERTY_BINDING_H
