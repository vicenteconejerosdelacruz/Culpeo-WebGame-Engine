#include <webgpu/webgpu_cpp.h>
#include <unordered_map>
#include <string>
#include <set>

// ==========================================
// 1. TEXTURE / RENDER FORMATS
// ==========================================

static inline std::unordered_map<wgpu::TextureFormat, std::string> TextureFormatToString = {
	{ wgpu::TextureFormat::Undefined, "UNDEFINED" },
	{ wgpu::TextureFormat::RGBA32Float, "R32G32B32A32_FLOAT" },
	{ wgpu::TextureFormat::RGBA32Uint, "R32G32B32A32_UINT" },
	{ wgpu::TextureFormat::RGBA32Sint, "R32G32B32A32_SINT" },
	{ wgpu::TextureFormat::RGBA16Float, "R16G16B16A16_FLOAT" },
	{ wgpu::TextureFormat::RGBA16Unorm, "R16G16B16A16_UNORM" },
	{ wgpu::TextureFormat::RGBA16Uint, "R16G16B16A16_UINT" },
	{ wgpu::TextureFormat::RGBA16Snorm, "R16G16B16A16_SNORM" },
	{ wgpu::TextureFormat::RGBA16Sint, "R16G16B16A16_SINT" },
	{ wgpu::TextureFormat::RG32Float, "R32G32_FLOAT" },
	{ wgpu::TextureFormat::RG32Uint, "R32G32_UINT" },
	{ wgpu::TextureFormat::RG32Sint, "R32G32_SINT" },
	{ wgpu::TextureFormat::RGB10A2Unorm, "R10G10B10A2_UNORM" },
	{ wgpu::TextureFormat::RG11B10Ufloat, "R11G11B10_FLOAT" },
	{ wgpu::TextureFormat::RGBA8Unorm, "R8G8B8A8_UNORM" },
	{ wgpu::TextureFormat::RGBA8UnormSrgb, "R8G8B8A8_UNORM_SRGB" },
	{ wgpu::TextureFormat::RGBA8Uint, "R8G8B8A8_UINT" },
	{ wgpu::TextureFormat::RGBA8Snorm, "R8G8B8A8_SNORM" },
	{ wgpu::TextureFormat::RGBA8Sint, "R8G8B8A8_SINT" },
	{ wgpu::TextureFormat::RG16Float, "R16G16_FLOAT" },
	{ wgpu::TextureFormat::RG16Unorm, "R16G16_UNORM" },
	{ wgpu::TextureFormat::RG16Uint, "R16G16_UINT" },
	{ wgpu::TextureFormat::RG16Snorm, "R16G16_SNORM" },
	{ wgpu::TextureFormat::RG16Sint, "R16G16_SINT" },
	{ wgpu::TextureFormat::Depth32Float, "D32_FLOAT" },
	{ wgpu::TextureFormat::R32Float, "R32_FLOAT" },
	{ wgpu::TextureFormat::R32Uint, "R32_UINT" },
	{ wgpu::TextureFormat::R32Sint, "R32_SINT" },
	{ wgpu::TextureFormat::Depth24PlusStencil8, "D24_UNORM_S8_UINT" },
	{ wgpu::TextureFormat::RG8Unorm, "R8G8_UNORM" },
	{ wgpu::TextureFormat::RG8Uint, "R8G8_UINT" },
	{ wgpu::TextureFormat::RG8Snorm, "R8G8_SNORM" },
	{ wgpu::TextureFormat::RG8Sint, "R8G8_SINT" },
	{ wgpu::TextureFormat::R16Float, "R16_FLOAT" },
	{ wgpu::TextureFormat::Depth16Unorm, "D16_UNORM" },
	{ wgpu::TextureFormat::R16Unorm, "R16_UNORM" },
	{ wgpu::TextureFormat::R16Uint, "R16_UINT" },
	{ wgpu::TextureFormat::R16Snorm, "R16_SNORM" },
	{ wgpu::TextureFormat::R16Sint, "R16_SINT" },
	{ wgpu::TextureFormat::R8Unorm, "R8_UNORM" },
	{ wgpu::TextureFormat::R8Uint, "R8_UINT" },
	{ wgpu::TextureFormat::R8Snorm, "R8_SNORM" },
	{ wgpu::TextureFormat::R8Sint, "R8_SINT" },
	{ wgpu::TextureFormat::RGB9E5Ufloat, "R9G9B9E5_SHAREDEXP" },
	{ wgpu::TextureFormat::BC1RGBAUnorm, "BC1_UNORM" },
	{ wgpu::TextureFormat::BC1RGBAUnormSrgb, "BC1_UNORM_SRGB" },
	{ wgpu::TextureFormat::BC2RGBAUnorm, "BC2_UNORM" },
	{ wgpu::TextureFormat::BC2RGBAUnormSrgb, "BC2_UNORM_SRGB" },
	{ wgpu::TextureFormat::BC3RGBAUnorm, "BC3_UNORM" },
	{ wgpu::TextureFormat::BC3RGBAUnormSrgb, "BC3_UNORM_SRGB" },
	{ wgpu::TextureFormat::BC4RUnorm, "BC4_UNORM" },
	{ wgpu::TextureFormat::BC4RSnorm, "BC4_SNORM" },
	{ wgpu::TextureFormat::BC5RGUnorm, "BC5_UNORM" },
	{ wgpu::TextureFormat::BC5RGSnorm, "BC5_SNORM" },
	{ wgpu::TextureFormat::BGRA8Unorm, "B8G8R8A8_UNORM" },
	{ wgpu::TextureFormat::BGRA8UnormSrgb, "B8G8R8A8_UNORM_SRGB" },
	{ wgpu::TextureFormat::BC6HRGBUfloat, "BC6H_UF16" },
	{ wgpu::TextureFormat::BC6HRGBFloat, "BC6H_SF16" },
	{ wgpu::TextureFormat::BC7RGBAUnorm, "BC7_UNORM" },
	{ wgpu::TextureFormat::BC7RGBAUnormSrgb, "BC7_UNORM_SRGB" },
};

