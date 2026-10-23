struct Material {
    baseColor: vec3<f32>,         // 12 bytes \__ Suman exactamente 16 bytes
    alphaCut: f32,                // 4 bytes  /   (Bloque 4)
    colorKey: vec3<f32>,          // 12 bytes \__ Suman exactamente 16 bytes
    specularExponent: f32,        // 4 bytes  /   (Bloque 5)
    metallicFactor: f32,          // 4 bytes  \
    roughnessFactor: f32,         // 4 bytes   | Suman exactamente 16 bytes
    frameIndex: u32,              // 4 bytes   |   (Bloque 6)
    luminanceCutOff: f32,         // 4 bytes  /
};