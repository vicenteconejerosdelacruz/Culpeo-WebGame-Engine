const GAMMA: f32 = 2.2f;
const INVGAMMA: f32 = 1.0f / GAMMA;

fn toGammaSpace(color: vec3<f32>) -> vec3<f32> {
    return pow(abs(color), vec3<f32>(INVGAMMA));
}

fn fromGammaSpace(color: vec3<f32>) -> vec3<f32> {
    return pow(abs(color), vec3<f32>(GAMMA));
}