static inline std::unordered_map<std::string, wgpu::TextureFormat> StringToTextureFormat = {
	{ "UNDEFINED", wgpu::TextureFormat::Undefined },
	{ "R32G32B32A32_FLOAT", wgpu::TextureFormat::RGBA32Float },
	{ "R32G32B32A32_UINT", wgpu::TextureFormat::RGBA32Uint },
	{ "R32G32B32A32_SINT", wgpu::TextureFormat::RGBA32Sint },
	{ "R16G16B16A16_FLOAT", wgpu::TextureFormat::RGBA16Float },
	{ "R16G16B16A16_UNORM", wgpu::TextureFormat::RGBA16Unorm },
	{ "R16G16B16A16_UINT", wgpu::TextureFormat::RGBA16Uint },
	{ "R16G16B16A16_SNORM", wgpu::TextureFormat::RGBA16Snorm },
	{ "R16G16B16A16_SINT", wgpu::TextureFormat::RGBA16Sint },
	{ "R32G32_FLOAT", wgpu::TextureFormat::RG32Float },
	{ "R32G32_UINT", wgpu::TextureFormat::RG32Uint },
	{ "R32G32_SINT", wgpu::TextureFormat::RG32Sint },
	{ "R10G10B10A2_UNORM", wgpu::TextureFormat::RGB10A2Unorm },
	{ "R11G11B10_FLOAT", wgpu::TextureFormat::RG11B10Ufloat },
	{ "R8G8B8A8_UNORM", wgpu::TextureFormat::RGBA8Unorm },
	{ "R8G8B8A8_UNORM_SRGB", wgpu::TextureFormat::RGBA8UnormSrgb },
	{ "R8G8B8A8_UINT", wgpu::TextureFormat::RGBA8Uint },
	{ "R8G8B8A8_SNORM", wgpu::TextureFormat::RGBA8Snorm },
	{ "R8G8B8A8_SINT", wgpu::TextureFormat::RGBA8Sint },
	{ "R16G16_FLOAT", wgpu::TextureFormat::RG16Float },
	{ "R16G16_UNORM", wgpu::TextureFormat::RG16Unorm },
	{ "R16G16_UINT", wgpu::TextureFormat::RG16Uint },
	{ "R16G16_SNORM", wgpu::TextureFormat::RG16Snorm },
	{ "R16G16_SINT", wgpu::TextureFormat::RG16Sint },
	{ "D32_FLOAT", wgpu::TextureFormat::Depth32Float },
	{ "R32_FLOAT", wgpu::TextureFormat::R32Float },
	{ "R32_UINT", wgpu::TextureFormat::R32Uint },
	{ "R32_SINT", wgpu::TextureFormat::R32Sint },
	{ "D24_UNORM_S8_UINT", wgpu::TextureFormat::Depth24PlusStencil8 },
	{ "R8G8_UNORM", wgpu::TextureFormat::RG8Unorm },
	{ "R8G8_UINT", wgpu::TextureFormat::RG8Uint },
	{ "R8G8_SNORM", wgpu::TextureFormat::RG8Snorm },
	{ "R8G8_SINT", wgpu::TextureFormat::RG8Sint },
	{ "R16_FLOAT", wgpu::TextureFormat::R16Float },
	{ "D16_UNORM", wgpu::TextureFormat::Depth16Unorm },
	{ "R16_UNORM", wgpu::TextureFormat::R16Unorm },
	{ "R16_UINT", wgpu::TextureFormat::R16Uint },
	{ "R16_SNORM", wgpu::TextureFormat::R16Snorm },
	{ "R16_SINT", wgpu::TextureFormat::R16Sint },
	{ "R8_UNORM", wgpu::TextureFormat::R8Unorm },
	{ "R8_UINT", wgpu::TextureFormat::R8Uint },
	{ "R8_SNORM", wgpu::TextureFormat::R8Snorm },
	{ "R8_SINT", wgpu::TextureFormat::R8Sint },
	{ "R9G9B9E5_SHAREDEXP", wgpu::TextureFormat::RGB9E5Ufloat },
	{ "BC1_UNORM", wgpu::TextureFormat::BC1RGBAUnorm },
	{ "BC1_UNORM_SRGB", wgpu::TextureFormat::BC1RGBAUnormSrgb },
	{ "BC2_UNORM", wgpu::TextureFormat::BC2RGBAUnorm },
	{ "BC2_UNORM_SRGB", wgpu::TextureFormat::BC2RGBAUnormSrgb },
	{ "BC3_UNORM", wgpu::TextureFormat::BC3RGBAUnorm },
	{ "BC3_UNORM_SRGB", wgpu::TextureFormat::BC3RGBAUnormSrgb },
	{ "BC4_UNORM", wgpu::TextureFormat::BC4RUnorm },
	{ "BC4_SNORM", wgpu::TextureFormat::BC4RSnorm },
	{ "BC5_UNORM", wgpu::TextureFormat::BC5RGUnorm },
	{ "BC5_SNORM", wgpu::TextureFormat::BC5RGSnorm },
	{ "B8G8R8A8_UNORM", wgpu::TextureFormat::BGRA8Unorm },
	{ "B8G8R8A8_UNORM_SRGB", wgpu::TextureFormat::BGRA8UnormSrgb },
	{ "BC6H_UF16", wgpu::TextureFormat::BC6HRGBUfloat },
	{ "BC6H_SF16", wgpu::TextureFormat::BC6HRGBFloat },
	{ "BC7_UNORM", wgpu::TextureFormat::BC7RGBAUnorm },
	{ "BC7_UNORM_SRGB", wgpu::TextureFormat::BC7RGBAUnormSrgb },
};

