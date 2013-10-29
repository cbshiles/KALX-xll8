// handle.h - lightweight C++ object embedding
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// Handles must be in their own cell for garbage collection to work.
#include <vector>

namespace xll {

	typedef double HANDLEX;

	// HANDLEX that returns an error to Excel
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
	};

	// handle denormalized numbers???
	template<class T>
	inline HANDLEX p2h(T* p)
	{
		union {
			T* p;
			HANDLEX h;
		} u;

		u.p = p;

		return u.h;
	}
	template<class T>
	inline T* h2p(HANDLEX h)
	{
		union {
			T* p;
			HANDLEX h;
		} u;

		u.h = h;

		return u.p;
	}

	template<class T>
	class handle_container {
		std::vector<T*> hc_;
		handle_container(const handle_container&);
		handle_container& operator=(const handle_container&);
	public:
		handle_container()
		{ }
		~handle_container()
		{
			for (auto pi : hc_)
				delete pi;
		}
		void add(T* p)
		{
			ensure (0 == find(p));

			hc_.push_back(p);
		}
		T* find(T* p)
		{
			auto pi = std::find(hc_.begin(), hc_.end(), p);

			return pi == hc_.end() ? 0 : *pi;
		}
		void remove(T* p)
		{
			auto pi = std::find(hc_.begin(), hc_.end(), p);

			if (pi != hc_.end()) {
				delete *pi;
				hc_.erase(pi);
			}
		}
	};

	template<class T>
	class handle {
		static handle_container<T>& handles(void)
		{
			static handle_container<T> hc_;

			return hc_;
		}
		T* p_;
	public:
		handle(T* p)
			: p_(p)
		{
			handles().add(p);

			OPERX o = XLL_XL_(Coerce, XLL_XLF(Caller));
			if (o.xltype == xltypeNum && o.val.num != 0) {
				handles().remove(h2p<T>(o.val.num));
			}
		}
		handle(HANDLEX h, bool check = true)
			: p_(handles().find(h2p<T>(h)))
		{
			if (check)
				ensure (p_);
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