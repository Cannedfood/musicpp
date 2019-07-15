#pragma once

#include <variant>

namespace music {

struct ToneBend {
	float amount = 0;
};

struct Mute {
	float muteAmount = .5f;
};

struct DynamicChange {
	float amount;
};

struct EffectChange {
	unsigned channel;
	float    value;
};

using Effect = std::variant<
	ToneBend,
	Mute,
	DynamicChange,
	EffectChange
>;

} // namespace music