static inline std::unordered_map<wgpu::TextureFormat, std::string> WGPU_DEPTH_FORMATToString =
{
	{ wgpu::TextureFormat::Depth32Float, "D32_FLOAT" },
	{ wgpu::TextureFormat::Depth24PlusStencil8, "D24_UNORM_S8_UINT" },
	{ wgpu::TextureFormat::Depth16Unorm, "D16_UNORM" } // Opcional, incluido por si usas de 16-bit
};

static inline std::unordered_map<std::string, wgpu::TextureFormat> StringTo_WGPU_DEPTH_FORMAT =
{
	{ "D32_FLOAT", wgpu::TextureFormat::Depth32Float },
	{ "D24_UNORM_S8_UINT", wgpu::TextureFormat::Depth24PlusStencil8 },
	{ "D16_UNORM", wgpu::TextureFormat::Depth16Unorm }
};

inline std::string FromWebGpuTextureFormat(wgpu::TextureFormat v) { 
	return TextureFormatToString.at(v); 
}

inline wgpu::TextureFormat ToWebGpuTextureFormat(std::string v) { 
	return StringToTextureFormat.at(v); 
}

static inline std::set<wgpu::TextureFormat> NonLinearWgpuFormats =
{
	wgpu::TextureFormat::RGBA8UnormSrgb,
	wgpu::TextureFormat::BC1RGBAUnormSrgb,
	wgpu::TextureFormat::BC2RGBAUnormSrgb,
	wgpu::TextureFormat::BC3RGBAUnormSrgb,
	wgpu::TextureFormat::BGRA8UnormSrgb,
	wgpu::TextureFormat::BC7RGBAUnormSrgb,
};


