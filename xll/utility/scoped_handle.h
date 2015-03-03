// scoped_handle.h - Windows handle modeled after unique_ptr
#pragma once

namespace scoped_handle {

	template<class H>
	struct traits {
		static H invalid(void) throw()
		{
			return nullptr;
		}
		static void close(H& h) throw()
		{
			CloseHandle(h);
		}
	};

	template<class H>
	class base {
	protected:
		H h_;
	public:
		explicit base(H h = traits<H>::invalid())
			: h_(h)
		{ }
		base(const base& h) = delete;
		base(base&& h)
			: h_(h.h_)
		{
			h.h_ = traits<H>::invalid();
		}
		base& operator=(const base&) = delete;
		base& operator=(base&& h)
		{
			if (this != &h) {
				h_ = h.h_;
				h.h_ = traits<H>::invalid();			
			}

			return *this;
		}
		virtual ~base()
		{
			if (h_ != traits<H>::invalid())
				traits<H>::close(h_);
		}
		operator const H&() const
		{
			return h_;
		}
	};

};

template<class H>
inline ::scoped_handle::base<H> make_scoped_handle(H h)
{
	return ::scoped_handle::base<H>(h);
}