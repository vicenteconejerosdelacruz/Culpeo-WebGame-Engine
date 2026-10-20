#pragma once
//#include <d3d12.h>
#include <webgpu/webgpu_cpp.h>

using namespace wgpu;
struct MaterialSamplerDesc : SamplerDescriptor {
	MaterialSamplerDesc() {
		addressModeU = AddressMode::ClampToEdge;
    	addressModeV = AddressMode::ClampToEdge;
    	addressModeW = AddressMode::ClampToEdge;
	    magFilter = FilterMode::Linear;
	    minFilter = FilterMode::Linear;
		mipmapFilter = MipmapFilterMode::Linear;
		lodMinClamp = 0.f;
    	lodMaxClamp = FLT_MAX;
		compare = CompareFunction::Never;
		maxAnisotropy = 0;
	}

	MaterialSamplerDesc(const MaterialSamplerDesc& other) {
		addressModeU = other.addressModeU;
    	addressModeV = other.addressModeV;
    	addressModeW = other.addressModeW;
	    magFilter = other.magFilter;
	    minFilter = other.minFilter;
		mipmapFilter = other.mipmapFilter;
		lodMinClamp = other.lodMinClamp;
    	lodMaxClamp = other.lodMaxClamp;
		compare = other.compare;
		maxAnisotropy = other.maxAnisotropy;
	}

	MaterialSamplerDesc(nlohmann::json& sampler)
	{
		nostd::ReplaceFromJsonUsingMap(addressModeU, StringToWGPU_ADDRESS_MODE, sampler, "addressModeU");
		nostd::ReplaceFromJsonUsingMap(addressModeV, StringToWGPU_ADDRESS_MODE, sampler, "addressModeV");
		nostd::ReplaceFromJsonUsingMap(addressModeW, StringToWGPU_ADDRESS_MODE, sampler, "addressModeW");
		nostd::ReplaceFromJsonUsingMap(magFilter, StringToWGPU_FILTER_MODE, sampler, "magFilter");
		nostd::ReplaceFromJsonUsingMap(minFilter, StringToWGPU_FILTER_MODE, sampler, "minFilter");
		nostd::ReplaceFromJsonUsingMap(mipmapFilter, StringToWGPU_MIPMAP_FILTER_MODE, sampler, "mipmapFilter");
		ReplaceFromJson(lodMinClamp, sampler, "lodMinClamp");
		ReplaceFromJson(lodMaxClamp, sampler, "lodMaxClamp");
		nostd::ReplaceFromJsonUsingMap(compare, StringToWGPU_COMPARISON_FUNC, sampler, "compare");
		ReplaceFromJson(maxAnisotropy, sampler, "maxAnisotropy");
	}

	MaterialSamplerDesc(const wgpu::SamplerDescriptor& other) {
		addressModeU = other.addressModeU;
    	addressModeV = other.addressModeV;
    	addressModeW = other.addressModeW;
	    magFilter = other.magFilter;
	    minFilter = other.minFilter;
		mipmapFilter = other.mipmapFilter;
		lodMinClamp = other.lodMinClamp;
    	lodMaxClamp = other.lodMaxClamp;
		compare = other.compare;
		maxAnisotropy = other.maxAnisotropy;
	}

	bool operator<(const MaterialSamplerDesc& other) const {
		return std::tie(addressModeU, addressModeV, addressModeW, magFilter,
			minFilter, mipmapFilter, lodMinClamp, lodMaxClamp,compare, maxAnisotropy)
			< std::tie(other.addressModeU, other.addressModeV, other.addressModeW, other.magFilter,
			other.minFilter, other.mipmapFilter, other.lodMinClamp, other.lodMaxClamp,other.compare, other.maxAnisotropy);
	}

	nlohmann::json json()
	{
		return nlohmann::json(
			{
				{ "addressModeU", WGPU_ADDRESS_MODEToString[addressModeU] },
				{ "addressModeV", WGPU_ADDRESS_MODEToString[addressModeV] },
				{ "addressModeW", WGPU_ADDRESS_MODEToString[addressModeW] },
				{ "magFilter", WGPU_FILTER_MODEToString[magFilter] },
				{ "minFilter", WGPU_FILTER_MODEToString[minFilter] },
				{ "mipmapFilter", WGPU_MIPMAP_FILTER_MODEToString[mipmapFilter] },
				{ "lodMinClamp", lodMinClamp },
				{ "lodMaxClamp", lodMaxClamp },
				{ "compare", WGPU_COMPARISON_FUNCToString[compare] },
				{ "maxAnisotropy", maxAnisotropy },
			}
		);
	}
};

