// Mutex.h
// ミューテックスの管理
#pragma once

#include <string>
#include <windows.h>
#include "utility.h"

namespace DGS {
	class Mutex : private Uncopyable {
		std::string name_;
		::HANDLE mutex_;
		bool duplicated_;

		void open();
		
	public:
		Mutex();
		Mutex(const std::string& name);
		~Mutex();

		bool isDuplicated() const { return duplicated_; }
	};
}
