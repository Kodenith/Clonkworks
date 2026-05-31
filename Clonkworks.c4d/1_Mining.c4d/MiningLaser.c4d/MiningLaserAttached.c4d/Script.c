/*--- Fl ssigkeitsgesch tz (angeschraubt) ---*/

#strict

#include CTW2

local laser;
local firing;

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(MLCN); }

func ComFire(object clonk)
{
  //Sound("Click");
  if(!firing){
	  if(FindObject2(Find_Container(GetActionTarget()),Find_ID(OBRL))){
		  AddEffect("LaserFuelConsume",GetActionTarget(),1,1,this());
		  ShootStart();
	  }else{
		 RemoveEffect("LaserFuelConsume",GetActionTarget());
		 //nothing is pointless! :D
		  Sound("Error");
	  }
  }
  else
  ShootStop();
  //inherited(clonk, ...);

  return(1);
}

public func ComStopDouble(object pClonk){
	while(laser) ShootStop();
	return(_inherited());
}

func ShootStart()
{
  firing=1;
  var tower = GetActionTarget();
  var user=tower;
  var r = GetR();

      var dist = 20;
      var angle = r;
      //InsertMaterial(mat, Sin(angle, dist), -Cos(angle, dist), Sin(r, 200), -Cos(r, 200));
      var x = Sin(angle, dist);
      var y = -Cos(angle, dist);
      if(!laser)
      {
      laser = CreateObject(LAS2,x,y,GetController(user));
      laser->SetClrModulation(RGBa(200,50,0,0));
      }
      if(laser)
      {
   	Sound("MiningLaser",0,0,0,0,1);
        laser->Set(angle,8,200,0,this(),this,4,0);
        laser->SetContinuous(5);
      }

  //Sound("Splash*");
}

func ShootStop()
{
  if(laser) RemoveObject(laser);
  laser=0;
  firing=0;
  Sound("MiningLaser",0,0,0,0,-1);
}


func LaserR()
{
  /*if(laser)
  {
    var angle = GetR();
    var dist = 20;
    var x = Sin(angle, dist);
    var y = -Cos(angle, dist);
    //SetPosition(x,y,laser);
    laser->Set(angle,8,600,0,this(),this,4,0);
    laser->SetContinuous(5);
  }*/
}

public func WeaponEnd(&x, &y) {
    var angle = GetR();
    var dist = 20;
    x = Sin(angle, dist);
    y = -Cos(angle, dist);
}

public func GetWeaponR() {
  return(GetR());
}

public func AimAngle(int iMaxAngle, int iRange) { // Zielwinkel bestimmen
	var angle = GetR();
	return(angle);
}

func LaserStrike(object pObj, int iTime) 
{
  Message("Yes");
  DoDamage(-1,pObj);
  DoEnergy(-1,pObj,0,0,GetOwner(this)+1);
  return(1);
}

//fuel consumption
func FxLaserFuelConsumeTimer(pTarget,iNum,iTime){
	if(!firing) return(-1);
	if(FindObject2(Find_Container(pTarget),Find_ID(OBRL))){
    //decrease the fuel instead of eating the whole barrel.
    var Barrel = FindObject2(Find_Container(pTarget),Find_ID(OBRL));
    LocalN("iFillLevel",Barrel) -= 1;
    if(LocalN("iFillLevel",Barrel) <= 0)
		  ChangeDef(BARL,Barrel);
	}else{
		Sound("Discharge");
		ShootStop();
		return(-1);
	}
}

public func AutoFire(pByTower){
  if(!pByTower) pByTower = GetActionTarget();
  if(!FindContents(OBRL,pByTower)){
    return(0);
  }

  if(!firing) ComFire();
  else return(0);

  return(1);
}

public func AutoStop(pByTower){
  if(firing) ComFire();
}

public func AutoRange(){ 
    return(250);
}