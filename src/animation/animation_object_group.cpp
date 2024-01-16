//
// Created by SeanR on 2024/1/10.
//

#include "animation_object_group.h"

AnimationObjectGroup::AnimationObjectGroup(std::initializer_list<Object3D::sptr> objects):isAnimationObjectGroup(true),uuid(sole::uuid1()),nCachedObjects_(0){

    this->_indicesByUUID = {}; // for bookkeeping

    this->_objects.insert(this->_objects.end(),objects.begin(),objects.end());

    for ( size_t i = 0, n = this->_objects.size(); i != n; ++ i ) {
        this->_indicesByUUID[ this->_objects[i]->uuid ] = i;
    }

    this->_paths = {}; // inside: string
    this->_parsedPaths = {}; // inside: { we don't care, here }
    this->_bindings = {}; // inside: Array< PropertyBinding >
    this->_bindingsIndicesByPath = {}; // inside: indices in these arrays

    this->stats = std::make_shared<AnimationObjectGroupStat>(this);

}

void AnimationObjectGroup::add() {

    auto objects = this->objects;
    auto indicesByUUID = this->_indicesByUUID;
    auto paths = this->_paths,
            parsedPaths = this->_parsedPaths;
    auto bindings = this->_bindings;
    auto nBindings = bindings.size();

    //let knownObject = undefined,
    auto nObjects = objects.size();
    auto nCachedObjects = this->nCachedObjects_;

    for ( size_t i = 0, n = this->_objects.size(); i != n; ++ i ) {

        auto object = this->_objects[ i ];
        auto uuid = object->uuid;
        auto index = indicesByUUID[ uuid ];

        if ( index == 0 ) {
            // unknown object -> add it to the ACTIVE region
            index = nObjects ++;
            indicesByUUID[ uuid ] = index;
            objects.push_back( object );

            // accounting is done, now do the same for all bindings

            for ( size_t j = 0, m = nBindings; j != m; ++ j ) {
                //todo:fix this
                //bindings[ j ]->push( new PropertyBinding( object, paths[ j ], parsedPaths[ j ] ) );

            }

        }
//        else if ( index < nCachedObjects ) {
//
//            knownObject = objects[ index ];
//
//            // move existing object to the ACTIVE region
//
//            const firstActiveIndex = -- nCachedObjects,
//                    lastCachedObject = objects[ firstActiveIndex ];
//
//            indicesByUUID[ lastCachedObject.uuid ] = index;
//            objects[ index ] = lastCachedObject;
//
//            indicesByUUID[ uuid ] = firstActiveIndex;
//            objects[ firstActiveIndex ] = object;
//
//            // accounting is done, now do the same for all bindings
//
//            for ( let j = 0, m = nBindings; j !== m; ++ j ) {
//
//                const bindingsForPath = bindings[ j ],
//                        lastCached = bindingsForPath[ firstActiveIndex ];
//
//                let binding = bindingsForPath[ index ];
//
//                bindingsForPath[ index ] = lastCached;
//
//                if ( binding === undefined ) {
//
//                    // since we do not bother to create new bindings
//                    // for objects that are cached, the binding may
//                    // or may not exist
//
//                    binding = new PropertyBinding( object, paths[ j ], parsedPaths[ j ] );
//
//                }
//
//                bindingsForPath[ firstActiveIndex ] = binding;
//
//            }
//
//        } else if ( objects[ index ] !== knownObject ) {
//
//            console.error( 'THREE.AnimationObjectGroup: Different objects with the same UUID ' +
//                           'detected. Clean the caches or recreate your infrastructure when reloading scenes.' );
//
//        } // else the object is already where we want it to be

    } // for arguments
//
//    this.nCachedObjects_ = nCachedObjects;

}