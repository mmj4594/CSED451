#pragma once

/*
 * This class defines poses that character can have.
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