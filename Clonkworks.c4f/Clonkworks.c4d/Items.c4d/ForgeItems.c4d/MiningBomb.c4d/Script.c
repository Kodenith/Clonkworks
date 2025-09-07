/*-- Neues Objekt --*/

#strict
#include DUMM

local Color;
local Mass;
local Speed;
local Power;
local Effect;

local Hitten;
local Time;
local Left;

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(10*Mass, this());
	SetClrModulation(Color, this());
	Time = 38-Speed;
	Left = 38 * Time;
	Left += 3 * 38;
}

func Initialize() {
	Hitten = false;
  return(1);
}

func Hit(){
	Sound("MetalHit*");
	if(!Hitten){
   SetGraphics("Lit", this(),MBMB,1,1);
   SetGraphics("Lit", this(),MBMB,2,3);
   Hitten = true;
   Sound("Fuse");
   Sound("Fusing", , , , , +1);
   AddEffect("Light", this(), 51, 1, this());
	}
}

func FxLightTimer(object pTarget, int EffectNumber){
	Left -= 1;
	if(Left <= 0){
		if(Power > 0)
		Explode(Power * 13);
		else Explode(13 - Abs(Power)); 
	}
}

func AssignEffects(){
	DebugLog("Mining Bomb is not affected by Effects!");
}
public func Activate()
{
 [$TxtFuse$]
	if(!Hitten){
   SetGraphics("Lit", this(),MBMB,1,1);
   SetGraphics("Lit", this(),MBMB,2,3);
   Hitten = true;
   Sound("Fuse");
   Sound("Fusing", , , , , +1);
   AddEffect("Light", this(), 51, 1, this());
	}
}