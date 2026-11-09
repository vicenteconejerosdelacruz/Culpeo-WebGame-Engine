#include "UUID.h"
#include "SceneUnitId.h"

DEF_TYPE_ID(DeviceUtils, TUUID, IndexBuffer, GetIndexBuffer);
DEF_TYPE_ID(DeviceUtils, TUUID, UniformBuffer, GetUniformBuffer);
DEF_TYPE_ID(DeviceUtils, TUUID, VertexBuffer, GetVertexBuffer);
DEF_TYPE_ID(DeviceUtils, TUUID, SwapChainPass, GetSwapChainPass);
DEF_TYPE_ID(DeviceUtils, TUUID, RenderToTexturePass, GetRenderToTexturePass);

DEF_TYPE_ID(Templates, TUUID, ShaderJson, GetShaderTemplate);
DEF_TYPE_ID(Templates, TUUID, ShaderInstance, GetShaderInstance);
DEF_TYPE_ID(Templates, TUUID, TextureJson, GetTextureTemplate);
DEF_TYPE_ID(Templates, TUUID, TextureInstance, GetTextureInstance);
DEF_TYPE_ID(Templates, TUUID, MaterialJson, GetMaterialTemplate);
DEF_TYPE_ID(Templates, TUUID, MaterialInstance, GetMaterialInstance);
DEF_TYPE_ID(Templates, TUUID, RenderPassJson, GetRenderPassTemplate);
DEF_TYPE_ID(Templates, TUUID, RenderPassInstance, GetRenderPassInstance);
DEF_TYPE_ID(Templates, TUUID, HtmlUIJson, GetHtmlUITemplate);
DEF_TYPE_ID(Templates, TUUID, HtmlUIInstance, GetHtmlUIInstance);
DEF_TYPE_ID(Templates, TUUID, MoldJson, GetMoldTemplate);
DEF_TYPE_ID(Templates, TUUID, MeshInstance, GetMeshInstance);
DEF_TYPE_ID(Templates, TUUID, SoundJson, GetSoundTemplate);
DEF_TYPE_ID(Templates, TUUID, SoundInstance, GetSoundInstance);
DEF_TYPE_ID(Templates, TUUID, Model3DJson, GetModel3DTemplate);
DEF_TYPE_ID(Templates, TUUID, Model3DInstance, GetModel3DInstance);

DEF_TYPE_CID(Scene, CUUID, Camera, GetCamera);
DEF_TYPE_CID(Scene, CUUID, Renderable, GetRenderable);
DEF_TYPE_CID(Scene, CUUID, Light, GetLight);
DEF_TYPE_CID(Scene, CUUID, SoundFX, GetSoundFX);

DEF_TYPE_SID(Scene, TSUUUID, SceneObject, GetSceneObject)