// ==========================================
// 2. SAMPLER ADDRESS MODE
// ==========================================

static inline std::unordered_map<wgpu::AddressMode, std::string> WGPU_ADDRESS_MODEToString = {
	{ wgpu::AddressMode::Repeat, "ADDRESS_MODE_WRAP" },
	{ wgpu::AddressMode::MirrorRepeat, "ADDRESS_MODE_MIRROR" },
	{ wgpu::AddressMode::ClampToEdge, "ADDRESS_MODE_CLAMP" },
};

static inline std::unordered_map<std::string, wgpu::AddressMode> StringToWGPU_ADDRESS_MODE = {
	{ "ADDRESS_MODE_WRAP", wgpu::AddressMode::Repeat },
	{ "ADDRESS_MODE_MIRROR", wgpu::AddressMode::MirrorRepeat },
	{ "ADDRESS_MODE_CLAMP", wgpu::AddressMode::ClampToEdge },
};

// ==========================================
// 2.1 SAMPLER FILTER MODE
// ==========================================

static inline std::unordered_map<wgpu::FilterMode, std::string> WGPU_FILTER_MODEToString = {
	{ wgpu::FilterMode::Nearest, "NEAREST" },
	{ wgpu::FilterMode::Linear, "LINEAR" },
};

static inline std::unordered_map<std::string, wgpu::FilterMode> StringToWGPU_FILTER_MODE = {

    { "NEAREST", wgpu::FilterMode::Nearest },
    { "LINEAR", wgpu::FilterMode::Linear },
};

// ==========================================
// 2.2 SAMPLER FILTER MODE
// ==========================================

static inline std::unordered_map<wgpu::MipmapFilterMode, std::string> WGPU_MIPMAP_FILTER_MODEToString = {
	{ wgpu::MipmapFilterMode::Nearest, "NEAREST" },
	{ wgpu::MipmapFilterMode::Linear, "LINEAR" },
};

static inline std::unordered_map<std::string, wgpu::MipmapFilterMode> StringToWGPU_MIPMAP_FILTER_MODE = {
    { "NEAREST", wgpu::MipmapFilterMode::Nearest },
    { "LINEAR", wgpu::MipmapFilterMode::Linear },
};

