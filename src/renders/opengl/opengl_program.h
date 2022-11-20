//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_OPENGL_PROGRAM_H
#define THREE_CPP_OPENGL_PROGRAM_H

class OpenGLProgramParameters{

};

//const parameters = {
//
//        isWebGL2: isWebGL2,
//
//        shaderID: shaderID,
//        shaderName: material.type,
//
//        vertexShader: vertexShader,
//        fragmentShader: fragmentShader,
//        defines: material.defines,
//
//        customVertexShaderID: customVertexShaderID,
//        customFragmentShaderID: customFragmentShaderID,
//
//        isRawShaderMaterial: material.isRawShaderMaterial === true,
//        glslVersion: material.glslVersion,
//
//        precision: precision,
//
//        instancing: object.isInstancedMesh === true,
//        instancingColor: object.isInstancedMesh === true && object.instanceColor !== null,
//
//        supportsVertexTextures: vertexTextures,
//        outputEncoding: ( currentRenderTarget === null ) ? renderer.outputEncoding : ( currentRenderTarget.isXRRenderTarget === true ? currentRenderTarget.texture.encoding : LinearEncoding ),
//        map: !! material.map,
//        matcap: !! material.matcap,
//        envMap: !! envMap,
//        envMapMode: envMap && envMap.mapping,
//        envMapCubeUVHeight: envMapCubeUVHeight,
//        lightMap: !! material.lightMap,
//        aoMap: !! material.aoMap,
//        emissiveMap: !! material.emissiveMap,
//        bumpMap: !! material.bumpMap,
//        normalMap: !! material.normalMap,
//        objectSpaceNormalMap: material.normalMapType === ObjectSpaceNormalMap,
//        tangentSpaceNormalMap: material.normalMapType === TangentSpaceNormalMap,
//
//        decodeVideoTexture: !! material.map && ( material.map.isVideoTexture === true ) && ( material.map.encoding === sRGBEncoding ),
//
//        clearcoat: useClearcoat,
//        clearcoatMap: useClearcoat && !! material.clearcoatMap,
//        clearcoatRoughnessMap: useClearcoat && !! material.clearcoatRoughnessMap,
//        clearcoatNormalMap: useClearcoat && !! material.clearcoatNormalMap,
//
//        iridescence: useIridescence,
//        iridescenceMap: useIridescence && !! material.iridescenceMap,
//        iridescenceThicknessMap: useIridescence && !! material.iridescenceThicknessMap,
//
//        displacementMap: !! material.displacementMap,
//        roughnessMap: !! material.roughnessMap,
//        metalnessMap: !! material.metalnessMap,
//        specularMap: !! material.specularMap,
//        specularIntensityMap: !! material.specularIntensityMap,
//        specularColorMap: !! material.specularColorMap,
//
//        opaque: material.transparent === false && material.blending === NormalBlending,
//
//        alphaMap: !! material.alphaMap,
//        alphaTest: useAlphaTest,
//
//        gradientMap: !! material.gradientMap,
//
//        sheen: material.sheen > 0,
//        sheenColorMap: !! material.sheenColorMap,
//        sheenRoughnessMap: !! material.sheenRoughnessMap,
//
//        transmission: material.transmission > 0,
//        transmissionMap: !! material.transmissionMap,
//        thicknessMap: !! material.thicknessMap,
//
//        combine: material.combine,
//
//        vertexTangents: ( !! material.normalMap && !! geometry.attributes.tangent ),
//        vertexColors: material.vertexColors,
//        vertexAlphas: material.vertexColors === true && !! geometry.attributes.color && geometry.attributes.color.itemSize === 4,
//        vertexUvs: !! material.map || !! material.bumpMap || !! material.normalMap || !! material.specularMap || !! material.alphaMap || !! material.emissiveMap || !! material.roughnessMap || !! material.metalnessMap || !! material.clearcoatMap || !! material.clearcoatRoughnessMap || !! material.clearcoatNormalMap || !! material.iridescenceMap || !! material.iridescenceThicknessMap || !! material.displacementMap || !! material.transmissionMap || !! material.thicknessMap || !! material.specularIntensityMap || !! material.specularColorMap || !! material.sheenColorMap || !! material.sheenRoughnessMap,
//        uvsVertexOnly: ! ( !! material.map || !! material.bumpMap || !! material.normalMap || !! material.specularMap || !! material.alphaMap || !! material.emissiveMap || !! material.roughnessMap || !! material.metalnessMap || !! material.clearcoatNormalMap || !! material.iridescenceMap || !! material.iridescenceThicknessMap || material.transmission > 0 || !! material.transmissionMap || !! material.thicknessMap || !! material.specularIntensityMap || !! material.specularColorMap || material.sheen > 0 || !! material.sheenColorMap || !! material.sheenRoughnessMap ) && !! material.displacementMap,
//
//        fog: !! fog,
//        useFog: material.fog === true,
//        fogExp2: ( fog && fog.isFogExp2 ),
//
//        flatShading: !! material.flatShading,
//
//        sizeAttenuation: material.sizeAttenuation,
//        logarithmicDepthBuffer: logarithmicDepthBuffer,
//
//        skinning: object.isSkinnedMesh === true,
//
//        morphTargets: geometry.morphAttributes.position !== undefined,
//        morphNormals: geometry.morphAttributes.normal !== undefined,
//        morphColors: geometry.morphAttributes.color !== undefined,
//        morphTargetsCount: morphTargetsCount,
//        morphTextureStride: morphTextureStride,
//
//        numDirLights: lights.directional.length,
//        numPointLights: lights.point.length,
//        numSpotLights: lights.spot.length,
//        numRectAreaLights: lights.rectArea.length,
//        numHemiLights: lights.hemi.length,
//
//        numDirLightShadows: lights.directionalShadowMap.length,
//        numPointLightShadows: lights.pointShadowMap.length,
//        numSpotLightShadows: lights.spotShadowMap.length,
//
//        numClippingPlanes: clipping.numPlanes,
//        numClipIntersection: clipping.numIntersection,
//
//        dithering: material.dithering,
//
//        shadowMapEnabled: renderer.shadowMap.enabled && shadows.length > 0,
//        shadowMapType: renderer.shadowMap.type,
//
//        toneMapping: material.toneMapped ? renderer.toneMapping : NoToneMapping,
//        physicallyCorrectLights: renderer.physicallyCorrectLights,
//
//        premultipliedAlpha: material.premultipliedAlpha,
//
//        doubleSided: material.side === DoubleSide,
//        flipSided: material.side === BackSide,
//
//        useDepthPacking: !! material.depthPacking,
//        depthPacking: material.depthPacking || 0,
//
//        index0AttributeName: material.index0AttributeName,
//
//        extensionDerivatives: material.extensions && material.extensions.derivatives,
//        extensionFragDepth: material.extensions && material.extensions.fragDepth,
//        extensionDrawBuffers: material.extensions && material.extensions.drawBuffers,
//        extensionShaderTextureLOD: material.extensions && material.extensions.shaderTextureLOD,
//
//        rendererExtensionFragDepth: isWebGL2 || extensions.has( 'EXT_frag_depth' ),
//        rendererExtensionDrawBuffers: isWebGL2 || extensions.has( 'WEBGL_draw_buffers' ),
//        rendererExtensionShaderTextureLod: isWebGL2 || extensions.has( 'EXT_shader_texture_lod' ),
//
//        customProgramCacheKey: material.customProgramCacheKey()
//
//};
//
//return parameters;
//
//}

#endif //THREE_CPP_OPENGL_PROGRAM_H
