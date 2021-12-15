#pragma once

#ifndef __BUILDING__
#define __BUILDING__ 

namespace Tower_Defence {

	class Building {

	private:
		int m_coord;

	public:
		Building(int coord = 0);
		int getCoord()        const noexcept { return m_coord; }
		virtual ~Building() {};

		virtual void turn(Landscape&, std::stringstream&) = 0;

	};
} 
#endif __BUILDING__
