// Funciones de cálculo de atenuación
fn DistanceAttenuation(distance: f32, attenuation: vec3<f32>) -> f32 {
    return dot(vec3<f32>(1.0, distance, distance * distance), attenuation);
}

// --- PLACEHOLDERS DE SOMBRAS (Se rellenarán más adelante) ---
fn ComputeShadowDirectional(light: LightAttributes, worldPos: vec3<f32>) -> f32 {
    // TODO: Implementar lógica de cascadas de sombras direccionales
    return 1.0;
}

fn ComputeShadowSpot(light: LightAttributes, worldPos: vec3<f32>) -> f32 {
    // TODO: Implementar lógica de sombras para luces spot
    return 1.0;
}

fn ComputeShadowPoint(light: LightAttributes, worldPos: vec3<f32>) -> f32 {
    // TODO: Implementar lógica de sombras omnidireccionales (cubemap) para point lights
    return 1.0;
}

// --- 1. AMBIENT ---
fn ComputeAmbient(light: LightAttributes, outDiffuse: ptr<function, vec3<f32>>, outSpecular: ptr<function, vec3<f32>>) {
    *outDiffuse = light.lightColor.xyz;
    *outSpecular = vec3<f32>(0.0);
}

// --- 2. DIRECTIONAL ---
fn ComputeDirectional(light: LightAttributes, normal: vec3<f32>, viewDir: vec3<f32>, specularExponent: f32, outDiffuse: ptr<function, vec3<f32>>, outSpecular: ptr<function, vec3<f32>>) {
    let color = light.lightColor.xyz;
    let direction = light.atts1.xyz;

    let L = normalize(-direction);
    let diffuseFactor = saturate(dot(normal, L));

    let HV = normalize(L + viewDir);
    let NoHV = max(dot(normal, HV), 0.0);
    let specularFactor = pow(NoHV, specularExponent);

    *outDiffuse = color * diffuseFactor;
    *outSpecular = color * specularFactor;
}

// --- 3. SPOT ---
fn ComputeSpot(light: LightAttributes, normal: vec3<f32>, viewDir: vec3<f32>, worldPos: vec3<f32>, specularExponent: f32, outDiffuse: ptr<function, vec3<f32>>, outSpecular: ptr<function, vec3<f32>>) {
    let color = light.lightColor.xyz;
    let position = light.atts1.xyz;
    let direction = light.atts2.xyz;
    let cosAngle = light.atts2.w;
    let attenuation = light.atts3.xyz;

    let WtoL = worldPos - position;
    let dist = length(WtoL);
    let nWtoL = normalize(WtoL);
    let L = normalize(direction);
    
    let diffuseFactorVal = dot(nWtoL, L);

    var diffuseFactor = 0.0;
    var specularFactor = 0.0;

    if (diffuseFactorVal > cosAngle) {
        let angleAttenuation = (1.0 - (1.0 - diffuseFactorVal) / (1.0 - cosAngle)) * saturate(dot(normal, -nWtoL));
        diffuseFactor = angleAttenuation / DistanceAttenuation(dist, attenuation);

        let HV = normalize(-nWtoL + viewDir);
        let NoHV = max(dot(normal, HV), 0.0);
        specularFactor = (pow(NoHV, specularExponent)) / DistanceAttenuation(dist, attenuation);
    }

    *outDiffuse = color * diffuseFactor;
    *outSpecular = color * specularFactor;
}

// --- 4. POINT ---
fn ComputePoint(light: LightAttributes, normal: vec3<f32>, viewDir: vec3<f32>, worldPos: vec3<f32>, specularExponent: f32, outDiffuse: ptr<function, vec3<f32>>, outSpecular: ptr<function, vec3<f32>>) {
    let color = light.lightColor.xyz;
    let position = light.atts1.xyz;
    let attenuation = light.atts2.xyz;

    let WtoL = worldPos - position;
    let dist = length(WtoL);
    let nWtoL = normalize(WtoL);

    let distAtten = DistanceAttenuation(dist, attenuation);
    let diffuseFactor = saturate(dot(normal, -nWtoL)) / distAtten;

    let HV = normalize(-nWtoL + viewDir);
    let NoHV = max(dot(normal, HV), 0.0);
    let specularFactor = pow(NoHV, specularExponent);

    *outDiffuse = color * diffuseFactor;
    *outSpecular = color * specularFactor / distAtten;
}

// --- FUNCIÓN PRINCIPAL DE ILUMINACIÓN DE ESCENA ---
fn SceneLighting(
    worldPos: vec3<f32>, 
    normal: vec3<f32>, 
    specularExponent: f32,
    viewDir: vec3<f32>,
	lightsBuffer: Lights,
	diffuseFinalLightContribution: ptr<function, vec3<f32>>,
	specularFinalLightContribution: ptr<function, vec3<f32>>
) {
    var accumDiffuse = vec3<f32>(0.0);
    var accumSpecular = vec3<f32>(0.0);
	
	let lightCount = min(lightsBuffer.numLights, 100u);
	
    for (var i = 0u; i < lightCount; i = i + 1u) {
        let light = lightsBuffer.atts[i];
        
        var diffuseLightContribution = vec3<f32>(0.0);
        var specularLightContribution = vec3<f32>(0.0);
        var shadowMapFactor = 1.0;

        switch (light.lightType) {
            case 0u: { // AMBIENT
                //ComputeAmbient(light, &diffuseLightContribution, &specularLightContribution);
				break;
			}
            case 1u: { // DIRECTIONAL
                ComputeDirectional(light, normal, viewDir, specularExponent, &diffuseLightContribution, &specularLightContribution);
                //if (light.hasShadowMap == 1u) {
                //    shadowMapFactor = ComputeShadowDirectional(light, worldPos);
                //}
				break;
			}
			case 2u: { // SPOT
                ComputeSpot(light, normal, viewDir, worldPos, specularExponent, &diffuseLightContribution, &specularLightContribution);
                //if (light.hasShadowMap == 1u) {
                //    shadowMapFactor = ComputeShadowSpot(light, worldPos);
                //}
				break;
			}
            case 3u: { // POINT
                ComputePoint(light, normal, viewDir, worldPos, specularExponent, &diffuseLightContribution, &specularLightContribution);
                //if (light.hasShadowMap == 1u) {
                //    shadowMapFactor = ComputeShadowPoint(light, worldPos);
                //}
				break;
			}
            default: {
                break;
            }
        }
		
		accumDiffuse += diffuseLightContribution * shadowMapFactor;
        accumSpecular += specularLightContribution * shadowMapFactor;
    }
	
	// Asignamos el resultado final a los punteros de salida
    *diffuseFinalLightContribution = accumDiffuse;
    *specularFinalLightContribution = accumSpecular;
}