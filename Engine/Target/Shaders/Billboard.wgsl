#include "Utils/Gamma.h"
#include "UBVars/Camera.h"
#include "UBVars/Model.h"
#include "UBVars/Material.h"

// ------------------------------------------
// GRUPO 0: Frecuencia por FRAME (Globales)
// ------------------------------------------
@group(0) @binding(0) var<uniform> camera: Camera;

// ------------------------------------------
// GRUPO 1: Per Model
// ------------------------------------------
@group(1) @binding(0) var<uniform> model: Model;

// ------------------------------------------
// GRUPO 2: Per Material
// ------------------------------------------
@group(2) @binding(0) var<uniform> material: Material;
@group(2) @binding(1) var samp0: sampler;
@group(2) @binding(2) var baseTexture: texture_2d<f32>;

// ==========================================
// ESTRUCTURAS DE ENTRADA / SALIDA DE VÉRTICES
// ==========================================
struct VertexShaderInput {
    @location(0) pos: vec3<f32>,
    @location(1) uv: vec2<f32>,
};

struct PixelShaderInput {
    @builtin(position) pos: vec4<f32>,
    @location(0) uv: vec2<f32>,
};

// ==========================================
// VERTEX SHADER
// ==========================================
@vertex
fn main_vs(input: VertexShaderInput) -> PixelShaderInput {
    var output: PixelShaderInput;

	let worldMat = model.world;
    // En HLSL con matrices row-major, world[3][0..2] es la traslación (fila 3).
    // En WGSL/Dawn (column-major), la traslación vive en la columna 3: worldMat[3].xyz
    let center = vec4<f32>(worldMat[3].xyz, 1.0);

    // En HLSL: mul(center, camera.viewProjection) [vector fila * matriz].
    // En WGSL se invierte a: matriz * vector columna.
    var pos = camera.viewProjection * center;

    // Cálculo del offset del billboard escalado a la resolución de pantalla
    let scale = vec2<f32>(
        60.0 / camera.widthHeight.x,
        60.0 / camera.widthHeight.y
    );
    let offset = input.pos.xy * scale;

    // Aplicar el offset en espacio de clip (multiplicado por w para mantener el tamaño constante en perspectiva)
    pos.x += offset.x * pos.w;
    pos.y += offset.y * pos.w;

    output.pos = pos;
    output.uv = input.uv;

    return output;
}

// ==========================================
// FRAGMENT SHADER
// ==========================================
@fragment
fn main_ps(input: PixelShaderInput) -> @location(0) vec4<f32> {
	// 1. Usar 'var' y especificar el tipo para permitir modificaciones
    var texColor: vec4<f32> = textureSample(baseTexture, samp0, input.uv);
    
    // 2. Multiplicar por el color base del material
    texColor = vec4<f32>(texColor.rgb * material.baseColor, texColor.a);
    
    // 3. Descartar píxeles si están por debajo del umbral de alpha
    if (texColor.a < material.alphaCut) {
        discard;
    }
    
    // 4. Retornar el color final manteniendo el alfa original (o 1.0f si es totalmente opaco)
    return texColor;
}