inline MaterialSamplerDesc ToMaterialSamplerDesc(nlohmann::json j)
{
	return MaterialSamplerDesc(j);
}

inline nlohmann::json FromMaterialSamplerDesc(MaterialSamplerDesc d)
{
	return d.json();
}

/*
template <>
struct std::hash<std::vector<MaterialSamplerDesc>>
{
	std::size_t operator()(const std::vector<MaterialSamplerDesc>& v) const
	{
		std::string s;
		for (auto& e : v)
		{
			s += D3D12_FILTERToString[e.Filter];
			s += D3D12_TEXTURE_ADDRESS_MODEToString[e.AddressU];
			s += D3D12_TEXTURE_ADDRESS_MODEToString[e.AddressV];
			s += D3D12_TEXTURE_ADDRESS_MODEToString[e.AddressW];
			s += std::to_string(e.MipLODBias);
			s += std::to_string(e.MaxAnisotropy);
			s += D3D12_COMPARISON_FUNCToString[e.ComparisonFunc];
			s += D3D12_STATIC_BORDER_COLORToString[e.BorderColor];
			s += std::to_string(e.MinLOD);
			s += std::to_string(e.MaxLOD);
			s += std::to_string(e.ShaderRegister);
			s += std::to_string(e.RegisterSpace);
			s += D3D12_SHADER_VISIBILITYToString[e.ShaderVisibility];
		}
		return hash<std::string>()(s);
	}

	std::size_t operator()(const nlohmann::json& v, std::string attribute) const
	{
		std::vector<MaterialSamplerDesc> sv;
		for (unsigned int i = 0; i < v.at(attribute).size(); i++)
		{
			sv.push_back(ToMaterialSamplerDesc(v.at(attribute).at(i)));
		}
		return operator()(sv);
	}
};
*/

/*
inline void TransformJsonToMaterialSamplers(std::vector<MaterialSamplerDesc>& samplers, nlohmann::json object, const std::string& key) {

	if (object.contains(key)) {
		nlohmann::json jsamplers = object[key];

		std::transform(jsamplers.begin(), jsamplers.end(), std::back_inserter(samplers), [](nlohmann::json sampler) {
			MaterialSamplerDesc desc;
			nostd::ReplaceFromJsonUsingMap(desc.Filter, StringToD3D12_FILTER, sampler, "Filter");
			nostd::ReplaceFromJsonUsingMap(desc.AddressU, StringToD3D12_TEXTURE_ADDRESS_MODE, sampler, "AddressU");
			nostd::ReplaceFromJsonUsingMap(desc.AddressV, StringToD3D12_TEXTURE_ADDRESS_MODE, sampler, "AddressV");
			nostd::ReplaceFromJsonUsingMap(desc.AddressW, StringToD3D12_TEXTURE_ADDRESS_MODE, sampler, "AddressW");
			ReplaceFromJson(desc.MipLODBias, sampler, "MipLODBias");
			ReplaceFromJson(desc.MaxAnisotropy, sampler, "MaxAnisotropy");
			nostd::ReplaceFromJsonUsingMap(desc.ComparisonFunc, StringToD3D12_COMPARISON_FUNC, sampler, "ComparisonFunc");
			nostd::ReplaceFromJsonUsingMap(desc.BorderColor, StringToD3D12_STATIC_BORDER_COLOR, sampler, "BorderColor");
			ReplaceFromJson(desc.MinLOD, sampler, "MinLOD");
			ReplaceFromJson(desc.MaxLOD, sampler, "MaxLOD");
			ReplaceFromJson(desc.ShaderRegister, sampler, "ShaderRegister");
			ReplaceFromJson(desc.RegisterSpace, sampler, "RegisterSpace");
			nostd::ReplaceFromJsonUsingMap(desc.ShaderVisibility, StringToD3D12_SHADER_VISIBILITY, sampler, "ShaderVisibility");
			return desc;
			}
		);
	}
}
*/