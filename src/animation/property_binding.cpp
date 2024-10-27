//
// Created by SeanR on 1/10/2024.
//

#include "property_binding.h"

#include <algorithm>

#include "common_utils.h"

//namespace property_binding___{
//    // Characters [].:/ are reserved for track binding syntax.
//    std::string _RESERVED_CHARS_RE = "(\\[\\]\\.:\\/)";
//    std::regex _reservedRe("[" + _RESERVED_CHARS_RE + "]",std::regex_constants::ECMAScript);
//
//
//    // Attempts to allow node names from any language. ES5's `\w` regexp matches
//    // only latin characters, and the unicode \p{L} is not yet supported. So
//    // instead, we exclude reserved characters and match everything else.
//    const std::string _wordChar = "[^" + _RESERVED_CHARS_RE + "]";
//    //const std::string _wordCharOrDot = "[^" + _RESERVED_CHARS_RE.replace( "\\.", "" ) + "]";
//    const std::string _wordCharOrDot = "[^" + threecpp::replaceWithStr(_RESERVED_CHARS_RE,"\\.", "" ) + "]";
//
//    // Parent directories, delimited by '/' or ':'. Currently unused, but must
//    // be matched to parse the rest of the track name.
//    const std::string _directoryRe =  threecpp::replaceWithStr(R""(((?:WC+[\/:])*))"","WC", _wordChar );
//
//    // Target node. May contain word characters (a-zA-Z0-9_) and '.' or '-'.
//    //const std::string _nodeRe = /*@__PURE__*/ /(WCOD+)?/.source.replace( 'WCOD', _wordCharOrDot );
//    const std::string _nodeRe = threecpp::replaceWithStr(R""((WCOD+)?)"","WCOD", _wordCharOrDot );
//
//    // Object on target node, and accessor. May not contain reserved
//    // characters. Accessor may contain any character except closing bracket.
//    //const _objectRe = /*@__PURE__*/ /(?:\.(WC+)(?:\[(.+)\])?)?/.source.replace( 'WC', _wordChar );
//    const std::string _objectRe = threecpp::replaceWithStr(R""((?:\.(WC+)(?:\[(.+)\])?)?)"","WC", _wordChar );
//
//    // Property and accessor. May not contain reserved characters. Accessor may
//    // contain any non-bracket characters.
//    //const _propertyRe = /*@__PURE__*/ /\.(WC+)(?:\[(.+)\])?/.source.replace( 'WC', _wordChar );
//    const std::string _propertyRe = threecpp::replaceWithStr(R""(\.(WC+)(?:\[(.+)\])?)"","WC", _wordChar );
//
//    const std::regex _trackRe( "^"
//                               + _directoryRe
//                               + _nodeRe
//                               + _objectRe
//                               + _propertyRe
//                               + "$"
//    );
//
//    const std::vector<std::string> _supportedObjectNames = { "material", "materials", "bones", "map" };
//}

namespace property_binding__{
    // Characters [].:/ are reserved for track binding syntax.
    const std::string _RESERVED_CHARS = R"(\[\]\.:\/)";
    const std::regex _reservedRe{"[" + _RESERVED_CHARS + "]"};
    //const _reservedRe = new RegExp( '[' + _RESERVED_CHARS_RE + ']', 'g' );

    // Attempts to allow node names from any language. ES5's `\w` regexp matches
    // only latin characters, and the unicode \p{L} is not yet supported. So
    // instead, we exclude reserved characters and match everything else.
    const std::string _wordChar{"[^" + _RESERVED_CHARS + "]"};
    //_RESERVED_CHARS_RE.replace( '\\.', '' )
    const std::string _wordCharOrDot{"[^" + std::regex_replace(_RESERVED_CHARS, std::regex("\\."), "") + "]"};

    // Parent directories, delimited by '/' or ':'. Currently unused, but must
    // be matched to parse the rest of the track name.
    const std::string _direcotryReChar = R"(((?:WC+[\/:])*))";
    const std::regex _directoryRe = std::regex{std::regex_replace(_direcotryReChar, std::regex{"WC"}, _wordChar)};

    // Target node. May contain word characters (a-zA-Z0-9_) and '.' or '-'.
    //const _nodeRe = /*@__PURE__*/ /(WCOD+)?/.source.replace( 'WCOD', _wordCharOrDot );
    const std::string _nodeReChar = R"((WCOD+)?)";
    const std::regex _nodeRe = std::regex{std::regex_replace(_nodeReChar,std::regex{"WCOD"}, _wordCharOrDot)};

    // Object on target node, and accessor. May not contain reserved
    // characters. Accessor may contain any character except closing bracket.
    //const _objectRe = /*@__PURE__*/ /(?:\.(WC+)(?:\[(.+)\])?)?/.source.replace( 'WC', _wordChar );
    const std::string _objectReChar = R"((?:\.(WC+)(?:\[(.+)\])?)?)";
    const std::regex _objectRe = std::regex{std::regex_replace(_objectReChar,std::regex{"WC"}, _wordChar)};

