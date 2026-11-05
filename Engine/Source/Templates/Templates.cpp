#include "Templates.h"
#include <unordered_map>
#include <set>
#include <yaml-cpp/yaml.h>
#include <Mesh/Mesh.h>
#include <Utils/FileSystem.h>

extern std::atomic<bool> g_EngineReady;

namespace Templates {

    const std::string systemShadersYaml = R"(
- name: Billboard_vs
  path: Billboard.wgsl
  type: VERTEX_SHADER
  uuid: 7d076bac-db2b-4ee3-8e4e-eadf891022fb
- name: Billboard_ps
  path: Billboard.wgsl
  type: PIXEL_SHADER
  uuid: abeeba0f-8f50-4780-92c2-02226cecb5dd
- name: IBLBRDFLUT_cs
  path: CSBRDFLUT.hlsl
  systemCreated: true
  type: COMPUTE_SHADER
  uuid: bb76e846-4015-48a0-ab94-5286dd843052
- name: IBLPrefilteredEnvironmentMap_cs
  path: CSPreFilteredEnvironmentMap.hlsl
  systemCreated: true
  type: COMPUTE_SHADER
  uuid: 6e278619-da6b-48ec-8434-53c3506e7bfd
- name: IBLDiffuseIrradianceMap_cs
  path: CSDiffuseIrradianceMap.hlsl
  systemCreated: true
  type: COMPUTE_SHADER
  uuid: 5ebcccb5-477a-49c9-9878-9ff6453266a0
- name: LuminanceHistogramAverage_cs
  path: CSLuminanceHistogramAverage.hlsl
  systemCreated: true
  type: COMPUTE_SHADER
  uuid: 1d436897-e925-415f-9209-1364005792a0
- name: LuminanceHistogram_cs
  path: CSLuminanceHistogram.hlsl
  systemCreated: true
  type: COMPUTE_SHADER
  uuid: 43b52d31-7040-47e7-80e6-97490550cbae
- name: BoundingBox_cs
  path: CSBoundingBox.hlsl
  systemCreated: true
  type: COMPUTE_SHADER
  uuid: c23ab559-be11-45ad-b598-1e48e5280914
- name: BoundingBox_vs
  path: BoundingBox.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: ae7a35a5-f012-4eb6-bbe1-1f52e6203ccb
- name: BoundingBox_ps
  mappedValues:
    - value: [1.0, 0.0, 0.0]
      variable: baseColor
      variableType: RGB
  path: BoundingBox.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 1bf837a7-1282-4fae-a1ba-9e74e6a99b37
- name: BaseLighting_vs
  path: BaseLighting.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: bc331f48-6a40-4b48-b435-8276051d6993
- name: BaseLighting_ps
  mappedValues:
    - value: [0.11764706671237946, 0.5647059082984924, 1.0]
      variable: baseColor
      variableType: RGB
    - value: 400.0
      variable: specularExponent
      variableType: FLOAT
  path: BaseLighting.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 719c0122-1e9f-46e3-90aa-8f1e5e81c098
- name: Grid_vs
  path: Grid.wgsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 5af4ba59-a09c-41ef-bc1f-13a51fc68439
- name: Grid_ps
  mappedValues:
    - value: [1.0, 0.0, 1.0]
      variable: baseColor
      variableType: RGB
    - value: 1024.0
      variable: specularExponent
      variableType: FLOAT
  path: Grid.wgsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 5929c8f6-e9b7-4680-8447-a430b5accdbf
- name: ShadowMap_vs
  path: ShadowMap.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 0069d1e9-45b0-4fd3-a28f-1f7508503a91
- name: ShadowMap_ps
  path: ShadowMap.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: ed41913d-1a28-40ce-9c92-07549714f367
- name: DepthMinMax_vs
  path: DepthMinMax.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 2ad43d9e-8dec-421c-b8f2-bda3520748bd
- name: DepthMinMax_ps
  path: DepthMinMax.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: dd93a59f-a87e-4d9a-a57c-b91066e7520e
- name: DepthMinMaxToRGBA_vs
  path: DepthMinMaxToRGBA.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 9815152b-84ad-45e5-8b91-0642cfde0543
- name: DepthMinMaxToRGBA_ps
  path: DepthMinMaxToRGBA.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 22c13e3e-5a88-4868-a5cf-bcc65864cf6c
- name: DepthMinMaxToRGBASpot_vs
  path: DepthMinMaxToRGBASpot.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 173a942d-83e2-4d51-83cd-59016cb5be4e
- name: DepthMinMaxToRGBASpot_ps
  path: DepthMinMaxToRGBASpot.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 438f86fd-9ef3-433f-ad7b-c1e60643cd3e
- name: FullScreenQuad_vs
  path: FullScreenQuad.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 8e26fbd4-3a2c-4c04-a628-d2f11d474d60
- name: FullScreenQuad_ps
  path: FullScreenQuad.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 9ab3d65f-be9a-49cc-87f8-bcbf1dafeac7
- name: FullScreenUIQuad_vs
  path: FullScreenUIQuad.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: a44d0097-6e84-433a-82da-0969b8bf31ba
- name: FullScreenUIQuad_ps
  path: FullScreenUIQuad.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 658b3241-1c63-4480-8cfe-28bf34b317f6
- name: ToneMap_vs
  path: ToneMap.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 8ee7a4d0-91f1-4264-aa56-9f82b3c38397
- name: ToneMap_ps
  path: ToneMap.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: 75e834c4-6898-4156-af67-43abba7fc6b5
- name: LoadingBar_vs
  path: LoadingBar.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: d0192f97-a56a-469d-b6f1-07d403ae331a
- name: LoadingBar_ps
  path: LoadingBar.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: b5ef5d53-2174-4d12-b231-5e07a7f5a7f8
- name: Picking_vs
  path: Picking.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 79568541-34c8-4464-bec1-77debde975e0
- name: Picking_ps
  path: Picking.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: e32c5e9c-26a5-4f2b-8d0c-5899c67f1def
- name: Translucent_vs
  path: Translucent.hlsl
  systemCreated: true
  type: VERTEX_SHADER
  uuid: 5231b6af-fc5a-4d77-ba71-5dc40cfd0da5
- name: Translucent_ps
  path: Translucent.hlsl
  systemCreated: true
  type: PIXEL_SHADER
  uuid: bc666a1e-97b4-4b01-979f-af4857e0d4b7
)";

	const std::string systemSoundsYaml = R"([])";

	const std::string systemMaterialsYaml0 = R"(
