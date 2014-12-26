// on.h - xlcOnXXX functions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once

// used with On<Key>
#define ON_SHIFT   _T("+")
#define ON_CTRL    _T("^")
#define ON_ALT     _T("%")
#define ON_COMMAND _T("*")

namespace xll {

	struct Data {
		static const int On = xlcOnData;
	};
	struct Doubleclick {
		static const int On = xlcOnDoubleclick;
	};
	struct Entry {
		static const int On = xlcOnEntry;
	};
	struct Key {
		static const int On = xlcOnKey;
	};
	struct Recalc {
		static const int On = xlcOnRecalc;
	};
	struct Sheet {
		static const int On = xlcOnSheet;
	};
	struct Time {
		static const int On = xlcOnTime;
	};
	struct Window {
		static const int On = xlcOnWindow;
	};

	// static On<Key> ok("shortcut", "MACRO");
	template<class Key, class X = XLOPERX>
	class On {
		typedef typename traits<X>::xcstr xcstr;
		typedef typename traits<X>::xword xword;
		typedef typename traits<X>::xint xint;
	public:
		On(xcstr text, xcstr macro)
		{
			XOPER<X> x(3,1);

			x[0] = Key::On;
			x[1] = text;
			x[2] = macro;

			callbacks().push_back(x);
		}
		On(xcstr text, xcstr macro, bool activate)
		{
			XOPER<X> x(4,1);

			ensure (Key::On == xlcOnSheet);
			x[0] = Key::On;
			x[1] = text;
			x[2] = macro;
			x[3] = activate;

			callbacks().push_back(x);
		}
		On(const XOPER<X>& time, xcstr macro, 
			const XOPER<X>& tolerance = OPERX(xltype::Missing), bool insert = true)
		{
			XOPER<X> x(5,1);

			ensure (Key::On == xlcOnTime);
			x[0] = Key::On;
			x[1] = time;
			x[2] = macro;
			x[3] = tolerance;
			x[4] = insert;

			callbacks().push_back(x);
		}

		static int Open(void)
		{
			try {
				for (const auto& i : callbacks()) {
					xword f = static_cast<xword>(i[0]);

					if (i.size() == 3)
						Excel<X>(f, i[1], i[2]);
					else if (i.size() == 4)
						Excel<X>(f, i[1], i[2], i[3]);
					else if (i.size() == 5)
						Excel<X>(f, i[1], i[2], i[3], i[4]);
				}
			}
			catch (const std::exception& ex) {
				XLL_ERROR(ex.what());

				return 0;
			}

			return 1;
		}
		static int Close(void)
		{
			try {
				for (const auto& i : callbacks()) {
					if (i.size() > 1) {
						xword f = static_cast<xword>(i[0]);
						Excel<X>(f, i[1]); // unregister
					}
				}
			}
			catch (const std::exception& ex) {
				XLL_ERROR(ex.what());

				return 0;
			}

			return 1;
		}
	private:
		typedef typename std::list<XOPER<X>> callback_list;
		typedef typename std::list<XOPER<X>>::iterator callback_iter;
		static callback_list& callbacks(void)
		{
			static callback_list callbacks_;

			return callbacks_;
		}
	};
} // namespace xll