    // Property and accessor. May not contain reserved characters. Accessor may
    // contain any non-bracket characters.
    //const _propertyRe = /*@__PURE__*/ /\.(WC+)(?:\[(.+)\])?/.source.replace( 'WC', _wordChar );
    const std::string _propertyReChar = R"(\.(WC+)(?:\[(.+)\])?)";
    const std::regex _propertyRe = std::regex{std::regex_replace(_propertyReChar,std::regex{"WC"}, _wordChar)};

    //const std::string _trackReStr = std::string("^((?:[^\\[\\]\\.:\\/]+[\\/:])*)([^\\[\\]:\\/]+)?(?:\\.([^\\[\\]\\.:\\/]+)(?:\\[(.+)\\])?)?\\.([^\\[\\]\\.:\\/]+)(?:\\[(.+)\\])?$");
    const std::string _trackReStr = std::string("^")
                                           + std::regex_replace(_direcotryReChar, std::regex{"WC"}, _wordChar)
                                           + std::regex_replace(_nodeReChar,std::regex{"WCOD"}, _wordCharOrDot)
                                           + std::regex_replace(_objectReChar,std::regex{"WC"}, _wordChar)
                                           + std::regex_replace(_propertyReChar,std::regex{"WC"}, _wordChar)
                                           + "$";
    const std::regex _trackRe = std::regex{_trackReStr};

//    const std::regex _trackRe = std::regex{std::string("")
//                                           + "^"
//                                           + std::regex_replace(_direcotryReChar, std::regex{"WC"}, _wordChar)
//                                           + std::regex_replace(_wordChar,std::regex{"WCOD"}, _wordCharOrDot)
//                                           + std::regex_replace(_objectReChar,std::regex{"WC"}, _wordChar)
//                                           + std::regex_replace(_propertyReChar,std::regex{"WC"}, _wordChar)
//                                           + "$" };

    std::vector<std::string> _supportedObjectNames = { "material", "materials", "bones", "map" };
}

using namespace property_binding__;

PropertyBinding::PropertyBinding( Object3D::sptr rootNode, std::string path, std::shared_ptr<ParsedPath> parsedPath ):rootNode(rootNode),path(path),parsedPath(parsedPath){
    if(!parsedPath)
        this->parsedPath = parseTrackName(path);

    auto foundNode = findNode(rootNode,parsedPath?parsedPath->nodeName:this->parsedPath->nodeName);
    this->node = foundNode?foundNode:rootNode;

}

std::string PropertyBinding::sanitizeNodeName( std::string name ) {
    std::regex regExpression{R"(\s)"};
    return regex_replace(std::regex_replace(name, regExpression, "_",std::regex_constants::match_any),_reservedRe,"");

}

std::shared_ptr<PropertyBinding::ParsedPath> PropertyBinding::parseTrackName( std::string trackName ) {
    std::shared_ptr<ParsedPath> results;

    std::smatch mObj;

    std::cout << "_trackRe = " << _trackReStr << std::endl;
    bool result = std::regex_search(trackName,mObj,_trackRe);

    if ( !result ) {
//        std::cerr << "PropertyBinding: Cannot parse trackName:" << trackName << std::endl;
        throw "PropertyBinding: Cannot parse trackName:" + trackName;
        return results;
    }

    results = std::make_shared<ParsedPath>("",mObj[2].str(),mObj[3].str(),
                                           mObj[4].str(),mObj[5].str(),mObj[6].str());
//    const results = {
//            // directoryName: matches[ 1 ], // (tschw) currently unused
//            nodeName: matches[ 2 ],
//            objectName: matches[ 3 ],
//            objectIndex: matches[ 4 ],
//            propertyName: matches[ 5 ], // required
//            propertyIndex: matches[ 6 ]
//    };

    auto lastDot = results->nodeName.find_last_of( '.' );

    if ( lastDot != std::string::npos ) {

        auto objectName = results->nodeName.substr( lastDot + 1 );

        // Object names must be checked against an allowlist. Otherwise, there
        // is no way to parse 'foo.bar.baz': 'baz' must be a property, but
        // 'bar' could be the objectName, or part of a nodeName (which can
        // include '.' characters).
        if ( std::find(_supportedObjectNames.begin(),_supportedObjectNames.end(), objectName ) != _supportedObjectNames.end() ) {

            results->nodeName = results->nodeName.substr( 0, lastDot );
            results->objectName = objectName;
        }

    }

    if ( results->propertyName.empty() || results->propertyName.size() == 0 ) {
        std::cerr <<  "PropertyBinding: can not parse propertyName from trackName: " + trackName << std::endl;
    }

    return results;

}