#pragma once

#include "m_constructor.h"


namespace mstd {

	template<class Iter>
	struct initialized_backout {
		Iter first_;
		Iter last_;

		explicit initialized_backout(Iter it) : first_(it), last_(it) {}
		initialized_backout(Iter first, Iter last) : first_(first), last_(last) {}

		initialized_backout(const initialized_backout&) = delete;
		initialized_backout& operator=(const initialized_backout&) = delete;

		~initialized_backout() {
			mstd::destroy(first_, last_);
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			mstd::construct(*last_, std::forward<Args>(args)...);
			++last_;
		}

		Iter release() {
			first_ = last_;
			return last_
		}

	};

}

