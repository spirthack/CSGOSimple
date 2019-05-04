#pragma once
#include "options.hpp"
#include "singleton.hpp"

#include <fstream>




class Config : public Singleton<Config> {
public:
	void Save() {
		std::ofstream fout("csgosimple.cfg", std::ios::binary);
		const auto sz = sizeof(Options);
		const auto var_sz = sizeof(Var<bool>);
		const auto cnt = sz / var_sz;
		for (auto i = 0; i < cnt; i++) {
			const auto el = &(*(Var<int>*)(&g_Options)) + i;
			auto name = el->name;
			auto val = el->value;
			auto sizeof_val = el->size;
			fout << name << "\t" << Utils::BytesToString((unsigned char*)*(int*)&val, sizeof_val) << std::endl;
		}
		fout.close();
	}


	void Load() {
		std::ifstream fin("csgosimple.cfg", std::ios::binary);
		std::stringstream ss;
		ss << fin.rdbuf();


		auto lines = Utils::Split(ss.str(), "\n");

		for (auto line : lines) {
			auto data = Utils::Split(line, "\t");
			const auto sz = sizeof(Options);
			const auto var_sz = sizeof(Var<bool>);
			const auto cnt = sz / var_sz;
			for (auto i = 0; i < cnt; i++) {
				const auto &el = &(*(Var<bool>*)(&g_Options)) + i;
				if (data[0] == el->name) {
					auto bytes = Utils::HexToBytes(data[1]);
					memcpy(*(void**)&el->value, bytes.data(), el->size);
				}
			}
		}
		fin.close();
	}
};