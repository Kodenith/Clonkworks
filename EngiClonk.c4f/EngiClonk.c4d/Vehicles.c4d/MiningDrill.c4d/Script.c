/*-- Neues Script --*/

#strict
#include STMG

local Made;

func Initlialize(){
	Made = false;
}


func PoweredEnough(){
	if(!FindObject2(Find_ID(ENRG)) || GetEnergy() > 0){
		return(1);
	}else{
		return(0);
	}
}

func NotPoweredEnough(){
	if(!FindObject2(Find_ID(ENRG)) || GetEnergy() > 0){
		return(0);
	}else{
		return(1);
	}
}

private func AdjustTrainSpeed()
{
	SetPhysical("Walk", 15000, 2);
}

public func ContextE(pCaller){
	[$TxtEnergysupply$|Image=LNKT|Condition=NotPoweredEnough]
	SetCommand(pCaller,"Energy",this());
}

protected func SignalDelay()
{
	
	if(GetID(Contained()) == WRKS && GetCon() == 100 && !Made){
		SetCommand(this(), "Exit");
		Made = true;
	}
	
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
		BlastFree(x,y-2,16,GetOwner()-1);
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

protected func Puff()
{
  // Geräusch nur wenn wir aktiv sind
  if (GetComDir() != COMD_None && Abs(GetXDir()) > 2 && PoweredEnough())
  {
    Sound("Chuff");
	if(GetDir() == 1)
    Smoke(-10, -8, 5 + Random(4));
	else Smoke(18, -8, 5 + Random(4));
  }
}