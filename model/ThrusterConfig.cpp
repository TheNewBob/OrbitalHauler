#include "OpStdLibs.h"
#include "Oparse.h"

#include "ThrusterConfig.h"

using namespace Oparse;

OpModelDef ThrusterConfig::GetModelDef() {
	return OpModelDef() = {
		{ "isp", { _Param(isp), { _REQUIRED() } } },
		{ "thrust", { _Param(thrust), { _REQUIRED() } } }
	};
}
