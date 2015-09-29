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
	struct type_map {
		
	};

	template<class X>
	class XArgs {
		typedef typename traits<X>::xchar xchar;
		typedef typename traits<X>::xcstr xcstr;
		typedef typename traits<X>::xword xword;
		typedef typename traits<X>::xstring xstring;

		XOPER<X> args_;
//		XOPER<X> type_; // return type followed by arg types
		mutable XOPER<X> default_;
		xstring doc_, see_; // documentation, see also

		XOPER<X>& Arg(RegisterArg ra)
		{
			return args_[static_cast<xword>(ra)];
		}
		const XOPER<X>& Arg(RegisterArg ra) const
		{
			return args_[static_cast<xword>(ra)];
		}
	public:
		XArgs(xcstr category)
			: args_((xword)RegisterArg::Max, 1)
		{
			Arg(RegisterArg::MacroType) = -1; // documentation
			Arg(RegisterArg::Category) = category;
		}
		XArgs(xcstr proc, xcstr text)
			: args_((xword)RegisterArg::Max, 1)
		{
			Arg(RegisterArg::MacroType) = 2; // macro command
			Arg(RegisterArg::Procedure) = proc;
			Arg(RegisterArg::FunctionText) = text;
		}
		XArgs(xcstr proc, xcstr type, xcstr func, xcstr args,
			xcstr cat = 0, xcstr help = 0, xcstr doc = 0)
			: args_((xword)RegisterArg::Max, 1)
		{
			Arg(RegisterArg::MacroType) = 1; // worksheet function
			Arg(RegisterArg::Procedure) = proc;
			Arg(RegisterArg::TypeText) = type;
			Arg(RegisterArg::FunctionText) = func;
			Arg(RegisterArg::ArgumentText) = args;
			if (cat)
				Arg(RegisterArg::Category) = cat;
			if (help)
				Arg(RegisterArg::FunctionHelp) = help;
			if (doc)
				doc_ = doc;
		}
		XArgs(xcstr type, xcstr proc, xcstr func)
			: args_((xword)RegisterArg::Max, 1)
		{
			Arg(RegisterArg::MacroType) = 1; // worksheet function
			Arg(RegisterArg::Procedure) = proc;
			Arg(RegisterArg::TypeText) = type;
			Arg(RegisterArg::FunctionText) = func;
//			type_.push_back(XOPER<X>(type));
		}

		XArgs(const XArgs&) = default;
		XArgs& operator=(const XArgs&) = default;
/*
		XArgs(XArgs&& args)
		{
			args_ = args.args_;
			type_ = type_;
			default_ = args.default_;
			see_ = args.see_;
			doc_ = args.doc_;
		}
		XArgs& operator=(XArgs&& args)
		{
			args_ = args.args_;
			type_ = type_;
			default_ = args.default_;
			see_ = args.see_;
			doc_ = args.doc_;

			return *this;
		}
*/
		~XArgs()
		{ }

		xword Arity() const
		{
			xword n = size() - static_cast<xword>(RegisterArg::Max);
			default_.resize(n, 1);

			return n;
		}
		xword size() const
		{
			return args_.size();
		}
		XOPER<X>& operator[](xword i)
		{
			return args_[i];
		}
		const XOPER<X>& operator[](xword i) const
		{
			return args_[i];
		}

		XArgs& Hidden(void)
		{
			Arg(RegisterArg::MacroType) == 0;

			return *this;
		}
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
		bool isDocument(void) const
		{
			return Arg(RegisterArg::MacroType) == -1;
		}

		XArgs& Volatile(void)
		{
			Arg(RegisterArg::TypeText).append(traits<X>::VOLATILE());

			return *this;
		}
		XArgs& Uncalced(void)
		{
			Arg(RegisterArg::TypeText).append(traits<X>::UNCALCED());

			return *this;
		}
		XArgs& ThreadSafe(void)
		{
			if (typeid(X) == typeid(XLOPER12)) {
				Arg(RegisterArg::TypeText).append(traits<X>::THREAD_SAFE());
			}

			return *this;
		}
		XArgs& ClusterSafe(void)
		{
			if (typeid(X) == typeid(XLOPER12)) {
				Arg(RegisterArg::TypeText).append(traits<X>::CLUSTER_SAFE());
			}

			return *this;
		}
		XArgs& Asynchronous(void)
		{
			if (typeid(X) == typeid(XLOPER12)) {
				Arg(RegisterArg::TypeText).append(traits<X>::ASYNCHRONOUS());
			}

			return *this;
		}

		// add an argument
		XArgs& Arg(xcstr type, xcstr name, xcstr help)
		{
			return Arg(type, name, help, XOPER<X>(xltype::Missing));
		}

		template<class T>
		XArgs& Arg(xcstr type, xcstr name, xcstr help, const T& t)
		{
			Arg(RegisterArg::TypeText).append(type);
			if (Arg(RegisterArg::ArgumentText))
				Arg(RegisterArg::ArgumentText).append(traits<X>::COMMA());
			Arg(RegisterArg::ArgumentText).append(name);
			args_.push_back(help); // individual argument help

			default_.push_back(XOPER<X>(t));

			return *this;
		}

#pragma region OPER_types

		// (double) 64-bit IEEE floating point number.
		XArgs& Num(xcstr name, xcstr help)
		{
			return Arg(XLL_DOUBLEX, name, help);
		}
		template<class T>
		XArgs& Num(xcstr name, xcstr help, const T& t)
		{
			return Arg(XLL_DOUBLEX, name, help, t);
		}
		// (double) Excel Julian date.
		XArgs& Date(xcstr name, xcstr help)
		{
			return Arg(XLL_DOUBLEX, name, help);
		}
		XArgs& Handle(xcstr name, xcstr help)
		{
			return Arg(XLL_HANDLEX, name, help);
		}
		// (traits<XLOPERX>::xcstr) Null terminated C string.
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
			return Arg(XLL_LPOPERX, name, help);
		}
		// (LPOPER) Two dimensional range of OPER values.
		XArgs& Range(xcstr name, xcstr help)
		{
			return Arg(XLL_LPOPERX, name, help);
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
		// (WORD) Unsigned 2 byte int.
		XArgs& Word(xcstr name, xcstr help)
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
			// trim
			xstring s(function_help);
			s.erase(0, s.find_first_not_of(' '));
			s.erase(s.find_last_not_of(' ') + 1);

			Arg(RegisterArg::FunctionHelp) = s.c_str();

			return *this;
		}

		// Individual help in the Function Wizard.
		const XOPER<X>& ArgumentHelp(xword i) const
		{
			ensure (i > 0);
			ensure (9 + i < arg_.size());

			return Arg(9 + i);
		}

		// Note documentation always uses chars to support string literals
		XArgs& Documentation(xcstr doc = 0, xcstr see = 0)
		{
			if (doc)
				doc_ = doc;
			if (see)
				see_ = see;

			return *this;
		}
		XArgs& Documentation(const xstring& doc)
		{
			doc_ = doc;

			return *this;
		}
		const xstring& Documentation() const
		{
			return doc_;
		}
		const xstring& SeeAlso() const
		{
			return doc_;
		}

		const XOPER<X>& Default(xword i) const
		{
			return default_[i];
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

