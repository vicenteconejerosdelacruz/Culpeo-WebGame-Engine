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
	
	bool Compile(SceneUnitId id, ShaderInstanceID shaderInstance, Source params, ShaderIncludesDependencies& dependencies)
	{
		auto& json = params.shaderTemplate;

		const std::string filename = json->path();

		// std::cout << "loading shader " << filename << std::endl;

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
		return true;
	}
}