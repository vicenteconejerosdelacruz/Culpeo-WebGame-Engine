#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <JTemplate.h>

namespace Templates {

	using JUUID = std::string;
	using JNAME = std::string;

    void CreateSystemTemplates();
};