// ==========================================
// 3. COMPARISON FUNCTION
// ==========================================

static inline std::unordered_map<wgpu::CompareFunction, std::string> WGPU_COMPARISON_FUNCToString = {
	{ wgpu::CompareFunction::Never, "NEVER" },
	{ wgpu::CompareFunction::Less, "LESS" },
	{ wgpu::CompareFunction::Equal, "EQUAL" },
	{ wgpu::CompareFunction::LessEqual, "LESS_EQUAL" },
	{ wgpu::CompareFunction::Greater, "GREATER" },
	{ wgpu::CompareFunction::NotEqual, "NOT_EQUAL" },
	{ wgpu::CompareFunction::GreaterEqual, "GREATER_EQUAL" },
	{ wgpu::CompareFunction::Always, "ALWAYS" }
};

static inline std::unordered_map<std::string, wgpu::CompareFunction> StringToWGPU_COMPARISON_FUNC = {
	{ "NEVER", wgpu::CompareFunction::Never },
	{ "LESS", wgpu::CompareFunction::Less },
	{ "EQUAL", wgpu::CompareFunction::Equal },
	{ "LESS_EQUAL", wgpu::CompareFunction::LessEqual },
	{ "GREATER", wgpu::CompareFunction::Greater },
	{ "NOT_EQUAL", wgpu::CompareFunction::NotEqual },
	{ "GREATER_EQUAL", wgpu::CompareFunction::GreaterEqual },
	{ "ALWAYS", wgpu::CompareFunction::Always }
};


// ==========================================
// 4. SHADER STAGE / VISIBILITY
// ==========================================

static inline std::unordered_map<wgpu::ShaderStage, std::string> WGPU_SHADER_STAGEToString = {
	{ wgpu::ShaderStage::Vertex, "VERTEX" },
	{ wgpu::ShaderStage::Fragment, "PIXEL" },
	{ wgpu::ShaderStage::Compute, "COMPUTE" },
};

static inline std::unordered_map<std::string, wgpu::ShaderStage> StringToWGPU_SHADER_STAGE = {
	{ "VERTEX", wgpu::ShaderStage::Vertex },
	{ "PIXEL", wgpu::ShaderStage::Fragment },
	{ "COMPUTE", wgpu::ShaderStage::Compute },
};


// ==========================================
// 5. PRIMITIVE TOPOLOGY
// ==========================================

static inline std::unordered_map<wgpu::PrimitiveTopology, std::string> WGPU_PRIMITIVE_TOPOLOGYToString = {
	{ wgpu::PrimitiveTopology::PointList, "POINTLIST" },
	{ wgpu::PrimitiveTopology::LineList, "LINELIST" },
	{ wgpu::PrimitiveTopology::LineStrip, "LINESTRIP" },
	{ wgpu::PrimitiveTopology::TriangleList, "TRIANGLELIST" },
	{ wgpu::PrimitiveTopology::TriangleStrip, "TRIANGLESTRIP" },
};

static inline std::unordered_map<std::string, wgpu::PrimitiveTopology> StringToWGPU_PRIMITIVE_TOPOLOGY = {
	{ "POINTLIST", wgpu::PrimitiveTopology::PointList },
	{ "LINELIST", wgpu::PrimitiveTopology::LineList },
	{ "LINESTRIP", wgpu::PrimitiveTopology::LineStrip },
	{ "TRIANGLELIST", wgpu::PrimitiveTopology::TriangleList },
	{ "TRIANGLESTRIP", wgpu::PrimitiveTopology::TriangleStrip },
};


// ==========================================
// 6. CULL MODE
// ==========================================

static inline std::unordered_map<wgpu::CullMode, std::string> WGPU_CULL_MODEToString = {
	{ wgpu::CullMode::None, "NONE" },
	{ wgpu::CullMode::Front, "FRONT" },
	{ wgpu::CullMode::Back, "BACK" }
};

