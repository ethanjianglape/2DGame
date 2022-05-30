#pragma once
#ifndef _IUPDATABLE_HPP
#define _IUPDATABLE_HPP

namespace game {
	class IUpdatable {
	public:
		virtual void update() = 0;
	};
}

#endif