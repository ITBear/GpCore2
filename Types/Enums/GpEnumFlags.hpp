#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_ENUMS)
/*
#include "GpEnum.hpp"

namespace GPlatform {

class GpEnumFlags
{
public:
	using value_type= GpEnum::value_type;
	using this_type	= value_type;
	using SP		= GpSharedPtr<this_type>;
	using CSP		= GpSharedConstPtr<this_type>;
	using WP		= GpWeakPtr<this_type>;
	using CWP		= GpWeakConstPtr<this_type>;

protected:
						GpEnumFlags	(void) noexcept:
						iFlags(0)
						{
						}

						GpEnumFlags	(const GpEnumFlags& aFlags) noexcept:
						iFlags(aFlags.iFlags)
						{
						}

						GpEnumFlags	(GpEnumFlags&& aFlags) noexcept:
						iFlags(aFlags.iFlags)
						{
						}

						GpEnumFlags	(const value_type aFlagsRaw) noexcept:
						iFlags(aFlagsRaw)
						{
						}

public:
	virtual				~GpEnumFlags(void) noexcept
						{
							iFlags = 0;
						}

	void				Clear		(void) noexcept {iFlags = 0;}

	value_type			FlagsRaw	(void) const noexcept {return iFlags;}
	void				SetFlagRaw	(const value_type aId) noexcept	{iFlags |=  value_type(value_type(1) << aId);}
	void				ClearFlagRaw(const value_type aId) noexcept {iFlags &= ~value_type(value_type(1) << aId);}
	bool				TestFlagRaw	(const value_type aId) const noexcept {return iFlags & value_type(value_type(1) << aId);}

	virtual void		SetFlagStr	(std::string_view aEnumName) = 0;
	virtual void		ClearFlagStr(std::string_view aEnumName) = 0;
	virtual bool		TestFlagStr	(std::string_view aEnumName) = 0;

	virtual std::string_view	ToStringSingleEnum	(const value_type aId) const = 0;

protected:
	value_type			iFlags;
};


template<typename E> class GpEnumFlagsST: public GpEnumFlags
{
public:
	static_assert(std::is_base_of<GpEnum, E>::value, "E must be inherited from GpEnum");

	using this_type	= GpEnumFlagsST<E>;
	using SP		= GpSharedPtr<this_type>;
	using CSP		= GpSharedConstPtr<this_type>;
	using WP		= GpWeakPtr<this_type>;
	using CWP		= GpWeakConstPtr<this_type>;

	using EType				= E;
	using EnumT				= typename E::EnumT;
	using init_mask_type	= std::initializer_list<EnumT>;

public:
						GpEnumFlagsST	(void) noexcept
						{
						}

						GpEnumFlagsST	(const GpEnumFlagsST& aFlags) noexcept:
						GpEnumFlags(aFlags)
						{
						}

						GpEnumFlagsST	(GpEnumFlagsST&& aFlags) noexcept:
						GpEnumFlags(std::move(aFlags))
						{
						}

						GpEnumFlagsST	(const init_mask_type aFlags) noexcept
						{
							SetFlags(aFlags);
						}

						GpEnumFlagsST	(const value_type aFlagsRaw) noexcept:
						GpEnumFlags(aFlagsRaw)
						{
						}

	virtual				~GpEnumFlagsST	(void) noexcept override final
						{
						}

	void				SetFlags		(const init_mask_type aFlags) noexcept
	{
		for (const EnumT e: aFlags)
		{
			GpEnumFlags::SetFlagRaw(GpEnumFlags::value_type(e));
		}
	}

	void				SetFlagE		(const EnumT aEnum) noexcept
	{
		GpEnumFlags::SetFlagRaw(GpEnumFlags::value_type(aEnum));
	}

	void				ClearFlagE		(const EnumT aEnum) noexcept
	{
		GpEnumFlags::ClearFlagRaw(GpEnumFlags::value_type(aEnum));
	}

	bool				TestFlagE		(const EnumT aEnum) const noexcept
	{
		return GpEnumFlags::TestFlagRaw(GpEnumFlags::value_type(aEnum));
	}

	virtual void		SetFlagStr		(std::string_view aEnumName) override final
	{
		GpEnumFlags::SetFlagRaw(GpEnumFlags::value_type(E::SFromString(aEnumName)));
	}

	virtual void		ClearFlagStr	(std::string_view aEnumName) override final
	{
		GpEnumFlags::ClearFlagRaw(GpEnumFlags::value_type(E::SFromString(aEnumName)));
	}

	virtual bool		TestFlagStr		(std::string_view aEnumName) override final
	{
		return GpEnumFlags::TestFlagRaw(GpEnumFlags::value_type(E::SFromString(aEnumName)));
	}

	virtual std::string_view
						ToStringSingleEnum	(const value_type aId) const override final
	{
		return E::SToString(EnumT(aId));
	}

	this_type&			operator=		(const this_type& aFlags) noexcept;

	friend this_type	operator|		(const this_type& aFlagsLeft, const this_type& aFlagsRight) noexcept
	{
		return this_type(aFlagsLeft.iFlags | aFlagsRight.iFlags);
	}

	friend this_type	operator|		(const this_type& aFlagsLeft, const typename E::EnumT aFlagRight) noexcept
	{
		return this_type(aFlagsLeft.iFlags | value_type(value_type(1) << aFlagRight));
	}

	friend this_type	operator|		(const typename E::EnumT aFlagLeft, const typename E::EnumT aFlagRight) noexcept
	{
		return this_type(value_type(value_type(1) << aFlagLeft) | value_type(value_type(1) << aFlagRight));
	}

	friend this_type	operator|		(const typename E::EnumT aFlagLeft, const this_type& aFlagsRight) noexcept
	{
		return this_type(value_type(value_type(1) << aFlagLeft) | aFlagsRight.iFlags);
	}
};

template<typename E>
typename GpEnumFlagsST<E>::this_type&	GpEnumFlagsST<E>::operator= (const this_type& aFlags) noexcept
{
	iFlags = aFlags.iFlags;
	return *this;
}

}//GPlatform

//********************** Hash *********************
namespace std{

template<> struct hash<GPlatform::GpEnumFlags>
{
	using argument_type = GPlatform::GpEnumFlags;
	using result_type	= std::size_t;

	result_type operator()(argument_type const& aArg) const noexcept
	{
		return result_type(aArg.FlagsRaw());
	}
};

}//std
*/
#endif//#if defined(GP_USE_ENUMS)
