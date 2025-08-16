/*-- Lore --*/

#strict
#include LORY
#include DUMM

/* Status */

public func IsLorry() { return(1); }
local Color;
local Mass;
local Speed;
local Power;
local Effect;

/* Steuerung */

func Initialize(){
	Color = RGBa(177, 171, 170, 0);
	Mass = 3;
}

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(25*Mass, this());
	SetClrModulation(Color, this());
}