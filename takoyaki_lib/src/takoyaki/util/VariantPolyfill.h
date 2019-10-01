#pragma once

namespace ty {

template <class... Ts>
struct make_overloaded : Ts... {
	using Ts::operator()...;
};
template <class... Ts>
make_overloaded(Ts...)->make_overloaded<Ts...>;

}  // namespace ty
