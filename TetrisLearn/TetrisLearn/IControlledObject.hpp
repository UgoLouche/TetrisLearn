#pragma once

#include <cstdint>

class IControlledObject
{
public:
	virtual ~IControlledObject();

	void inputDown();
	void inputLeft();
	void inputRight();
	void inputFlipR();
	void inputFlipL();
	void inputHold();
	void inputDashLeft();
	void inputDashRight();
	void inputDashDown();

	void input(uint32_t inputCode);

private:
	virtual void input_custom(uint32_t inputCode) = 0;
};
