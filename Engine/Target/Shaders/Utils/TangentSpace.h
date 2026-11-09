fn getNormal(n: vec3<f32>, t: vec3<f32>, uv: vec2<f32>, normalMap: texture_2d<f32>, samp: sampler) -> vec3<f32> {
    var tangent = normalize(t);
    tangent = normalize(tangent - n * dot(n, tangent)); // Recalcular tangente
    let b = normalize(cross(tangent, n)); // Bitangente (sistema de mano izquierda)
    
    // En WGSL, mat3x3 se construye con vectores columna. 
    // Al pasar t, b y n como columnas, mat3x3 * nts equivale matemáticamente al mul(nts, ts2ws) de HLSL.
    let ts2ws = mat3x3<f32>(tangent, b, n); 
    
    let nts = textureSample(normalMap, samp, uv).rgb * 2.0 - 1.0; // Normal en espacio tangente
    
    return normalize(ts2ws * nts);
}