#include "Utils/Gamma.h"
#include "Utils/TangentSpace.h"
#include "UBVars/Camera.h"
#include "UBVars/Model.h"
#include "UBVars/Material.h"
#include "UBVars/Lights.h"
#include "UBVars/ShadowMaps.h"
#include "Lighting/BlinnPhong/SceneLighting.h"
#include "Animation/Animated3D.h"

// ------------------------------------------
// GRUPO 0: Frecuencia por FRAME (Globales)
// ------------------------------------------
@group(0) @binding(0) var<uniform> camera: Camera;
@group(0) @binding(1) var<storage, read> lights: Lights;

// ------------------------------------------
// GRUPO 1: Per Model
// ------------------------------------------
@group(1) @binding(0) var<uniform> model: Model; // Nota: accede como model.world
#ifdef _HAS_SKINNING
@group(1) @binding(1) var<storage, read> animation: Animated3D;
#endif

// ------------------------------------------
// GRUPO 2: Per Material
// ------------------------------------------
@group(2) @binding(0) var<uniform> material: Material;
#ifdef _HAS_BASE_TEXTURE
@group(2) @binding(1) var samp0: sampler;
#endif
#ifdef _HAS_BASE_TEXTURE
@group(2) @binding(2) var baseTexture: texture_2d<f32>;
#endif
#ifdef _HAS_NORMALMAP_TEXTURE
@group(2) @binding(3) var normalMap: texture_2d<f32>;
#endif

struct VertexShaderInput
{
    @location(0) pos: vec3<f32>,
#ifdef _HAS_NORMAL
    @location(1) normal: vec3<f32>,
#endif
#ifdef _HAS_TANGENT
    @location(2) tangent: vec3<f32>,
#endif
#ifdef _HAS_TEXCOORD0
    @location(3) uv: vec2<f32>,
#endif
#ifdef _HAS_SKINNING
    @location(4) boneIds: vec4<u32>,
    @location(5) boneWeights: vec4<f32>,
#endif
};

// Se renombró para coincidir con el tipo de retorno de main_vs
struct PixelShaderInput
{
    @builtin(position) pos: vec4<f32>,
#ifdef _HAS_NORMAL
    @location(0) normal: vec3<f32>,
#endif
#ifdef _HAS_TANGENT
    @location(1) tangent: vec3<f32>,
#endif
#ifdef _HAS_TEXCOORD0
    @location(2) uv: vec2<f32>,
#endif
    @location(3) viewDirection: vec3<f32>,
    @location(4) worldPos: vec3<f32>,
};

@vertex
fn main_vs(input: VertexShaderInput) -> PixelShaderInput {
    var output: PixelShaderInput;

    // Usamos model.world en lugar de world (según la definición del binding)
    let wvp = camera.viewProjection * model.world;

#ifdef _HAS_SKINNING
    let b0 = input.boneIds.x;
    let b1 = input.boneIds.y;
    let b2 = input.boneIds.z;
    let b3 = input.boneIds.w;

    let boneTransform = animation.boneMatrices[b0] * input.boneWeights.x +
                        animation.boneMatrices[b1] * input.boneWeights.y +
                        animation.boneMatrices[b2] * input.boneWeights.z +
                        animation.boneMatrices[b3] * input.boneWeights.w;
	

    let boneMat3 = mat3x3<f32>(
        boneTransform[0].xyz,
        boneTransform[1].xyz,
        boneTransform[2].xyz
    );

    let skinnedPos = (boneTransform * vec4<f32>(input.pos, 1.0f)).xyz;
    let skinnedNormal = boneMat3 * input.normal;
    let skinnedTangent = boneMat3 * input.tangent;

    let pos = wvp * vec4<f32>(skinnedPos, 1.0f);
    
    let worldMat3 = mat3x3<f32>(
        model.world[0].xyz,
        model.world[1].xyz,
        model.world[2].xyz
    );

    let normal = worldMat3 * skinnedNormal;
    let tangent = worldMat3 * skinnedTangent;
    let worldPos = (model.world * vec4<f32>(skinnedPos, 1.0f)).xyz;
#else
    let pos = wvp * vec4<f32>(input.pos, 1.0f);
    
    let worldMat3 = mat3x3<f32>(
        model.world[0].xyz,
        model.world[1].xyz,
        model.world[2].xyz
    );

    let normal = worldMat3 * input.normal;
    let tangent = worldMat3 * input.tangent;
    let worldPos = (model.world * vec4<f32>(input.pos, 1.0f)).xyz;
#endif

    let viewDirection = camera.eyePosition.xyz - worldPos;

    output.pos = pos;
#ifdef _HAS_NORMAL
    output.normal = normal;
#endif
#ifdef _HAS_TANGENT
    output.tangent = tangent;
#endif
#ifdef _HAS_TEXCOORD0
    output.uv = input.uv;
#endif
    output.viewDirection = viewDirection;
    output.worldPos = worldPos;

    return output;
}

@fragment
fn main_ps(input: PixelShaderInput) -> @location(0) vec4<f32> {
    let worldPos = input.worldPos;
    
    var normal = vec3<f32>(0.0);
#ifdef _HAS_NORMAL
    normal = normalize(input.normal);
#endif

    var tangent = vec3<f32>(0.0);
#ifdef _HAS_TANGENT
    tangent = normalize(input.tangent);
#endif

    let viewDir = normalize(input.viewDirection);
    
    var uv = vec2<f32>(0.0);
#ifdef _HAS_TEXCOORD0
    uv = input.uv;
#endif

#ifdef _HAS_BASE_TEXTURE
    // Corregido a minúsculas para coincidir con el binding 'baseTexture'
    var texturesColor = textureSample(baseTexture, samp0, uv);
    if (texturesColor.a < material.alphaCut) { 
        discard; 
    }
#ifndef _BASE_TEXTURE_IN_GAMMA_SPACE
    texturesColor = vec4<f32>(fromGammaSpace(texturesColor.rgb), texturesColor.a);
#endif
#else
    let texturesColor = vec4<f32>(1.0, 1.0, 1.0, 1.0);
#endif

    var diffuseFinalLightContribution = vec3<f32>(0.0);
    var specularFinalLightContribution = vec3<f32>(0.0);

#ifdef _HAS_SHADOWMAPS_TEXTURES
    let viewPos = camera.view * vec4<f32>(worldPos, 1.0f);
#endif

#ifdef _HAS_NORMALMAP_TEXTURE
    // Corregido a minúsculas para coincidir con el binding 'normalMap'
    normal = getNormal(normal, tangent, uv, normalMap, samp0);
#endif

    SceneLighting(worldPos, normal, material.specularExponent, viewDir, &diffuseFinalLightContribution, &specularFinalLightContribution);

    var outputColor = texturesColor.rgb * diffuseFinalLightContribution;
    outputColor += specularFinalLightContribution;

#ifndef _BASE_TEXTURE_IN_GAMMA_SPACE
    outputColor = toGammaSpace(outputColor);
#endif

    return vec4<f32>(outputColor, 1.0f);
}