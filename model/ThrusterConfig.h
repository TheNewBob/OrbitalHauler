#pragma once

struct ThrusterConfig {
	double isp;
	double thrust;

	Oparse::OpModelDef GetModelDef();
};