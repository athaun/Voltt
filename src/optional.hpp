#pragma once

#include "logger.hpp"
#include <functional>
#include <utility>

template<class T>
struct Optional {
	T data;
	bool none = true;

	auto unwrap() -> T& 
	{
		if (none) Logger::debug(Logger::DBCTX, Logger::DebugErrID::UNWRAP_ERR, "");
		return data;
	}
	
	auto unwrap_or(const T& _data) -> T& 
	{
		if (none) data = _data;
		none = false;
		return data;
	}
	
	auto unwrap_or(T&& _data) -> T&
	{
		if (none) data = std::move(_data);
		none = false;
		return data;
	}

	auto unwrap_or_else(const std::function<void(void)>& _act) -> T&
	{
		if (none) _act();
		return data;
	}

	auto is_none() -> const bool
	{
		return none;
	}

	auto is_some() -> const bool
	{
		return !none;
	}

	Optional() = default;

	explicit Optional(const T& _data)
	: data(_data), none(false)
	{}

	explicit Optional(T&& _data)
	: data(std::move(_data)), none(false)
	{}
};
