#pragma once

struct OrbitalHaulerConfig
{
	ThrusterConfig mainEngineConfig;
	ThrusterConfig rcsConfig;

	Oparse::OpModelDef GetModelDef();

};

