struct Camera {
    view: mat4x4<f32>,
    viewProjection: mat4x4<f32>,
    eyePosition: vec4<f32>,
    eyeForward: vec4<f32>,
    eyeUp: vec4<f32>,
    eyeRight: vec4<f32>,
    widthHeight: vec4<f32>,
    white: f32,
    IBLNumEnvLevels: f32,
    // --- Padding obligatorio para WebGPU ---
    _pad0: f32,
    _pad1: f32,
};