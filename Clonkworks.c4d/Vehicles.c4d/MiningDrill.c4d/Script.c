/*-- Neues Script --*/

#strict
#include STMG

local iFuel;
local FuelTick;

func Construction(){
	return(_inherited());
}

func Initialize(){
	FuelTick = false;
	AddEffect("Drilling",this(),1,1,this());
	return(_inherited());
}

func IsFueled(){
	if(FindObject(FUDS)) iFuel = 250;
	return(iFuel > 0);
}

func ControlThrow(pClonk){
	if(ContentsCount() < 20 || !ContentsCount(,pClonk)) return(0);
	return(1);
}

public func IsTrainLike(){ return(1); }

private func AdjustTrainSpeed()
{
	if(IsFueled() && GetComDir() !=  COMD_None){
	if(!FuelTick){
		FuelTick = true;
		SetGraphics("Fueled",this(),GetID());
		Sound("Fuse");
		Sound("Fire",false,this(),0,0,+1);
	}
	SetPhysical("Walk", 30000, 2);
	}else{
	if(FuelTick){
		FuelTick = false;
		SetGraphics(0,this(),GetID());
		Sound("Discharge");
		Sound("Fire",false,this(),0,0,-1);
	}
	SetPhysical("Walk", (30000/3)*2, 2);
	}
}

private func MaxContents() { return(0); }

func ConsumeFuel(){
		if(FindContents(COAL)){
			RemoveObject(FindContents(COAL));
			iFuel += 250;
		}
		else if(FindContents(WOOD)){
			RemoveObject(FindContents(WOOD));
			iFuel += 125;
		}
}

protected func SignalDelay()
{
	//setting stats and sprite
	AdjustTrainSpeed();
	
	//fueling
	if(FindObject(FUDS)) iFuel = 250;
	if(GetComDir() != COMD_None && !iFuel){
		ConsumeFuel();
	}
	
  // Keine Wartezeit
  if (!iWait) return(0);
  // Wartezeit aufzählen
  --iWait;
  // Abfahrtszeit vorm Zählerende
  if (iWait == 2) DepartFromSignal();
  return(1);
}

protected func Drill(int x){
		if(iFuel < 20) ConsumeFuel();
		x = GetX()+x;
			var y;
			y = GetY();
		for(var i = -12; i < 12; i++){
			if(GetMaterial(AbsX(x),AbsY(y)+1) == Material("Granite") || !GetMaterialVal("Density", "Material", GetMaterial(AbsX(x), AbsY(y)+i)) || GetMaterial(AbsX(x),AbsY(y)+1) == Material("Vehicle")) continue;
			if(GetMaterialVal("DigFree", "Material", GetMaterial(AbsX(x), AbsY(y)+i))){
				BlastFree(x,y+i,RandomX(3,5),GetOwner()-1);
				if(!Random(10)) iFuel-=RandomX(1,2);
			}
			else{
				BlastFree(x,y-3,RandomX(15,17),GetOwner()-1);
				iFuel -= RandomX(1,15);
			}
		}
		Sound("Drill");
}

protected func Puff()
{
  // Geräusch nur wenn wir aktiv sind
  if (GetComDir() != COMD_None && Abs(GetXDir()) != 0 && IsFueled())
  {
    Sound("Chuff");
	if(GetDir() == 1)
    Smoke(-15, -8, 5 + Random(4));
	else Smoke(23, -8, 5 + Random(4));
  }
}

func IsAdvancedProduct(){ return(1); }

//drilling is an effect as it has to be done every frame.
public func FxDrillingTimer(){
		var x;
	if(GetDir() == 0) x = -25;
	else x = 25;
	
	if(IsFueled() && GetComDir() != COMD_None){
	if(GetMaterial(x,0) != Material("Sky") && GetMaterial(x,0) != Material("Tunnel") && Abs(GetXDir()) != 0){
		Drill(x);
	}else if(GetMaterial(x,5) != Material("Sky") && GetMaterial(x,5) != Material("Tunnel") && Abs(GetXDir()) != 0){
		Drill(x);
	}else if(GetMaterial(x,-5) != Material("Sky") && GetMaterial(x,-5) != Material("Tunnel") && Abs(GetXDir()) != 0){
		Drill(x);
	}
	}
}

public func ALKConnectType(){
	return([FNPP]);
}

//fuel Station Logic
public func REFUNeedFuel(){
	if(FindObject(FUDS)) return(0);
	if(ContentsCount(COAL) < 5 || ContentsCount(WOOD) < 10){
		return(1);
	}
}
public func REFUFuelType(){
	return([COAL,WOOD]);
}

public func HowToREFU(pFuel){
	Enter(this(),pFuel);
	return(1);
}

//INFOBAR
public func InfobarTrigger(){
	if(FindObject(FUDS)) return(0);
	return(IB_Grab);
}

public func InfobarMax(){
	return(20);
}

public func InfobarValue(){
	return(ContentsCount(WOOD)+ContentsCount(COAL));
}

public func InfobarColor(){
	return(RGBa(25,25,25));
}

public func InfobarInfo(){
	return("$InfoFuel$");
}