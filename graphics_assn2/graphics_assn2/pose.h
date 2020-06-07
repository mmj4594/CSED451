#pragma once

/*
	\class pose

	\brief
	This class defines postures that character can have.

	\detail
	float lua_angle: joint angle of left upper arm
	float lla_angle: joint angle of left lower arm
	float rua_angle: joint angle of right upper arm
	float rla_angle: joint angle of right upper arm
 */

class pose {
public:
	pose(){}
	pose(float a, float b, float c, float d, int e) {
		lua_angle = a;
		lla_angle = b;
		rua_angle = c;
		rla_angle = d;
		color = e;
	}
	float lua_angle = 0;
	float lla_angle = 0;
	float rua_angle = 0;
	float rla_angle = 0;
	int color = 0;
};