static inline std::unordered_map<std::string, wgpu::CullMode> StringToWGPU_CULL_MODE = {
	{ "NONE", wgpu::CullMode::None },
	{ "FRONT", wgpu::CullMode::Front },
	{ "BACK", wgpu::CullMode::Back },
};


// ==========================================
// 7. BLEND FACTORS & BLEND OPERATIONS
// ==========================================

static inline std::unordered_map<wgpu::BlendFactor, std::string> WGPU_BLENDToString =
{
	{ wgpu::BlendFactor::Zero, "ZERO" },
	{ wgpu::BlendFactor::One, "ONE" },
	{ wgpu::BlendFactor::Src, "SRC_COLOR" },
	{ wgpu::BlendFactor::OneMinusSrc, "INV_SRC_COLOR" },
	{ wgpu::BlendFactor::SrcAlpha, "SRC_ALPHA" },
	{ wgpu::BlendFactor::OneMinusSrcAlpha, "INV_SRC_ALPHA" },
	{ wgpu::BlendFactor::Dst, "DEST_COLOR" },
	{ wgpu::BlendFactor::OneMinusDst, "INV_DEST_COLOR" },
	{ wgpu::BlendFactor::DstAlpha, "DEST_ALPHA" },
	{ wgpu::BlendFactor::OneMinusDstAlpha, "INV_DEST_ALPHA" },
	{ wgpu::BlendFactor::SrcAlphaSaturated, "SRC_ALPHA_SAT" },
	{ wgpu::BlendFactor::Constant, "BLEND_FACTOR" },
	{ wgpu::BlendFactor::OneMinusConstant, "INV_BLEND_FACTOR" },
};

static inline std::unordered_map<std::string, wgpu::BlendFactor> StringToWGPU_BLEND =
{
	{ "ZERO", wgpu::BlendFactor::Zero },
	{ "ONE", wgpu::BlendFactor::One },
	{ "SRC_COLOR", wgpu::BlendFactor::Src },
	{ "INV_SRC_COLOR", wgpu::BlendFactor::OneMinusSrc },
	{ "SRC_ALPHA", wgpu::BlendFactor::SrcAlpha },
	{ "INV_SRC_ALPHA", wgpu::BlendFactor::OneMinusSrcAlpha },
	{ "DEST_ALPHA", wgpu::BlendFactor::DstAlpha },
	{ "INV_DEST_ALPHA", wgpu::BlendFactor::OneMinusDstAlpha },
	{ "DEST_COLOR", wgpu::BlendFactor::Dst },
	{ "INV_DEST_COLOR", wgpu::BlendFactor::OneMinusDst },
	{ "SRC_ALPHA_SAT", wgpu::BlendFactor::SrcAlphaSaturated },
	{ "BLEND_FACTOR", wgpu::BlendFactor::Constant },
	{ "INV_BLEND_FACTOR", wgpu::BlendFactor::OneMinusConstant },
};

static inline std::unordered_map<wgpu::BlendOperation, std::string> WGPU_BLEND_OPToString =
{
	{ wgpu::BlendOperation::Add, "ADD" },
	{ wgpu::BlendOperation::Subtract, "SUBTRACT" },
	{ wgpu::BlendOperation::ReverseSubtract, "REV_SUBTRACT" },
	{ wgpu::BlendOperation::Min, "MIN" },
	{ wgpu::BlendOperation::Max, "MAX" },
};

static inline std::unordered_map<std::string, wgpu::BlendOperation> StringToWGPU_BLEND_OP =
{
	{ "ADD", wgpu::BlendOperation::Add },
	{ "SUBTRACT", wgpu::BlendOperation::Subtract },
	{ "REV_SUBTRACT", wgpu::BlendOperation::ReverseSubtract },
	{ "MIN", wgpu::BlendOperation::Min },
	{ "MAX", wgpu::BlendOperation::Max },
};

// ==========================================
// 8. BUFFER BINDING TYPES (Para CBuffers y Structured Buffers)
// ==========================================

