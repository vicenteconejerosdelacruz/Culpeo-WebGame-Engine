// ==========================================
// ESTRUCTURAS DE DATOS Y UNIFORMS
// ==========================================

struct Camera {
    viewProjection: mat4x4<f32>,
    view: mat4x4<f32>,
    eyePosition: vec4<f32>,
};

struct Model {
    world: mat4x4<f32>,
};

struct Material {
    baseColor: vec4<f32>,
    specularExponent: f32,
};

//// Estructura de luces (Ajustar según tu archivo SceneLighting.h)
//struct Lights {
//    // Definición de tus luces
//};
//
//struct ShadowMaps {
//    // Definición de tus matrices/datos de sombras
//};

// ------------------------------------------
// GRUPO 0: Frecuencia por FRAME (Globales)
// ------------------------------------------
@group(0) @binding(0) var<uniform> camera: Camera;
//@group(0) @binding(1) var<uniform> lights: Lights;

// (Condicional inyectado por tu preprocesador de C++ si _HAS_SHADOWMAPS_TEXTURES está activo)
//#if defined(_HAS_SHADOWMAPS_TEXTURES)
//@group(0) @binding(2) var<uniform> shadowMaps: ShadowMaps;
//#endif

// ------------------------------------------
// GRUPO 1: Frecuencia por MATERIAL / OBJETO (Renderable)
// ------------------------------------------
@group(1) @binding(0) var<uniform> modelUniform: Model;
@group(1) @binding(1) var<uniform> materialUniform: Material;

// ------------------------------------------
// GRUPO 2: Texturas y Samplers (Ej. Shadowmaps)
// ------------------------------------------
//#if defined(_HAS_SHADOWMAPS_TEXTURES)
//// En WGSL las texturas array se declaran así:
//@group(2) @binding(0) var shadowMapsTextures: texture_depth_2d_array; // o texture_2d_array<f32> según corresponda
//@group(2) @binding(1) var shadowSampler: sampler_comparison; // o sampler normal
//#endif


// ==========================================
// ESTRUCTURAS DE ENTRADA / SALIDA DE VÉRTICES
// ==========================================
struct VertexShaderInput {
    @location(0) pos: vec3<f32>,
    @location(1) normal: vec3<f32>,
};

struct PixelShaderInput {
    @builtin(position) pos: vec4<f32>,
    @location(0) normal: vec3<f32>,
    @location(1) viewDirection: vec3<f32>,
    @location(2) worldPos: vec3<f32>,
};


// ==========================================
// VERTEX SHADER
// ==========================================
@vertex
fn main_vs(input: VertexShaderInput) -> PixelShaderInput {
    var output: PixelShaderInput;

    let worldMat = modelUniform.world;
    let pos4 = vec4<f32>(input.pos, 1.0f);

    // Multiplicación de matrices (en WGSL se respeta el orden matemáticamente correcto)
    let wvp = camera.viewProjection * worldMat;
    let pos = wvp * pos4;
    
    // Extracción de la matriz 3x3 para transformar la normal
    let normalMatrix = mat3x3<f32>(
        worldMat[0].xyz,
        worldMat[1].xyz,
        worldMat[2].xyz
    );
    let normal = normalize(normalMatrix * input.normal);
    let worldPos = (worldMat * pos4).xyz;
    let viewDirection = camera.eyePosition.xyz - worldPos;

    output.pos = pos;
    output.normal = normal;
    output.viewDirection = viewDirection;
    output.worldPos = worldPos;
    
    return output;
}


// ==========================================
// FUNCIONES AUXILIARES (Grid y Gamma)
// ==========================================
fn getGridColor(input: PixelShaderInput) -> vec3<f32> {
    var gridColor = vec3<f32>(1.0f);
    let coord = input.worldPos.xz;
    
    // Grilla basada en http://madebyevan.com/shaders/grid/
    let grid1 = abs(fract(coord - 0.5f) - 0.5f) / fwidth(coord);
    let l1 = min(grid1.x, grid1.y);
    let unitColor = 1.0f - min(l1, 1.0f);

    let grid2 = abs(fract((coord - 0.05f) * 10.0f) - 0.5f) / fwidth(coord * 10.0f);
    let l2 = min(grid2.x, grid2.y);
    let subUnitColor = 1.0f - min(l2, 1.0f);

    gridColor -= vec3<f32>(0.5f * unitColor + 0.3f * subUnitColor);
    return gridColor;
}

fn toGammaSpace(color: vec3<f32>) -> vec3<f32> {
    // Equivalente a tu Gamma.h
    return pow(color, vec3<f32>(1.0f / 2.2f));
}


// ==========================================
// FRAGMENT SHADER
// ==========================================
@fragment
fn main_ps(input: PixelShaderInput) -> @location(0) vec4<f32> {
    let worldPos = input.worldPos;
    let normal = normalize(input.normal);
    let viewDir = normalize(input.viewDirection);

    var diffuseFinalLightContribution = vec3<f32>(0.0f);
    var specularFinalLightContribution = vec3<f32>(0.0f);

/*
#if defined(_HAS_SHADOWMAPS_TEXTURES)
    let viewPos = camera.view * vec4<f32>(worldPos, 1.0f);
#endif
*/

    // Llamada a la función de iluminación (equivalente a SceneLighting.h traducido a WGSL)
    /*
    SceneLighting(
        worldPos, normal, materialUniform.specularExponent,
        viewDir,
        lights,
        #if defined(_HAS_SHADOWMAPS_TEXTURES)
        viewPos,
        shadowMaps,
        shadowMapsTextures,
        shadowSampler,
        #endif
        &diffuseFinalLightContribution, &specularFinalLightContribution
    );
    */

    let baseColor = materialUniform.baseColor.rgb;
    var outputColor = baseColor * getGridColor(input) * diffuseFinalLightContribution;
    outputColor += specularFinalLightContribution;
    
    return vec4<f32>(toGammaSpace(outputColor), 1.0f);
}