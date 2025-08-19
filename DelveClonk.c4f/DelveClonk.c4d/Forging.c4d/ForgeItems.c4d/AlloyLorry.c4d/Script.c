#strict
#include LORY
#include DUMM

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(35*Mass, this());
	SetClrModulation(Color, this());
}