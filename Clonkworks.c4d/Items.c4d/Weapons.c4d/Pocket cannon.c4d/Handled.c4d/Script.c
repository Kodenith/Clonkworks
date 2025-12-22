/*-- Neues Script --*/

#strict 2

func Initialize() {
  return(1);
}


// grab stuff

func RejectGrabbed(pByObject){
	//other clonks can grab it. the aimer is forced to grab this object despite this being not allowed.
	return(1);
}

func Grabbed(pByObject, fGrab){
	if(!fGrab){
		ReturnToObj();
	}
}

//controls: classic (i suck at classic so not sure how to make this comfy for them og clonk players. sorry!)
func ControlUp(clonk){
	[$TxtAimup$|Image=CAN1:2]
	if(GetDir(clonk) == DIR_Left)
	SetAction("AimUp");
	else SetAction("AimDown");
	return(1);
}

public func ControlDig() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=Classic]
  SetAction("NoAim");
}

//controls: jnr
public func ControlLeft()
{
  [$TxtSwap$|Method=JumpAndRun]
  if(GetR() > 0) SetR(GetR()-180);
}

public func ControlRight()
{
  [$TxtSwap$|Method=JumpAndRun]
  if(GetR() < 0) SetR(GetR()+180);
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
  if(GetDir(clonk) == DIR_Left){
  if(comdir == COMD_Up) SetAction("AimUp");
  if(comdir == COMD_Down) SetAction("AimDown");
  if(comdir == COMD_Stop){
	  SetAction("NoAim");
  }
  }else{
	   if(comdir == COMD_Up) SetAction("AimDown");
	   if(comdir == COMD_Down) SetAction("AimUp");
	   if(comdir == COMD_Stop){
		SetAction("NoAim");
	   }
  }
}

//rotation
func AimUp(){
	SetR(GetR()+3);
}

func AimDown(){
	SetR(GetR()-3);
}

func DirHandle(){
	SetYDir(0);
	var Grabber = FindObject2(Find_Action("Push"),Find_ActionTarget(this()));
	if(!Grabber){
		ReturnToObj();
		return(0);
	}
	
	if(GetR() < 0){
		SetDir(DIR_Left);
		Grabber->SetDir(DIR_Left);
	}else{
		SetDir(DIR_Right);
		Grabber->SetDir(DIR_Right);
	}
	
	if(GetR() > 120) SetR(120);
	if(GetR() < -120) SetR(-120);
}

//general control
public func ControlThrow(pClonk)
{
  [$TxtFire$|Image=CAN1:1]
  return(Fire());
}

public func ControlDown(clonk)
{
  [$TxtAimdown$|Image=CAN1:0]
  	if(GetDir(clonk) == DIR_Left)
	SetAction("AimDown");
	else SetAction("AimUp");
}

public func Fire()
{
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  if (!pGunpowder || !pProjectile) 
  {
    Sound("Click");
    return(0);
  }
  RemoveObject(pGunpowder);

  if (GetOCF(pProjectile) & OCF_Alive) pProjectile->SetKiller(GetController());
  pProjectile->SetController(GetController());

  var iX = GetVertex(1,0);
  var iY = GetVertex(1,1);
  
  Exit(pProjectile,iX,iY,Random(360),Sin(GetR(), CannonPower(pProjectile)), -Cos(GetR(), CannonPower(pProjectile)),+30);
  pProjectile->~Launch();
  
  Sound("Blast1");
  
  for (var i = 0; i < 6; ++i) 
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));

  Schedule("ReturnToObj(true)",15,0,this());
  return(1);
}

//other
func ReturnToObj(Cooldown){
	ClearScheduleCall(this(),"ReturnToObj");
	Sound("Click");
	ChangeDef(PCAN);
	SetCategory(C4D_Object);
	if(Cooldown){
		this()->ApplyCooldown();
	}
}
protected func CannonPower(object obj) { return(8); }

//Mouse aiming
//TODO: make it not instant, like currently updated aimables

public func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  if(szCommand == "MoveTo"){
    SetR(Angle(GetX(),GetY(),iX,iY));
	if(GetR() > 120) SetR(120);
	if(GetR() < -120) SetR(-120);
	Fire();
	return(1);
  }
}
