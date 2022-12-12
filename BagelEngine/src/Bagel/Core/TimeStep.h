#pragma once

namespace Bagel {
	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: _time(time) 
		{

		}

		//Implicitly convert a Timestep object into a float. Returns time in seconds
		operator float() const { return _time; }

		float GetSeconds() const { return _time; }
		float GetMilliseconds() const { return _time * 1000.0f; }

	private:
		float _time;
	};
}
