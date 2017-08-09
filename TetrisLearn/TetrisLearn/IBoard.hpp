#pragma once

#include <string>

#include "enum.hpp"

#include "IActingObject.hpp"
#include "IGraphicObject.hpp"
#include "IControlledObject.hpp"
#include "IBoardUI.hpp"
#include "IBoardContent.hpp"
#include "IBoardScore.hpp"
#include "IBlocksPool.hpp"
#include "ISettingsWrapper.hpp"

class IBoard : 
	public IActingObject, 
	public IGraphicObject, 
	public IControlledObject, 
	public IBoardUI, 
	public IBoardContent, 
	public IBoardScore, 
	public IBlocksPool
{
public:
	virtual ~IBoard();

	void gameOver();

	//Get a dump of the current inputData.
	std::string getStrInputData(InputKeys overrideInput);

private:
	virtual void gameOver_custom() = 0;
	virtual std::string getStrInputData_custom(InputKeys overrideInput) = 0;
};