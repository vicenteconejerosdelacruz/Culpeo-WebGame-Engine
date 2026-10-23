struct ShadowMapAttributes {
    atts0: mat4x4<f32>,           // 64 bytes
    atts1: mat4x4<f32>,           // 64 bytes
    atts2: mat4x4<f32>,           // 64 bytes
    atts3: mat4x4<f32>,           // 64 bytes
    atts4: mat4x4<f32>,           // 64 bytes
    atts5: mat4x4<f32>,           // 64 bytes
    atts6: mat4x4<f32>,           // 64 bytes
    atts7: vec4<f32>,             // 16 bytes (Total struct: 464 bytes, múltiplo exacto de 16)
}

struct ShadowMaps {
    atts: array<ShadowMapAttributes, 100>,
}