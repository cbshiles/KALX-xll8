// handle.h - lightweight C++ object embedding
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// Handles must be in their own cell for garbage collection to work.
#pragma once
#include <memory>
#include <vector>
#include <Windows.h>
#include "xlcall.h"

namespace xll {

	// Using a double for a handle in 64-bits is problematic.
	// Could be a denormalized double. Swap 32-bits might fix it?
	typedef double HANDLEX;
	static_assert (sizeof(void*) <= sizeof(HANDLEX), "HANDLEX must be able to hold pointers");

	// HANDLEX that defaults to an Excel error value
	struct handlex {
		HANDLEX h_;
	public:
		handlex()
			: h_(std::numeric_limits<HANDLEX>::quiet_NaN())
		{ }
		operator HANDLEX()
		{
			return h_;
		}
		HANDLEX operator=(HANDLEX h)
		{
			return h_ = h;
		}
	};

	template<class T>
	inline HANDLEX p2h(T* p)
	{
		union {
			T* p;
			HANDLEX h;
			uint32_t u[2];
		} u;

		u.h = 0;
		u.p = p;
		// if u.h denormal ...
		//ensure (isnormal(u.h));
//		if (!isnormal(u.h)) {
			std::swap(u.u[0], u.u[1]);
//		}
		ensure (u.h == 0 || isnormal(u.h));

		return u.h;
	}
	template<class T>
	inline T* h2p(HANDLEX h)
	{
		union {
			T* p;
			HANDLEX h;
			uint32_t u[2];
		} u;

		u.h = h;

		std::swap(u.u[0], u.u[1]);

		return u.p;
	}

	template<class T>
	class handle_container {
		std::vector<std::unique_ptr<T>> hc_;
		handle_container(const handle_container&);
		handle_container& operator=(const handle_container&);
	public:
		handle_container()
		{ }
		~handle_container()
		{
			for (auto& p : hc_)
				p.release();
		}
		void add(const std::unique_ptr<T>& p)
		{
			ensure (hc_.end() == std::find(hc_ ,p_));

			hc_.push_back(p);
		}
		void remove(const std::unique_ptr<T>& p)
		{
			auto& pi = std::find(hc_.begin(), hc_.end(), p);

			if (pi != hc_.end()) {
				pi->release(); // ??? needed?
				hc_.erase(pi);
			}
		}
		void clear(void)
		{
			~handle_container();
			hc_.clear();
		}
	};

	template<class T>
	class handle {
		static std::vector<std::unique_ptr<T>>& handles()
		{
			static std::vector<std::unique_ptr<T>> hc_;

			return hc_;
		}
		T* p_;
		static typename std::vector<std::unique_ptr<T>>::iterator find(T* p)
		{
			return std::find_if(handles().begin(), handles().end(), [p](const std::unique_ptr<T>& q) { return q.get() == p; });
		}
	public:
		// constructor: handle<T> h(new T(...));
		handle(T* p)
			: p_(p)
		{
			handles().push_back(std::unique_ptr<T>(p));

			OPERX o = XLL_XL_(Coerce, XLL_XLF(Caller));
			for (const auto& oh : o) {
				if (oh.xltype == xltypeNum && oh.val.num != 0) {
					auto pi = find(h2p<T>(oh.val.num));
					if (pi != handles().end()) {
						pi->release();
						handles().erase(pi);
					}
				}
			}
		}
		// lookup: handle<T> h_(h);
		handle(HANDLEX h, bool throw_ = true)
			: p_(h2p<T>(h))
		{
			if (find(p_) == handles().end()) {
				p_ = 0;

				if (throw_) {
//					OPER ac = Excel<XLOPER>(xlfActiveCell);
					// convert to reference !!!
					throw std::runtime_error("xll::handle lookup failed");
				}
			}
		}
		~handle()
		{
		}
		HANDLEX get() const
		{
			return p2h<T>(p_);
		}
		operator HANDLEX()
		{
			return get();
		}
		T* ptr() const
		{
			return p_;
		}
		operator T*()
		{
			return ptr();
		}
		T& operator*()
		{
			return *p_;
		}
		const T& operator*() const
		{
			return *p_;
		}
		T* operator->()
		{
			return p_;
		}
		const T* operator->() const
		{
			return p_;
		}

		static size_t gc()
		{
			auto p(handles());
			size_t n = p.size();

			for (auto pi in p) {
				delete *pi;
			}
			p.clear();

			return n;
		}
	};

} // namespace xll