- name: BoundingBox
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 1bf837a7-1282-4fae-a1ba-9e74e6a99b37
  shader_vs: ae7a35a5-f012-4eb6-bbe1-1f52e6203ccb
  systemCreated: true
  uuid: 2e4d8bf0-0761-45d9-8313-17cdf9b5f8fc
- name: BaseLighting
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  samplers:
    - addressModeU: ADDRESS_MODE_CLAMP
      addressModeV: ADDRESS_MODE_CLAMP
      addressModeW: ADDRESS_MODE_CLAMP
      magFilter: LINEAR
      minFilter: LINEAR
      mipmapFilter: LINEAR
      lodMinClamp: 0.0
      lodMaxClamp: 3.4028234663852886e+38
      compare: UNDEFINED
      maxAnisotropy: 0
  shader_ps: 719c0122-1e9f-46e3-90aa-8f1e5e81c098
  shader_vs: bc331f48-6a40-4b48-b435-8276051d6993
  systemCreated: true
  uuid: 4a5a2cb8-f2ea-4e15-8584-22bb675ae1bc
- name: Floor
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  samplers:
    - addressModeU: ADDRESS_MODE_CLAMP
      addressModeV: ADDRESS_MODE_CLAMP
      addressModeW: ADDRESS_MODE_CLAMP
      magFilter: LINEAR
      minFilter: LINEAR
      mipmapFilter: LINEAR
      lodMinClamp: 0.0
      lodMaxClamp: 3.4028234663852886e+38
      compare: UNDEFINED
      maxAnisotropy: 1
  shader_ps: 5929c8f6-e9b7-4680-8447-a430b5accdbf
  shader_vs: 5af4ba59-a09c-41ef-bc1f-13a51fc68439
  systemCreated: true
  uuid: ecd1688c-73d6-49d0-870f-ca916a417c49
