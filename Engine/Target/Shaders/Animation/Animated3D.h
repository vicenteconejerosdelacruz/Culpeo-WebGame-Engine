struct Animated3D {
    // Puedes mantener el tamaño fijo o dejarlo sin tamaño si usas buffers dinámicos
    boneMatrices: array<mat4x4<f32>, 1024>,
};