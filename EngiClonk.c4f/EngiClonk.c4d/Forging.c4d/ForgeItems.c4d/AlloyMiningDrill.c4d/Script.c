/*-- Neues Script --*/

#strict
#include MDRL
#include DUMM

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(250*Mass, this());
	SetClrModulation(Color, this());
}

protected func SignalDelay()
{
	var x;
	if(GetDir() == 0) x = -25;
	else x = 25;
	
	if(PoweredEnough()){
	if(GetMaterial(x,0) != Material("Sky") && GetMaterial(x,0) != Material("Tunnel") && Abs(GetXDir()) > 2){
			x = GetX()+x;
			var y;
			y = GetY();
		/* FreeRect(GetX(), GetY()-20, 25,30);
		FreeRect(GetX()+x, GetY()-20, 10,30); */
		BlastFree(x,(y-2)-(Power/2),16+Power,GetOwner()-1);
		Sound("Drill");
	}
  }
  
  DoEnergy(-1);
  
  // Keine Wartezeit
  if (!iWait) return(0);
  // Wartezeit aufzählen
  --iWait;
  // Abfahrtszeit vorm Zählerende
  if (iWait == 2) DepartFromSignal();
  return(1);
}

private func AdjustTrainSpeed()
{
	var PW;
	PW = Power;
	if(Power == 0) PW = 1;
	
	if(PW < 0) SetPhysical("Walk",(500*Abs(PW))*Speed, 2);
	else SetPhysical("Walk",(500/PW)*Speed, 2);
}