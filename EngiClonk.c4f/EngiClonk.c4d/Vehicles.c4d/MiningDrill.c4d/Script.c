/*-- Neues Script --*/

#strict
#include STMG
#include CXEC

local CanExit;

func Construction(){
	CanExit = true;
}

func PoweredEnough(){
	if(!FindObject2(Find_ID(ENRG)) || GetEnergy() > 0){
		return(1);
	}else{
		return(0);
	}
}

private func AdjustTrainSpeed()
{
	SetPhysical("Walk", 15000, 2);
}

protected func SignalDelay()
{
	
	var x;
	if(GetDir() == 0) x = -25;
	else x = 25;
	
	if(PoweredEnough()){
	if(GetMaterial(x,0) != Material("Sky") && GetMaterial(x,0) != Material("Tunnel") && Abs(GetXDir()) != 0){
			x = GetX()+x;
			var y;
			y = GetY();
		/* FreeRect(GetX(), GetY()-20, 25,30);
		FreeRect(GetX()+x, GetY()-20, 10,30); */
		BlastFree(x,y-2,16,GetOwner()-1);
		Sound("Drill");
	}
  }
  
  if(CanExit && GetCon() > 99) CanExit = false;
  
  // Keine Wartezeit
  if (!iWait) return(0);
  // Wartezeit aufzählen
  --iWait;
  // Abfahrtszeit vorm Zählerende
  if (iWait == 2) DepartFromSignal();
  return(1);
}

protected func Puff()
{
  // Geräusch nur wenn wir aktiv sind
  if (GetComDir() != COMD_None && Abs(GetXDir()) != 0 && PoweredEnough())
  {
    Sound("Chuff");
	if(GetDir() == 1)
    Smoke(-10, -8, 5 + Random(4));
	else Smoke(18, -8, 5 + Random(4));
  }
}

func ExitWorkshop(){
	if(CanExit && GetCon() == 100){
		CanExit = false;
		return(1); 
	}else{
		return(0);
	}
}