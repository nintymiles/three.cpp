//
// Created by SeanR on 1/10/2024.
//

#ifndef THREE_CPP_PROPERTY_BINDING_H
#define THREE_CPP_PROPERTY_BINDING_H

// Note: This class uses a State pattern on a per-method basis:
// 'bind' sets 'this.getValue' / 'setValue' and shadows the
// prototype version of these methods with one that represents
// the bound state. When the property is not found, the methods
// become no-ops.
class PropertyBinding {
public:
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



};


#endif //THREE_CPP_PROPERTY_BINDING_H