- name: ShadowMap
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  samplers:
    - addressModeU: ADDRESS_MODE_CLAMP
      addressModeV: ADDRESS_MODE_CLAMP
      addressModeW: ADDRESS_MODE_CLAMP
      magFilter: LINEAR
      minFilter: LINEAR
      mipmapFilter: LINEAR
      lodMinClamp: 0.0
      lodMaxClamp: 3.4028234663852886e+38
      compare: UNDEFINED
      maxAnisotropy: 0
  shader_ps: ed41913d-1a28-40ce-9c92-07549714f367
  shader_vs: 0069d1e9-45b0-4fd3-a28f-1f7508503a91
  systemCreated: true
  uuid: 3be1cf4e-cc15-41ae-97e1-6bb3e110271f
)";
	const std::string systemMaterialsYaml1 = R"(
- name: DepthMinMax
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  samplers:
    - addressModeU: ADDRESS_MODE_CLAMP
      addressModeV: ADDRESS_MODE_CLAMP
      addressModeW: ADDRESS_MODE_CLAMP
      magFilter: LINEAR
      minFilter: LINEAR
      mipmapFilter: LINEAR
      lodMinClamp: 0.0
      lodMaxClamp: 3.4028234663852886e+38
      compare: UNDEFINED
      maxAnisotropy: 0
  shader_ps: dd93a59f-a87e-4d9a-a57c-b91066e7520e
  shader_vs: 2ad43d9e-8dec-421c-b8f2-bda3520748bd
  systemCreated: true
  twoSided: true
  uuid: 35da9e7d-1ef8-4165-8e71-36d6cf599c3c
- name: DepthMinMaxToRGBA
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  samplers:
    - addressModeU: ADDRESS_MODE_CLAMP
      addressModeV: ADDRESS_MODE_CLAMP
      addressModeW: ADDRESS_MODE_CLAMP
      magFilter: LINEAR
      minFilter: LINEAR
      mipmapFilter: LINEAR
      lodMinClamp: 0.0
      lodMaxClamp: 3.4028234663852886e+38
      compare: UNDEFINED
      maxAnisotropy: 0
  shader_ps: 22c13e3e-5a88-4868-a5cf-bcc65864cf6c
  shader_vs: 9815152b-84ad-45e5-8b91-0642cfde0543
  systemCreated: true
  twoSided: true
  uuid: 84f0cabb-9b0c-4508-ac6e-d7a84dee696f
- name: DepthMinMaxToRGBASpot
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  samplers:
    - addressModeU: ADDRESS_MODE_CLAMP
      addressModeV: ADDRESS_MODE_CLAMP
      addressModeW: ADDRESS_MODE_CLAMP
      magFilter: LINEAR
      minFilter: LINEAR
      mipmapFilter: LINEAR
      lodMinClamp: 0.0
      lodMaxClamp: 3.4028234663852886e+38
      compare: UNDEFINED
      maxAnisotropy: 0
  shader_ps: 438f86fd-9ef3-433f-ad7b-c1e60643cd3e
  shader_vs: 173a942d-83e2-4d51-83cd-59016cb5be4e
  systemCreated: true
  twoSided: true
  uuid: 908332fb-48b2-42ee-b678-e57fb3ad352e
)";
	const std::string systemMaterialsYaml2 = R"(
- name: FullScreenQuad
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 9ab3d65f-be9a-49cc-87f8-bcbf1dafeac7
  shader_vs: 8e26fbd4-3a2c-4c04-a628-d2f11d474d60
  systemCreated: true
  uuid: 8e98708c-fe2e-4123-b1f0-5b80fabd1888
- blendState:
    AlphaToCoverageEnable: 0
    IndependentBlendEnable: 0
    RenderTarget:
      - BlendEnable: true
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: INV_SRC_ALPHA
        DestBlendAlpha: INV_SRC_ALPHA
        LogicOp: NOOP
        LogicOpEnable: false
        RenderTargetWriteMask: 15
        SrcBlend: SRC_ALPHA
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
  name: FullScreenUIQuad
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 658b3241-1c63-4480-8cfe-28bf34b317f6
  shader_vs: a44d0097-6e84-433a-82da-0969b8bf31ba
  systemCreated: true
  uuid: 94932d78-6316-4a90-8597-2d1a87fdc376
)";
	const std::string systemMaterialsYaml3 = R"(
