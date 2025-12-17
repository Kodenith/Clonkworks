/*-- Neues Script --*/

#strict 2
local rider;
local iFuel;
local CanCharge;

local RelayTimer; //for some reason riding clonks cant initiate control left double and control right double on their mounts, but anything else works. so this is a makeshift way to get the same effect.

func Initialize(){
	RelayTimer = 16;
	SetAction("Walk");
}

//GENERAL
func ControlUp(pClonk){
	if(rider == pClonk){
		if(GetAction() == "Walk" || GetAction() == "Idle"){
			iFuel -= 25;
			if(iFuel < 0) iFuel = 0;
			Sound("Airlock2");
			Jump();
		}
		return(1);
	}
}

func ControlDownDouble(pClonk){
	SetComDir(COMD_Stop);
    ObjectSetAction(rider,"Walk");
	rider = 0;
}

func DashRight(){
	CanCharge = false;
	SetDir(DIR_Right);
	SetXDir(70);
	SetYDir(-30);
	Sound("SteamBlast*");
	iFuel -= 50;
	return(1);
}

func DashLeft(){
	DashRight();
	SetDir(DIR_Left);
	SetXDir(-70);
}

//CLASSIC
func ControlLeft(pClonk){
	if(rider == pClonk && CheckFuel() && GetAction() == "Jump" && CanCharge && RelayTimer <= 15 && GetDir() ==  DIR_Left){
		DashLeft();
		for(var i = 0; i < RandomX(10,20); i++)
		CreateParticle("PSpark",20,0,RandomX(50,70),RandomX(-25,25),RandomX(30,100),RGBa(255,255,255));
		return(1);
		}
	if(!GetPlrJumpAndRunControl(pClonk->GetController())){
		SetDir(DIR_Left);
		SetComDir(COMD_Left);
		return(1);
	}
	if(GetAction() == "Jump")
	RelayTimer = 0;
}

func ControlRight(pClonk){
	if(rider == pClonk && CheckFuel() && GetAction() == "Jump" && CanCharge && RelayTimer <= 15 && GetDir() ==  DIR_Right){
		DashRight();
		for(var i = 0; i < RandomX(10,20); i++)
		CreateParticle("PSpark",-20,0,-RandomX(50,70),RandomX(-25,25),RandomX(30,100),RGBa(255,255,255));
		return(1);
		}
	if(!GetPlrJumpAndRunControl(pClonk->GetController())){
		SetDir(DIR_Right);
		SetComDir(COMD_Right);
		return(1);
	}
	if(GetAction() == "Jump")
	RelayTimer = 0;
}

func ControlDown(pClonk){
	if(!GetPlrJumpAndRunControl(pClonk->GetController())){
		SetComDir(COMD_Stop);
		return(1);
	}
}

//JUMP AND RUN
public func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
    SetComDir(comdir);
}

//Funcs and Riding
func Destruction()
{
  ObjectSetAction(rider,"Walk");
  return(_inherited());
}

func ActivateEntrance()
{
  if(!ObjectCall(rider,"IsRiding")) rider=0;
  if(GetOCF(Par(0))&OCF_CrewMember && !rider)
	  if(!CheckFuel()){
		  Sound("Error");
		  FuelWarn(this(),OBRL,GetController(Par(0))+1);
		  if(GetCommand(Par(0),0) == "Enter") FinishCommand(Par(0),1);
		  return(0);
	  }
    if(GetAction()!="Jump") {
      AdjustVertex();
      ObjectSetAction(rider=Par(0),"Ride",this());
	  var grabbers = FindObjects(Find_Action("Push"), Find_ActionTarget(this()), Find_Exclude(Contents()));
	  for(var i in grabbers){
			i->SetAction("Walk");
	  }
    }
  return(1);
}

func AdjustVertex(){
	if(GetDir() == DIR_Left) SetVertex(0,0,2);
	else SetVertex(0,0,-2);
	
	if(GetAction() == "Walk"){
		if(!CanCharge) CanCharge = true;
		var y;
		y=-5;
		if(GetPhase() == 1) y = -6;
		if(GetPhase() == 2) y = -7;
		if(GetPhase() == 3) y = -8;
		if(GetPhase() == 4) y = -9;
		if(GetPhase() == 5) y = -8;
		if(GetPhase() == 6) y = -6;
		SetVertex(0,1,y);
	}
	if(GetAction() == "Jump"){
		var y;
		y=-5;
		if(GetPhase() == 1) y = -7;
		if(GetPhase() == 2) y = -9;
		if(GetPhase() == 3) y = -10;
		if(GetPhase() == 4) y = -12;
		SetVertex(0,1,y);
	}
	if(GetAction() == "Tumble"){
		if(rider){
			ObjectSetAction(rider,"Tumble");
			SetXDir(GetXDir(),rider); SetYDir(GetYDir(),rider);
			rider = 0;
			SetComDir(COMD_Stop);
		}
	}
	if(GetAction() == "KneelDown"){
		var y;
		y=-1;
		if(GetPhase() == 0) y = -7;
		if(GetPhase() == 1) y = -5;
		if(GetPhase() == 2) y = -1;
		if(GetPhase() == 3) y = 0;
		if(GetPhase() == 4) y = -3;
		SetVertex(0,1,y);
	}
}

func Update(){
	AdjustVertex();
	CheckFuel();
	
	if(RelayTimer <= 15) RelayTimer++;
	
	var BannedPhases = [4,5,6,7,8];
	if(GetComDir() == COMD_Stop && InArray(GetPhase(),BannedPhases) != -1 && GetAction() == "Walk") SetPhase(GetPhase()+1);
	
	if(rider)
	if(!WildcardMatch(GetAction(rider),"*Ride*")){
		SetComDir(COMD_Stop);
		rider = 0;
	}
}

func CheckFuel(){
	if(iFuel < 0) iFuel = 0;
	if(FindObject(FUDS)) iFuel = 200;
	if(FindContents(OBRL) && iFuel <= 0){
		var Amount = FindContents(OBRL)->GetAmount();
		ChangeDef(BARL,FindContents(OBRL));
		iFuel += Amount;
		Sound("Snuff2");
	}
	if(iFuel > 0){
		return(1);
	}
}

func UseFuel(){
	if(CheckFuel() && !FindObject2(Find_Action("Push"), Find_ActionTarget(this()))) return(1);
}

func Puff(){
	if(!UseFuel()){
		SetPhysical("Jump",30000,2);
		SetPhysical("Walk",30000,2);
		return(0);
	}else{
		SetPhysical("Jump",75000,2);
		SetPhysical("Walk",100000,2);
	}
	if(GetPhase()%10 == 0){
		Smoke(GetVertex(0,0),GetVertex(0,1),RandomX(10,20));
		Sound("Chuff");
	}
	
	if(GetPhase()%4 == 0) iFuel--;
}

func RejectGrabbed(){
	return(rider);
}

//fuel Station Logic
public func REFUNeedFuel(){
	if(ContentsCount(OBRL) < 6) return(1);
}
public func REFUFuelType(){
	return([OBRL]);
}

public func HowToREFU(pFuel){
	Enter(this(),pFuel);
	return(1);
}

//inspired by western pack's steam horse
func ControlDigDouble(){
	[$TxtCheck$]
	var additional;
	for(var i in FindObjects(Find_ID(OBRL),Find_Container(this()))){
		additional += i->GetAmount(0);
	}
	Message("$TxtFuel$",this(),iFuel,additional);
}

public func IsAdvancedProduct(){ return(1); }
public func GetResearchBase(){ return(CVCB); }

func LandSound(){ Sound("ClonkHit*"); }