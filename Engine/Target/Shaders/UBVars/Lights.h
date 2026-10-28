// Reemplazo del enum LightTypes usando constantes
const LIGHT_TYPE_AMBIENT: u32 = 0u;
const LIGHT_TYPE_DIRECTIONAL: u32 = 1u;
const LIGHT_TYPE_SPOT: u32 = 2u;
const LIGHT_TYPE_POINT: u32 = 3u;

const MAX_LIGHTS: u32 = 100u;

struct LightAttributes {
	lightType: u32,             // 4 bytes
	shadowMapIndex: u32,        // 4 bytes
    hasShadowMap: u32,          // 4 bytes
    _pad0: f32,					// 4 bytes
	
    lightColor: vec4<f32>,      // 16 bytes
    atts1: vec4<f32>,           // 16 bytes
    atts2: vec4<f32>,           // 16 bytes
    atts3: vec4<f32>,           // 16 bytes
}

struct Lights {
    numLights: u32,
    _pad3: vec3<u32>,             // Padding de 12 bytes para alinear el array a múltiplo de 16
    atts: array<LightAttributes, 100>,
}