- name: ToneMap
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 75e834c4-6898-4156-af67-43abba7fc6b5
  shader_vs: 8ee7a4d0-91f1-4264-aa56-9f82b3c38397
  systemCreated: true
  uuid: 8291ba82-165d-464b-be15-d9fa6d7b9a7c
- name: Picking
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: e32c5e9c-26a5-4f2b-8d0c-5899c67f1def
  shader_vs: 79568541-34c8-4464-bec1-77debde975e0
  systemCreated: true
  uuid: 1896d918-4e47-49a6-950b-3135ab020a0b
- mappedValues:
    - value: 0.9803921580314636
      variable: alphaCut
      variableType: FLOAT
    - value: [1.0, 1.0, 1.0]
      variable: baseColor
      variableType: RGB
  name: Camera
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: abeeba0f-8f50-4780-92c2-02226cecb5dd
  shader_vs: 7d076bac-db2b-4ee3-8e4e-eadf891022fb
  systemCreated: true
  textures:
    BaseTexture: 2c207f54-9cdc-4c7e-a70a-60b373f2de79
  uuid: 65d6c9ad-226a-4073-924a-74d0c61acfc6
)";
	const std::string systemMaterialsYaml4 = R"(
- mappedValues:
    - value: 0.9803921580314636
      variable: alphaCut
      variableType: FLOAT
    - value: [1.0, 1.0, 1.0]
      variable: baseColor
      variableType: RGB
  name: LightBulb
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: abeeba0f-8f50-4780-92c2-02226cecb5dd
  shader_vs: 7d076bac-db2b-4ee3-8e4e-eadf891022fb
  systemCreated: true
  textures:
    BaseTexture: fed123fa-e248-47cd-9662-20f73285ad0e
  uuid: 7b774c44-527d-4315-a80c-aacf0a1383a6
- mappedValues:
    - value: 0.9803921580314636
      variable: alphaCut
      variableType: FLOAT
  name: SoundEffect
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: abeeba0f-8f50-4780-92c2-02226cecb5dd
  shader_vs: 7d076bac-db2b-4ee3-8e4e-eadf891022fb
  systemCreated: true
  textures:
    BaseTexture: 5e3cba75-a495-44d8-ba5b-2b888f812a2b
  uuid: e14a13cf-089e-401c-904b-75ebd75984e0
- mappedValues:
    - value: 0.9803921580314636
      variable: alphaCut
      variableType: FLOAT
  name: CameraPicking
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 2dbc1cfd-8bcb-484a-bfc2-ec8be5150e55
  shader_vs: 744b10ef-4f0c-46d5-bd20-e94f7b66b8f9
  systemCreated: true
  textures:
    BaseTexture: 2c207f54-9cdc-4c7e-a70a-60b373f2de79
  uuid: e82b4687-4705-4202-8d96-65096426b00e
- mappedValues:
    - value: 0.9803921580314636
      variable: alphaCut
      variableType: FLOAT
  name: LightBulbPicking
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 2dbc1cfd-8bcb-484a-bfc2-ec8be5150e55
  shader_vs: 744b10ef-4f0c-46d5-bd20-e94f7b66b8f9
  systemCreated: true
  textures:
    BaseTexture: fed123fa-e248-47cd-9662-20f73285ad0e
  uuid: 3786f66e-550a-449d-8526-2507ebec6750
- mappedValues:
    - value: 0.9803921580314636
      variable: alphaCut
      variableType: FLOAT
  name: SoundEffectPicking
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: 2dbc1cfd-8bcb-484a-bfc2-ec8be5150e55
  shader_vs: 744b10ef-4f0c-46d5-bd20-e94f7b66b8f9
  systemCreated: true
  textures:
    BaseTexture: 5e3cba75-a495-44d8-ba5b-2b888f812a2b
  uuid: 44b7750d-534b-4df6-bc43-776054503b4c
)";
	const std::string systemMaterialsYaml5 = R"(
- mappedValues:
    - value: 1.0
      variable: alphaCut
      variableType: FLOAT
  name: TriggerPicking
  overrideDepthStencil: true
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: e32c5e9c-26a5-4f2b-8d0c-5899c67f1def
  shader_vs: 79568541-34c8-4464-bec1-77debde975e0
  systemCreated: true
  textures: []
  uuid: 5d14b58e-44b4-4d71-9d28-c758e055ecf3