static inline std::unordered_map<std::string, wgpu::BufferBindingType> StringToWGPU_BufferBindingType = {
	{ "CBUFFER", wgpu::BufferBindingType::Uniform },
	{ "TBUFFER", wgpu::BufferBindingType::Uniform },
	{ "STRUCTURED", wgpu::BufferBindingType::Storage },
	{ "UAV_RWSTRUCTURED", wgpu::BufferBindingType::Storage },
	{ "BYTEADDRESS", wgpu::BufferBindingType::Storage },
	{ "UAV_RWBYTEADDRESS", wgpu::BufferBindingType::Storage },
	{ "UAV_APPEND_STRUCTURED", wgpu::BufferBindingType::Storage },
	{ "UAV_CONSUME_STRUCTURED", wgpu::BufferBindingType::Storage },
	{ "UAV_RWSTRUCTURED_WITH_COUNTER", wgpu::BufferBindingType::Storage },
};

static inline std::unordered_map<wgpu::BufferBindingType, std::string> WGPU_BufferBindingTypeToString = {
	{ wgpu::BufferBindingType::Uniform, "CBUFFER" },
	{ wgpu::BufferBindingType::Storage, "STRUCTURED" },
	{ wgpu::BufferBindingType::ReadOnlyStorage, "READ_ONLY_STORAGE" },
};


// ==========================================
// 9. SAMPLER BINDING TYPES
// ==========================================

static inline std::unordered_map<std::string, wgpu::SamplerBindingType> StringToWGPU_SamplerBindingType = {
	{ "SAMPLER", wgpu::SamplerBindingType::Filtering },
};

static inline std::unordered_map<wgpu::SamplerBindingType, std::string> WGPU_SamplerBindingTypeToString = {
	{ wgpu::SamplerBindingType::Filtering, "SAMPLER" },
	{ wgpu::SamplerBindingType::Comparison, "COMPARISON_SAMPLER" },
	{ wgpu::SamplerBindingType::NonFiltering, "NON_FILTERING_SAMPLER" },
};


// ==========================================
// 10. DEPTH WRITE ENABLED (Sustituto lógico de Depth Write Mask)
// En WebGPU el depth write no es una máscara de bits, sino un booleano.
// ==========================================

static inline std::unordered_map<std::string, bool> StringToWGPU_DepthWriteEnabled = {
	{ "ZERO", false },
	{ "ALL", true },
};


// ==========================================
// 11. STENCIL OPERATIONS
// ==========================================

static inline std::unordered_map<wgpu::StencilOperation, std::string> WGPU_STENCIL_OPToString = {
	{ wgpu::StencilOperation::Keep, "KEEP" },
	{ wgpu::StencilOperation::Zero, "ZERO" },
	{ wgpu::StencilOperation::Replace, "REPLACE" },
	{ wgpu::StencilOperation::Invert, "INVERT" },
	{ wgpu::StencilOperation::IncrementClamp, "INCR_SAT" },
	{ wgpu::StencilOperation::DecrementClamp, "DECR_SAT" },
	{ wgpu::StencilOperation::IncrementWrap, "INCR" },
	{ wgpu::StencilOperation::DecrementWrap, "DECR" },
};

static inline std::unordered_map<std::string, wgpu::StencilOperation> StringToWGPU_STENCIL_OP = {
	{ "KEEP", wgpu::StencilOperation::Keep },
	{ "ZERO", wgpu::StencilOperation::Zero },
	{ "REPLACE", wgpu::StencilOperation::Replace },
	{ "INVERT", wgpu::StencilOperation::Invert },
	{ "INCR_SAT", wgpu::StencilOperation::IncrementClamp },
	{ "DECR_SAT", wgpu::StencilOperation::DecrementClamp },
	{ "INCR", wgpu::StencilOperation::IncrementWrap },
	{ "DECR", wgpu::StencilOperation::DecrementWrap },
};