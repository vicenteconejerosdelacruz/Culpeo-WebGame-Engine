#include "ShaderCompiler.h"
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>
#include <Utils/FileSystem.h>
#include <Renderer.h>
#include <Scene.h>
using namespace Templates;

extern std::unique_ptr<JRenderer> renderer;

struct PreprocessorState {
    bool active;       // ¿Debe escribirse este bloque al output?
    bool branchTaken;  // ¿Ya se ha ejecutado alguna rama (#if, #ifdef, #elif) en este bloque?
};

namespace ShaderCompiler {

	std::vector<ShaderResourceInfo> ParseWGSLBindings(const std::string& preprocessedWGSL) {
		std::vector<ShaderResourceInfo> resources;

		// Expresión regular para capturar:
		// 1. Grupo: @group(X)
		// 2. Binding: @binding(Y)
		// 3. Palabra clave var (con o sin atributos como <uniform>, <storage, read>, etc.)
		// 4. Nombre de la variable
		// 5. Tipo / Estructura después de los dos puntos (:)
		std::regex bindingRegex(R"(@group\s*\(\s*(\d+)\s*\)\s*@binding\s*\(\s*(\d+)\s*\)\s*var\s*(?:<[^>]*>)?\s*([a-zA-Z_]\w*)\s*:\s*([^;]+);)");

		auto begin = std::sregex_iterator(preprocessedWGSL.begin(), preprocessedWGSL.end(), bindingRegex);
		auto end = std::sregex_iterator();

		for (std::sregex_iterator i = begin; i != end; ++i) {
			std::smatch match = *i;
			ShaderResourceInfo info;
			
			info.group = static_cast<uint32_t>(std::stoul(match[1].str()));
			info.binding = static_cast<uint32_t>(std::stoul(match[2].str()));
			info.variableName = match[3].str();
			
			// Limpiar espacios en blanco al inicio y final del tipo extraído
			std::string rawType = match[4].str();
			rawType.erase(0, rawType.find_first_not_of(" \t\r\n"));
			rawType.erase(rawType.find_last_not_of(" \t\r\n") + 1);
			info.typeName = rawType;

			resources.push_back(info);
		}

		return resources;
	}

std::string PreprocessWGSL(
    const std::string& filename, 
    std::unordered_set<std::string>& definedMacros,
    const std::function<std::string(const std::string&)>& loadFileCallback,
    std::unordered_set<std::string>& includedFiles) 
{
    // Evitar inclusiones circulares
    if (includedFiles.count(filename)) {
        return "";
    }
    includedFiles.insert(filename);

    std::string fileContent = loadFileCallback(filename);
    if (fileContent.empty()) {
        std::cerr << "[Preprocessor] Error: No se pudo cargar el archivo: " << filename << std::endl;
        return "";
    }

    std::istringstream stream(fileContent);
    std::ostringstream output;
    std::string line;

    // Pila de condiciones inicializada con el estado global activo
    std::vector<PreprocessorState> conditionStack = { { true, true } };
    
    // Estado para rastrear comentarios multilínea (/* ... */) que cruzan varias líneas
    bool inMultiLineComment = false;

    while (std::getline(stream, line)) {
        // --- Paso 1: Eliminar comentarios en la línea ---
        std::string cleanedLine = "";
        
        for (size_t i = 0; i < line.length(); ++i) {
            if (inMultiLineComment) {
                if (i + 1 < line.length() && line[i] == '*' && line[i + 1] == '/') {
                    inMultiLineComment = false;
                    i++; 
                }
                continue;
            }

            if (i + 1 < line.length() && line[i] == '/' && line[i + 1] == '*') {
                inMultiLineComment = true;
                i++; 
                continue;
            }

            if (i + 1 < line.length() && line[i] == '/' && line[i + 1] == '/') {
                break; // Ignorar el resto de la línea
            }

            cleanedLine += line[i];
        }

        if (inMultiLineComment && cleanedLine.empty()) {
            continue;
        }

        // --- Paso 2: Recortar espacios iniciales en la línea ya limpia ---
        size_t start = cleanedLine.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            continue; // Línea vacía o solo espacios
        }

        std::string trimmed = cleanedLine.substr(start);

        // --- Paso 3: Evaluar y consumir CUALQUIER directiva del preprocesador que empiece con '#' ---
        if (!trimmed.empty() && trimmed[0] == '#') {
            if (trimmed.rfind("#include", 0) == 0) {
                if (conditionStack.back().active) {
                    size_t firstQuote = trimmed.find('"');
                    size_t lastQuote = trimmed.rfind('"');
                    if (firstQuote != std::string::npos && lastQuote != std::string::npos && lastQuote > firstQuote) {
                        std::string includePath = trimmed.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                        // Llamada recursiva para incluir el archivo anidado
                        output << PreprocessWGSL(includePath, definedMacros, loadFileCallback, includedFiles);
                    }
                }
            }
            else if (trimmed.rfind("#define", 0) == 0) {
                if (conditionStack.back().active) {
                    std::istringstream iss(trimmed);
                    std::string directive, macroName;
                    iss >> directive >> macroName;
                    if (!macroName.empty()) {
                        definedMacros.insert(macroName);
                    }
                }
            }
            else if (trimmed.rfind("#ifdef", 0) == 0) {
                std::istringstream iss(trimmed);
                std::string directive, macroName;
                iss >> directive >> macroName;

                bool parentActive = conditionStack.back().active;
                bool macroExists = (definedMacros.find(macroName) != definedMacros.end());
                bool shouldActivate = parentActive && macroExists;

                conditionStack.push_back({ shouldActivate, shouldActivate });
            }
            else if (trimmed.rfind("#ifndef", 0) == 0) {
                std::istringstream iss(trimmed);
                std::string directive, macroName;
                iss >> directive >> macroName;

                bool parentActive = conditionStack.back().active;
                bool macroExists = (definedMacros.find(macroName) != definedMacros.end());
                bool shouldActivate = parentActive && !macroExists;

                conditionStack.push_back({ shouldActivate, shouldActivate });
            }
            else if (trimmed.rfind("#elif", 0) == 0) {
                if (conditionStack.size() > 1) {
                    std::istringstream iss(trimmed);
                    std::string directive, macroName;
                    iss >> directive >> macroName;

                    bool parentActive = conditionStack[conditionStack.size() - 2].active;
                    bool branchTaken = conditionStack.back().branchTaken;
                    bool macroExists = (definedMacros.find(macroName) != definedMacros.end());

                    bool shouldActivate = parentActive && !branchTaken && macroExists;
                    conditionStack.back().active = shouldActivate;
                    if (shouldActivate) {
                        conditionStack.back().branchTaken = true;
                    }
                }
            }
            else if (trimmed == "#else") {
                if (conditionStack.size() > 1) {
                    bool parentActive = conditionStack[conditionStack.size() - 2].active;
                    bool branchTaken = conditionStack.back().branchTaken;

                    bool shouldActivate = parentActive && !branchTaken;
                    conditionStack.back().active = shouldActivate;
                    conditionStack.back().branchTaken = true;
                }
            }
            else if (trimmed == "#endif") {
                if (conditionStack.size() > 1) {
                    conditionStack.pop_back();
                }
            }
            
            // IMPORTANTE: Al ser una directiva de preprocesador, siempre se descarta del output final
            continue; 
        }

        // --- Paso 4: Líneas de código normal / WGSL ---
        if (conditionStack.back().active) {
            output << cleanedLine << "\n";
        }
    }