- blendState:
    AlphaToCoverageEnable: 0
    IndependentBlendEnable: 0
    RenderTarget:
      - BlendEnable: true
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: INV_SRC_ALPHA
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: false
        RenderTargetWriteMask: 15
        SrcBlend: SRC_ALPHA
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
      - BlendEnable: 0
        BlendOp: ADD
        BlendOpAlpha: ADD
        DestBlend: ZERO
        DestBlendAlpha: ZERO
        LogicOp: NOOP
        LogicOpEnable: 0
        RenderTargetWriteMask: 15
        SrcBlend: ONE
        SrcBlendAlpha: ONE
  depthStencil:
    BackFace:
      StencilDepthFailOp: KEEP
      StencilFailOp: KEEP
      StencilFunc: ALWAYS
      StencilPassOp: KEEP
    DepthEnable: true
    DepthFunc: LESS_EQUAL
    DepthWriteMask: ZERO
    FrontFace:
      StencilDepthFailOp: KEEP
      StencilFailOp: KEEP
      StencilFunc: ALWAYS
      StencilPassOp: KEEP
    StencilEnable: false
    StencilReadMask: 255
    StencilWriteMask: 255
  mappedValues:
    - value: 0.5
      variable: alpha
      variableType: FLOAT
    - value: [0.2, 1.0, 0.78]
      variable: baseColor
      variableType: FLOAT3
  name: Translucent
  overrideDepthStencil: true
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: BACK
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: false
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: bc666a1e-97b4-4b01-979f-af4857e0d4b7
  shader_vs: 5231b6af-fc5a-4d77-ba71-5dc40cfd0da5
  systemCreated: true
  textures: []
  uuid: e241b072-3aea-4c22-afee-b3887732ea89
)";
	const std::string systemMaterialsYaml6 = R"(
- depthStencil:
    BackFace:
      StencilDepthFailOp: KEEP
      StencilFailOp: KEEP
      StencilFunc: ALWAYS
      StencilPassOp: KEEP
    DepthEnable: true
    DepthFunc: LESS
    DepthWriteMask: ALL
    FrontFace:
      StencilDepthFailOp: KEEP
      StencilFailOp: KEEP
      StencilFunc: ALWAYS
      StencilPassOp: KEEP
    StencilEnable: false
    StencilReadMask: 255
    StencilWriteMask: 255
  mappedValues:
    - value: 1.0
      variable: alpha
      variableType: FLOAT
    - value: [0.26, 1.3, 1.014]
      variable: baseColor
      variableType: FLOAT3
  name: Translucent_wired
  overrideDepthStencil: true
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: BACK
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: false
    FillMode: WIREFRAME
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: bc666a1e-97b4-4b01-979f-af4857e0d4b7
  shader_vs: 5231b6af-fc5a-4d77-ba71-5dc40cfd0da5
  systemCreated: true
  textures: []
  uuid: 1d7630c4-86b0-49eb-88f5-40bacb02a652
- name: TranslucentPicking
  overrideDepthStencil: true
  rasterizerState:
    AntialiasedLineEnable: false
    ConservativeRaster: CONSERVATIVE_RASTERIZATION_MODE_OFF
    CullMode: NONE
    DepthBias: 0
    DepthBiasClamp: 0.0
    DepthClipEnable: true
    FillMode: SOLID
    ForcedSampleCount: 0
    FrontCounterClockwise: false
    MultisampleEnable: false
    SlopeScaledDepthBias: 0.0
  shader_ps: e32c5e9c-26a5-4f2b-8d0c-5899c67f1def
  shader_vs: 79568541-34c8-4464-bec1-77debde975e0
  systemCreated: true
  uuid: c6396dfa-16ba-4c07-adfc-daa9c3866eb3
)";

	const std::string systemRenderPassesYaml = R"(
- depthStencilFormat: D32_FLOAT
  fitWindow: true
  materialOverride: None
  name: mainPass
  renderCallbackOverride: None
  renderTargetFormats:
    - R32G32B32A32_FLOAT
  systemCreated: true
  type: RenderToTexturePass
  uuid: b4ee2cf4-0231-4ff1-ada8-ae745cf0709e

