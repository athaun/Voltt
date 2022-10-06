#pragma once

#include <functional>

struct defer {

	using Action = std::function<void(void)>;

	Action m_action;

	explicit defer(const Action& _act)
	: m_action(_act)
	{}

	explicit defer(Action&& _act)
	: m_action(std::move(_act))
	{}

	~defer() { m_action(); }
};