// args.h - Arguments to xlfRegister
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
// included by oper.h

namespace xll {

	enum class RegisterArg {
		ModuleText,
		Procedure,
		TypeText,
		FunctionText,
		ArgumentText,
		MacroType,
		Category,
		ShortcutText,
		HelpTopic,
		FunctionHelp,
		Max // start of ArgumentHelp
	};

	template<class X>
	class XArgs : public XOPER<X> {
		typedef typename traits<X>::xchar xchar;
		typedef typename traits<X>::xcstr xcstr;
		typedef typename traits<X>::xword xword;

		XOPER<X>& Arg(RegisterArg ra)
		{
			return operator[](static_cast<xword>(ra));
		}
		XOPER<X>& Append(RegisterArg ra, xcstr str)
		{
			return Arg(ra).append(str);
		}
	public:
		using XOPER<X>::operator[];
		XArgs()
//			: XOPER<X>((xword)RegisterArg::Max, 1)
		{ }
		XArgs(xcstr proc, xcstr text)
			: XOPER<X>(6, 1)
		{
			Arg(RegisterArg::MacroType) = 2; // macro command
			Arg(RegisterArg::Procedure) = proc;
			Arg(RegisterArg::FunctionText) = text;
		}
		XArgs(xcstr proc, xcstr type, xcstr func, xcstr args, xcstr cat = nullptr)
			: XOPER<X>((xword)RegisterArg::Max, 1)
		{
			Arg(RegisterArg::MacroType) = 1; // worksheet function
			Arg(RegisterArg::Procedure) = proc;
			Arg(RegisterArg::TypeText) = type;
			Arg(RegisterArg::FunctionText) = func;
			if (args)
				Arg(RegisterArg::ArgumentText) = args;
			if (cat)
				Arg(RegisterArg::Category) = cat;
		}
		XArgs(xcstr type, xcstr proc, xcstr func)
			: XOPER<X>((xword)RegisterArg::Max, 1)
		{
			Arg(RegisterArg::MacroType) = 1; // worksheet function
			Arg(RegisterArg::Procedure) = proc;
			Arg(RegisterArg::TypeText) = type;
			Arg(RegisterArg::FunctionText) = func;
		}

		XArgs(XArgs&& args)
			: XOPER<X>(args)
		{
			this->resize(0,0);
		}
		~XArgs()
		{ }

		bool isHidden(void) const
		{
			return Arg(RegisterArg::MacroType) == 0;
		}
	
		bool isFunction(void) const
		{
			return Arg(RegisterArg::MacroType) == 1;
		}
		bool isMacro(void) const
		{
			return Arg(RegisterArg::MacroType) == 2;
		}


		XArgs& Volatile(void)
		{
			Append(RegisterArg::TypeText, XLL_VOLATILEX);

			return *this;
		}
		XArgs& Uncalced(void)
		{
			Append(RegisterArg::TypeText,  XLL_UNCALCEDX);

			return *this;
		}
		XArgs& ThreadSafe(void)
		{
			if (typeid(X) == typeid(XLOPER12)) {
				Append(RegisterArg::TypeText,  XLL_THREAD_SAFE12);
			}

			return *this;
		}
		XArgs& ClusterSafe(void)
		{
			if (typeid(X) == typeid(XLOPER12)) {
				Append(RegisterArg::TypeText, XLL_CLUSTER_SAFE12);
			}

			return *this;
		}
		XArgs& Asynchronous(void)
		{
			if (typeid(X) == typeid(XLOPER12)) {
				Append(RegisterArg::TypeText, XLL_ASYNCHRONOUS12);
			}

			return *this;
		}

		// add an argument
		XArgs& Arg(xcstr type, xcstr name, xcstr help)
		{
			Append(RegisterArg::TypeText, type);
			if (Arg(RegisterArg::ArgumentText))
				Append(RegisterArg::ArgumentText, _T(", "));
			Append(RegisterArg::ArgumentText, name);
			push_back(XOPER<X>(help)); // individual argument help

			return *this;
		}

#pragma region OPER_types