- depthStencilFormat: UNKNOWN
  fitWindow: true
  materialOverride: None
  name: toneMappingPass
  renderCallbackOverride: ToneMapping
  renderTargetFormats:
    - R8G8B8A8_UNORM
  systemCreated: true
  type: RenderToTexturePass
  uuid: 722d8147-6483-4675-94a3-1e8af09ec5e1

- depthStencilFormat: UNKNOWN
  fitWindow: true
  materialOverride: None
  name: resolvePass
  renderCallbackOverride: Resolve
  renderTargetFormats:
    - B8GRB8A8_UNORM
  systemCreated: true
  type: SwapChainPass
  uuid: 53693830-779c-4ed0-a985-402d6a72485b

- depthStencilFormat: UNKNOWN
  fitWindow: true
  materialOverride: None
  name: resolveUIPass
  renderCallbackOverride: ResolveUI
  renderTargetFormats:
    - R8G8B8A8_UNORM
  systemCreated: true
  type: SwapChainPass
  usePrevPassTexture: false
  uuid: 90fc8b65-23ff-4823-880d-50868186100a

- depthStencilFormat: D32_FLOAT
  fitWindow: true
  materialOverride: None
  name: simplePass
  renderCallbackOverride: None
  renderTargetFormats:
    - B8G8R8A8_UNORM
  systemCreated: true
  type: SwapChainPass
  uuid: c483d4c9-94ce-48d6-8116-ea838e69119b

- depthStencilFormat: D32_FLOAT
  fitWindow: false
  materialOverride: ShadowMap
  name: ShadowMap
  renderCallbackOverride: None
  renderTargetFormats: []
  systemCreated: true
  type: RenderToTexturePass
  uuid: 241cbc97-c047-4334-9393-ae5d33268220

- depthStencilFormat: D32_FLOAT
  fitWindow: false
  materialOverride: Picking
  name: PickingPass
  renderCallbackOverride: None
  renderTargetFormats:
    - R32_UINT
  systemCreated: true
  type: RenderToTexturePass
  uuid: d607f54c-11cf-461e-9a3e-0a74a84feb2f

- depthStencilFormat: UNKNOWN
  fitWindow: false
  materialOverride: None
  name: ShadowMapMinMaxChainPass
  renderCallbackOverride: MinMaxChain
  renderTargetFormats:
    - R32_FLOAT
    - R32_FLOAT
  systemCreated: true
  type: RenderToTexturePass
  uuid: fcd248e2-55b3-42f8-ab98-8b65d6fec86e

- depthStencilFormat: UNKNOWN
  fitWindow: false
  materialOverride: None
  name: ShadowMapMinMaxChainResultPass
  renderCallbackOverride: MinMaxChainResult
  renderTargetFormats:
    - R8G8B8A8_UNORM
  systemCreated: true
  type: RenderToTexturePass
  uuid: 6b1bc75a-956f-4673-b35a-a8bc820f5153

- depthStencilFormat: D32_FLOAT
  fitWindow: false
  materialOverride: None
  name: ModelPreviewPass
  renderCallbackOverride: None
  renderTargetFormats:
    - R8G8B8A8_UNORM
  systemCreated: true
  type: RenderToTexturePass
  uuid: 7b06958d-2897-4110-9095-c9b541070eaa

- depthStencilFormat: UNKNOWN
  fitWindow: true
  materialOverride: None
  name: simpleUI
  renderCallbackOverride: None
  renderTargetFormats:
    - R8G8B8A8_UNORM
  systemCreated: true
  type: RenderToTexturePass
  uuid: d5a1867a-a480-48a0-b1aa-606cee1e087d
)";

	const std::string systemTexturesYaml = R"(
- format: B8G8R8A8_UNORM_SRGB
  height: 256
  images:
    - Assets/gizmos/light-bulb.png
  mipLevels: 8
  name: Assets/gizmos/light-bulb.png
  numFrames: 1
  systemCreated: true
  type: 2D
  uuid: fed123fa-e248-47cd-9662-20f73285ad0e
  width: 256

