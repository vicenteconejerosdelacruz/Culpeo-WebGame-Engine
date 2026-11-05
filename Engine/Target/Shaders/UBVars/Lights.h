// Reemplazo del enum LightTypes usando constantes
const LIGHT_TYPE_AMBIENT: u32 = 0u;
const LIGHT_TYPE_DIRECTIONAL: u32 = 1u;
const LIGHT_TYPE_SPOT: u32 = 2u;
const LIGHT_TYPE_POINT: u32 = 3u;

const MAX_LIGHTS: u32 = 100u;

struct Lights {
    numLights: u32,
	lightType: array<u32,100>,
	shadowMapIndex: array<u32,100>,
    hasShadowMap: array<u32,100>,
    lightColor: array<vec4<f32>,100>,
    atts1: array<vec4<f32>,100>,
    atts2: array<vec4<f32>,100>,
    atts3: array<vec4<f32>,100>,
}

/*
uint numLights;
uint3 _pad3;
  struct LightAttributes {
  uint lightType;
  uint shadowMapIndex;
  uint hasShadowMap;
  float _pad0;
  float4 lightColor;
  float4 atts1;
  float4 atts2;
  float4 atts3;
} atts[100];
*/