    return output.str();
}

	std::vector<VertexAttributeInfo> ParseVertexInputAttributes(const std::string& preprocessedWGSL) {
		std::vector<VertexAttributeInfo> attributes;

		// --- Paso 1: Encontrar dinámicamente el nombre de la estructura de entrada en la función @vertex ---
		// Busca cualquier función con @vertex y captura el nombre de su tipo de parámetro (ej. VertexShaderInput)
		std::regex vertexFuncRegex(R"(@vertex\s+fn\s+[a-zA-Z_]\w*\s*\(\s*[a-zA-Z_]\w*\s*:\s*([a-zA-Z_]\w*)\s*\))");
		
		std::smatch funcMatch;
		std::string inputStructName = "";

		if (std::regex_search(preprocessedWGSL, funcMatch, vertexFuncRegex)) {
			if (funcMatch.size() > 1) {
				inputStructName = funcMatch[1].str();
			}
		}

		if (inputStructName.empty()) {
			std::cerr << "[Reflection] Advertencia: No se encontró ninguna función @vertex con un struct de entrada válido." << std::endl;
			return attributes;
		}

		// --- Paso 2: Aislar el bloque de la estructura encontrada (struct NombreDeEstructura { ... }) ---
		std::regex structRegex(R"(struct\s+)" + inputStructName + R"(\s*\{([^}]*)\})");
		std::smatch structMatch;
		std::string structBody = "";

		if (std::regex_search(preprocessedWGSL, structMatch, structRegex)) {
			if (structMatch.size() > 1) {
				structBody = structMatch[1].str();
			}
		}

		if (structBody.empty()) {
			return attributes;
		}

		// --- Paso 3: Extraer todos los atributos y sus ubicaciones (@location) del cuerpo del struct ---
		// Patrón: @location(X) nombre_atributo : tipo
		std::regex fieldRegex(R"(@location\s*\(\s*(\d+)\s*\)\s*([a-zA-Z_]\w*)\s*:)");
		auto begin = std::sregex_iterator(structBody.begin(), structBody.end(), fieldRegex);
		auto end = std::sregex_iterator();

		for (std::sregex_iterator i = begin; i != end; ++i) {
			std::smatch match = *i;
			VertexAttributeInfo attr;
			attr.location = static_cast<uint32_t>(std::stoul(match[1].str()));
			attr.name = match[2].str();
			attributes.push_back(attr);
		}

		return attributes;
	}
	
	// static std::mutex compileMutex;
	bool Compile(SceneUnitId id, ShaderInstanceID shaderInstance, Source params, ShaderIncludesDependencies& dependencies)
	{
		// std::lock_guard<std::mutex> lock(compileMutex);
		//read the shader

		auto& json = params.shaderTemplate;

		const std::string filename = json->path();

		std::cout << "loading shader " << filename << std::endl;

		std::unordered_set<std::string> macros;
		std::transform(params.defines.begin(), params.defines.end(), std::inserter(macros,macros.begin()), [](std::string& def){ return def;});

		auto fileLoader = [](const std::string& path) {
			AssetStream stream(defaultShadersFolder + path); // Tu función existente
			return stream.get().str();
		};

		std::unordered_set<std::string> includedFiles;
		shaderInstance->byteCode = PreprocessWGSL(filename, macros, fileLoader, includedFiles);
		shaderInstance->shaderSource = params;
		shaderInstance->CreateVSSemantics();
		shaderInstance->CreateResourcesBinding();
		shaderInstance->CreateShaderModule(id);
		//shaderInstance.CreateResourcesBinding(shaderReflection, shaderDesc);
		//shaderInstance.CreateConstantsBuffersVariables(shaderReflection, shaderDesc);
		//shaderInstance.CreateByteCode(pCompileResult);

		//std::cout << shaderSource << std::endl;
	
		// GetSceneUnit(id)->PushToPreRender([ filename, shaderInstance] {
		// 	WGPUShaderSourceWGSL wgslDesc{};
		// 	wgslDesc.chain.sType = static_cast<WGPUSType>(WGPUSType_ShaderSourceWGSL);
		// 	wgslDesc.chain.next = nullptr;
		// 	wgslDesc.code = { shaderInstance->byteCode.c_str() , WGPU_STRLEN };

		// 	wgpu::ShaderModuleDescriptor descriptor{};
		// 	descriptor.label = filename.c_str();
		// 	descriptor.nextInChain = reinterpret_cast<const wgpu::ChainedStruct*>(&wgslDesc);

		// 	// 4. Crear el módulo en la GPU
		// 	shaderInstance->shaderModule = renderer->GetDevice().CreateShaderModule(&descriptor);
		// 	if (!shaderInstance->shaderModule) {
		// 		std::cerr << "[WebGPU] Error al compilar el shader WGSL." << std::endl;
		// 	} else 
		// 	{
		// 		std::cout << "[WebGPU] compilacion completa." << std::endl;
		// 	}
		// });
		
		/*
		ShaderByteCode shaderSource = DX::ReadDataAsync(filename.c_str()).get();

		//create a blob for the shader source code
		ComPtr<IDxcBlobEncoding> pSource;
		pUtils->CreateBlob(&shaderSource[0], (UINT32)shaderSource.size(), CP_UTF8, pSource.GetAddressOf());

		//build the arguments
		std::vector<LPCWSTR> arguments;
		arguments.push_back(L"-E");// -E for the entry point (eg. 'main')
		arguments.push_back(shaderEntryPoint[params.shaderType].c_str());
		arguments.push_back(L"-T");// -T for the target profile (eg. 'ps_6_6')
		arguments.push_back(params.shaderTarget.c_str());

#ifndef _DEBUG
		arguments.push_back(L"-Qstrip_debug"); //remove debug info(PDB)
		arguments.push_back(L"-Qstrip_reflect"); // Strip reflection data and pdbs (see later)
#else
		arguments.push_back(L"-Qembed_debug"); //add debug info(PDB)
#endif

		arguments.push_back(DXC_ARG_WARNINGS_ARE_ERRORS); //-WX
#ifdef _DEBUG
		arguments.push_back(DXC_ARG_DEBUG); //-Zi
#endif

		arguments.push_back(DXC_ARG_PACK_MATRIX_ROW_MAJOR);

		std::vector<std::wstring> wdefines;
		std::transform(params.defines.begin(), params.defines.end(), std::back_inserter(wdefines), [](std::string def) { return nostd::StringToWString(def); });
		wdefines.push_back(shaderDefine.at(params.shaderType));

		for (const std::wstring& def : wdefines) {
			arguments.push_back(L"-D");
			arguments.push_back(def.c_str());
		}

		DxcBuffer sourceBuffer{ .Ptr = pSource->GetBufferPointer(), .Size = pSource->GetBufferSize(), .Encoding = 0 };

		std::string compiler_arguments;
		for (int i = 0; i < arguments.size(); i++)
		{
			compiler_arguments += nostd::WStringToString(arguments[i]);
			if (i < (arguments.size() - 1))
				compiler_arguments += " ";
		}
		OutputDebugStringA(std::string("Compiling :" + nostd::WStringToString(shaderDefine.at(params.shaderType)) + " " + filename + " " + compiler_arguments + "\n").c_str());

		//compile the shader
		ComPtr<IDxcResult> pCompileResult;
		dependencies[params].clear();
		CustomIncludeHandler includeHandler(dependencies[params], defaultShadersFolder, pUtils);
		DX::ThrowIfFailed(pCompiler->Compile(&sourceBuffer, arguments.data(), (UINT32)arguments.size(), &includeHandler, IID_PPV_ARGS(pCompileResult.GetAddressOf())));

		//get the errors
		ComPtr<IDxcBlobUtf8> pErrors;
		pCompileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(pErrors.GetAddressOf()), nullptr);
		if (pErrors && pErrors->GetStringLength() > 0) {
			OutputDebugStringA(("compilation error:" + filename + "\n").c_str());
			OutputDebugStringA((char*)pErrors->GetBufferPointer());
			return false;
		}

		// Get shader reflection data.
		ComPtr<IDxcBlob> reflectionBlob{};
		DX::ThrowIfFailed(pCompileResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflectionBlob), nullptr));
		const DxcBuffer reflectionBuffer{ .Ptr = reflectionBlob->GetBufferPointer(), .Size = reflectionBlob->GetBufferSize(), .Encoding = 0, };

		//create the reflection
		ComPtr<ID3D12ShaderReflection> shaderReflection{};
		pUtils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(&shaderReflection));

		//get a description of the shader
		D3D12_SHADER_DESC shaderDesc{};
		shaderReflection->GetDesc(&shaderDesc);

		shaderInstance.shaderSource = params;
		shaderInstance.CreateVSSemantics(shaderReflection, shaderDesc);
		shaderInstance.CreateResourcesBinding(shaderReflection, shaderDesc);
		shaderInstance.CreateConstantsBuffersVariables(shaderReflection, shaderDesc);
		shaderInstance.CreateByteCode(pCompileResult);
		*/
		return true;
	}

	// void BuildShaderCompiler() {
	// 	DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(pUtils.GetAddressOf()));
	// 	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));
	// }

	// void DestroyShaderCompiler()
	// {
	// 	pCompiler = nullptr;
	// 	pUtils = nullptr;
	// }
}