		// (double) 64-bit IEEE floating point number.
		XArgs& Num(xcstr name, xcstr help)
		{
			return Arg(XLL_DOUBLEX, name, help);
		}
		// (xll::traits<XLOPERX>::xcstr) Null terminated C string.
		XArgs& Str(xcstr name, xcstr help)
		{
			return Arg(XLL_CSTRINGX, name, help);
		}
		// (traits<XLOPERX>::xcstr) Pascal counted string.
		XArgs& PStr(xcstr name, xcstr help)
		{
			return Arg(XLL_PSTRINGX, name, help);
		}
		// (BOOL) Boolean TRUE or FALSE value.
		XArgs& Bool(xcstr name, xcstr help)
		{
			return Arg(XLL_BOOLX, name, help);
		}
		/*
		// (LPXLOPER) Two dimensional range reference.
		XArgs& SRef(xcstr name, xcstr help)
		{
			return Arg(XLL_LPXLOPERX, name, help);
		}
		*/
		// (LPOPER) Two dimensional range of OPER values.
		XArgs& Multi(xcstr name, xcstr help)
		{
			return Arg(XLL_LOPERX, name, help);
		}
		// Same as Multi.
		XArgs& Range(xcstr name, xcstr help)
		{
			return Arg(XLL_LOPERX, name, help);
		}
		// (traits<XLOPERX>::xfp) Two dimensional array of doubles.
		XArgs& Array(xcstr name, xcstr help)
		{
			return Arg(XLL_FPX, name, help);
		}
		// (USHORT) Unsigned 2 byte int.
		XArgs& UShort(xcstr name, xcstr help)
		{
			return Arg(XLL_USHORTX, name, help);
		}
		// (SHORT) Signed 2 byte int.
		XArgs& Short(xcstr name, xcstr help)
		{
			return Arg(XLL_SHORTX, name, help);
		}
		// (LONG) Signed 4 byte int. 
		XArgs& Long(xcstr name, xcstr help)
		{
			return Arg(XLL_LONGX, name, help);
		}

#pragma endregion OPER_types

		XArgs& ModuleText(xcstr s)
		{
			Arg(RegisterArg::ModuleText) = s;

			return *this;
		}
		XOPER<X>& ModuleText(void)
		{
			return Arg(RegisterArg::ModuleText);
		}
		// C function name
		const XOPER<X>& Procedure() const
		{
			return Arg(RegisterArg::Procedure);
		}
		XArgs& Procedure(xcstr procedure)
		{
			Arg(RegisterArg::Procedure) = procedure;

			return *this;
		}
		// C signature
		const XOPER<X>& TypeText() const
		{
			return Arg(RegisterArg::TypeText);
		}
		XArgs& TypeText(xcstr type_text)
		{
			Arg(RegisterArg::TypeText).append(type_text);

			return *this;
		
		}
		// Excel function name
		const XOPER<X>& FunctionText() const
		{
			return Arg(RegisterArg::FunctionText);
		}
		XArgs& FunctionText(xcstr function_text)
		{
			Arg(RegisterArg::FunctionText) = function_text;

			return *this;
		}
		// Excel Control-Shift-A argument prompt
		const XOPER<X>& ArgumentText() const
		{
			return Arg(RegisterArg::ArgumentText);
		}
		XArgs& ArgumentText(xcstr argument_text)
		{
			Arg(RegisterArg::ArgumentText) = argument_text;

			return *this;
		}
		// 0 - hidden function, 1 - function, 2 - macro
		const XOPER<X>& MacroType() const
		{
			return Arg(RegisterArg::MacroType);
		}
		XArgs& MacroType(int macro_type)
		{
			Arg(RegisterArg::MacroType) = macro_type;

			return *this;
		}
		XArgs& MacroType(xcstr macro_type)
		{
			Arg(RegisterArg::MacroType) = macro_type;

			return *this;
		}
		// Function Wizard category
		const XOPER<X>& Category() const
		{
			return Arg(RegisterArg::Category);
		}
		XArgs& Category(xcstr category)
		{
			Arg(RegisterArg::Category) = category;

			return *this;
		}
		// CONTROL+SHIFT+shorcut_text single character macro shortcut
		const XOPER<X>& ShortcutText() const
		{
			return Arg(RegisterArg::ShortcutText);
		}
		XArgs& ShortcutText(xcstr shortcut_text)
		{
			Arg(RegisterArg::ShortcutText) = shortcut_text;

			return *this;
		}
		// path\file.chm!help_context_id
		const XOPER<X>& HelpTopic() const
		{
			return Arg(RegisterArg::HelpTopic);
		}
		XArgs& HelpTopic(xcstr help_topic)
		{
			Arg(RegisterArg::HelpTopic) = help_topic;

			return *this;
		}
		// Function description in Function Wizard
		const XOPER<X>& FunctionHelp() const
		{
			return Arg(RegisterArg::FunctionHelp);
		}
		XArgs& FunctionHelp(xcstr function_help)
		{
			Arg(RegisterArg::FunctionHelp) = function_help;

			return *this;
		}

		// Individual help in the Function Wizard.
		const XOPER<X>& ArgumentHelp(xword i) const
		{
			ensure (i > 0);
			ensure (9 + i < arg_.size());

			return Arg(9 + i);
		}
	};

	typedef XArgs<XLOPER>   Args;
	typedef XArgs<XLOPER12> Args12;
	typedef X_(Args)        ArgsX;

	typedef XArgs<XLOPER>   Function;
	typedef XArgs<XLOPER12> Function12;
	typedef X_(Args)        FunctionX;

	typedef XArgs<XLOPER>   Macro;
	typedef XArgs<XLOPER12> Macro12;
	typedef X_(Args)        MacroX;

	typedef XArgs<XLOPER>   Document;
	typedef XArgs<XLOPER12> Document12;
	typedef X_(Args)        DocumentX;

} // namespace xll

#include "addin.h"