- format: B8G8R8A8_UNORM_SRGB
  height: 64
  images:
    - Assets/gizmos/soundspeaker.png
  mipLevels: 6
  name: Assets/gizmos/soundspeaker.png
  numFrames: 1
  systemCreated: true
  type: 2D
  uuid: 5e3cba75-a495-44d8-ba5b-2b888f812a2b
  width: 64

- format: B8G8R8A8_UNORM_SRGB
  height: 64
  images:
    - Assets/gizmos/camera.png
  mipLevels: 6
  name: Assets/gizmos/camera.png
  numFrames: 1
  systemCreated: true
  type: 2D
  uuid: 2c207f54-9cdc-4c7e-a70a-60b373f2de79
  width: 64

- format: B8G8R8A8_UNORM_SRGB
  height: 64
  images:
    - Assets/gizmos/camera.png
  mipLevels: 6
  name: Assets/gizmos/camera.png
  numFrames: 1
  systemCreated: true
  type: 2D
  uuid: 2c207f54-9cdc-4c7e-a70a-60b373f2de79
  width: 64

)";

	const std::string systemPhysicGeometriesYaml = R"(
- mesh: d41e5c29-49bb-4f2c-aa2b-da781fbac512
  model: ""
  name: floor
  systemCreated: true
  uuid: 330c6bfd-2c71-4c6b-be42-b797d07ab5ba

- mesh: f7786ac1-e296-4e9a-a7e6-6f1949de75ef
  model: ""
  name: cube
  systemCreated: true
  uuid: a5d06057-5344-413c-9647-ed2aac874021

- mesh: d76b3bd8-0f53-4128-974e-2d6d5062bc00
  model: ""
  name: pyramid
  systemCreated: true
  uuid: 36c75d6d-970f-43dc-97bd-9bc13a035a3a

- mesh: 4d1174b2-8225-4c09-9db6-ff09718ae0f5
  model: ""
  name: sphere
  systemCreated: true
  uuid: a96cbfaf-0827-4b04-ae68-548c9bffaf87

- mesh: ad73990a-c59d-45d2-8ec3-807b1f52f5b9
  model: ""
  name: cone
  systemCreated: true
  uuid: 8a41c342-e388-4e1f-8ba9-8b0a41b1c975

