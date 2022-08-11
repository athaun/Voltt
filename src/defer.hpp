#pragma once

#include <functional>

struct defer {
	std::function<void(void)> m_action;

	explicit defer(const std::function<void(void)>& _act)
	: m_action(_act)
	{}

	explicit defer(std::function<void(void)>&& _act)
	: m_action(std::move(_act))
	{}

	~defer() { m_action(); }
};
