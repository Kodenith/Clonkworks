/*-- Lore --*/

#strict
#include LORY

/* Status */

public func IsLorry() { return(1); }
local Color;
local Mass;

/* Steuerung */

func Initialize(){
	SetGraphics("Overlay.png", this(),, 1,1);
	Color = RGBa(177, 171, 170, 0);
	Mass = 3;
}

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(25*Mass, this());
	SetClrModulation(Color, this());
}

