/*-- Neues Script --*/

#strict
#include STMG

func Initlialize(){

}


func PoweredEnough(){
	if(!FindObject2(Find_ID(ENRG)) || GetEnergy() > 0){
		return(1);
	}else{
		return(0);
	}
}

//foo?
func NotPoweredEnough(){
	if(!FindObject2(Find_ID(ENRG)) || GetEnergy() > 0){
		return(0);
	}else{
		return(1);
	}
}

public func ContextPower(object pCaller){
	[$NotPowered$|Image=LNKT|Condition=NotPoweredEnough]
	SetCommand(pCaller, "Energy", this());
	return(1);
}

private func AdjustTrainSpeed()
{
  // Zug angefasst: langsam fahren
  if (FindObject(0, 0,0,0,0, OCF_Living(), "Push", this()) || (GetActionTarget() && FindObject(0, 0,0,0,0, OCF_Living(), "Push", GetActionTarget())))
    SetPhysical("Walk", 15000, 2);
  // Zug nicht angefasst: schnell fahren
  else
    SetPhysical("Walk", 15000, 2);
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