- mesh: c900056b-9f67-47d1-a252-71e0ef1f9a65
  model: ""
  name: capsule
  systemCreated: true
  uuid: 95eee3bb-5b2f-41d7-b43a-11ab5524cca1
)";


	std::unordered_map<TemplateType, std::set<JUUID>> templates;
	std::unordered_map<JUUID, TemplateType> templatesTypes;
	std::set<JUUID>& GetTemplates(TemplateType type)
	{
		if (!templates.contains(type))
			templates[type].clear();
		return templates.at(type);
	}
	std::unordered_map<JUUID, TemplateType>& GetTemplatesTypes()
	{
		return templatesTypes;
	}
	TemplateType GetTemplateType(JUUID uuid)
	{
		return templatesTypes.at(uuid);
	}
	bool TemplateExists(JUUID uuid)
	{
		return templatesTypes.contains(uuid);
	}

	void CreateSystemTemplates()
	{
		LoadTemplates(systemShadersYaml, CreateShader);
		LoadTemplates(systemSoundsYaml, CreateSound);
		LoadTemplates(systemMaterialsYaml0, CreateMaterial);
		LoadTemplates(systemMaterialsYaml1, CreateMaterial);
		LoadTemplates(systemMaterialsYaml2, CreateMaterial);
		LoadTemplates(systemMaterialsYaml3, CreateMaterial);
		LoadTemplates(systemMaterialsYaml4, CreateMaterial);
		LoadTemplates(systemMaterialsYaml5, CreateMaterial);
		LoadTemplates(systemMaterialsYaml6, CreateMaterial);
		LoadTemplates(systemRenderPassesYaml, CreateRenderPass);
		LoadTemplates(systemTexturesYaml, CreateTexture);
		// LoadTemplates(systemPhysicGeometriesYaml, CreatePhysicGeometry);

		CreatePrimitiveMeshTemplate("d41e5c29-49bb-4f2c-aa2b-da781fbac512", "floor");
		CreatePrimitiveMeshTemplate("d8bfdef4-55f9-4f6e-b4a8-20915eb854d6", "utahteapot");
		CreatePrimitiveMeshTemplate("f7786ac1-e296-4e9a-a7e6-6f1949de75ef", "cube");
		CreatePrimitiveMeshTemplate("d76b3bd8-0f53-4128-974e-2d6d5062bc00", "pyramid");
		CreatePrimitiveMeshTemplate("7dec1229-075f-4599-95e1-9ccfad0d48b1", "decal");
		CreatePrimitiveMeshTemplate("30f15e68-db42-46fa-b846-b2647a0ac9b9", "boxlines");
		CreatePrimitiveMeshTemplate("4d1174b2-8225-4c09-9db6-ff09718ae0f5", "sphere");
		CreatePrimitiveMeshTemplate("ad73990a-c59d-45d2-8ec3-807b1f52f5b9", "cone");
		CreatePrimitiveMeshTemplate("c900056b-9f67-47d1-a252-71e0ef1f9a65", "capsule");
	}

  void CreateTemplates()
  {
      // Lanzamos un hilo en segundo plano usando std::thread (se convierte en un Web Worker)
      std::thread([]() {
        try {
          // std::cout << "[Templates] Iniciando carga de templates..." << std::endl;
          LoadTemplates(defaultTemplatesFolder, Shader::templateName, CreateShader);
          LoadTemplates(defaultTemplatesFolder, Material::templateName, CreateMaterial);
          // LoadTemplates(defaultTemplatesFolder, Model3D::templateName, CreateModel3D);
          // LoadTemplates(defaultTemplatesFolder, Sound::templateName, CreateSound);
          LoadTemplates(defaultTemplatesFolder, Texture::templateName, CreateTexture);
          LoadTemplates(defaultTemplatesFolder, RenderPass::templateName, CreateRenderPass);
          // // LoadTemplates(defaultTemplatesFolder, PhysicGeometry::templateName, CreatePhysicGeometry);
          LoadTemplates(defaultTemplatesFolder, HtmlUI::templateName, CreateHtmlUI);
          LoadTemplates(defaultTemplatesFolder, Mold::templateName, CreateMold);
          
          g_EngineReady.store(true, std::memory_order_release);
          std::cout << "[Templates] carga completa..." << std::endl;
        } catch (const std::exception& e) {
          std::cerr << "[CRITICAL] Excepción en hilo de templates: " << e.what() << std::endl;
        } catch (...) {
          std::cerr << "[CRITICAL] Excepción desconocida en hilo de templates." << std::endl;
        }
      }).detach(); // .detach() permite que el hilo corra libremente en background
  }

	void LoadTemplates(nlohmann::json templates, std::function<void(nlohmann::json&)> loader)
	{
		for (unsigned int i = 0; i < templates.size(); i++)
		{
			loader(templates.at(i));
		}
	}

	void LoadTemplates(const std::string yaml, std::function<void(nlohmann::json&)> loader)
	{
		YAML::Node templates_yaml = YAML::Load(yaml);

		nlohmann::json templates = yaml_to_json(templates_yaml);
		for (unsigned int i = 0; i < templates.size(); i++)
		{
			loader(templates.at(i));
		}
	}

	void LoadTemplates(const std::string folder, const std::string fileName, std::function<void(nlohmann::json&)> loader)
	{
		//first create the directory if needed
		std::filesystem::path directory(folder);
		const std::string finalFilename = folder + fileName;
		std::filesystem::path path(finalFilename);
		std::string pathStr = path.generic_string();
    AssetStream assetStream(pathStr);

    auto& file = assetStream.get();
		YAML::Node templates_yaml = YAML::Load(file);
		nlohmann::json data = yaml_to_json(templates_yaml);

		for (unsigned int i = 0; i < data.size(); i++)
		{
			loader(data.at(i));
		}
	}
	
	void DestroyTemplates()
	{
		ReleaseRenderPassTemplates();
		ReleaseTextureTemplates();
		//ReleaseSoundTemplates();
		//ReleaseModel3DTemplates();
		ReleaseMaterialTemplates();
		ReleaseShaderTemplates();
		//ReleasePhysicGeometryTemplates();
		ReleaseHtmlUITemplates();
		ReleaseMoldTemplates();
	}
} // namespace Templates