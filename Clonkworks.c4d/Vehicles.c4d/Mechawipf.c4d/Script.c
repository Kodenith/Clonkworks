/*-- Neues Script --*/

#strict 2
local rider;
local iFuel;

func Initialize(){
	SetAction("Walk");
}

//GENERAL
func ControlUp(pClonk){
	if(rider == pClonk){
		if(GetAction() == "Walk" || GetAction() == "Idle"){
			iFuel -= 45;
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

//CLASSIC
func ControlLeft(pClonk){
	if(!GetPlrJumpAndRunControl(pClonk->GetController()) && rider == pClonk){
		SetDir(DIR_Left);
		SetComDir(COMD_Left);
		return(1);
	}
}

func ControlRight(pClonk){
	if(!GetPlrJumpAndRunControl(pClonk->GetController()) && rider == pClonk){
		SetDir(DIR_Right);
		SetComDir(COMD_Right);
		return(1);
	}
}

func ControlDown(pClonk){
	if(!GetPlrJumpAndRunControl(pClonk->GetController()) && rider == pClonk){
		SetComDir(COMD_Stop);
		return(1);
	}
}

//JUMP AND RUN
public func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
	if(self == rider)
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
    if(GetAction()!="Jump") {
      AdjustVertex();
      ObjectSetAction(rider=Par(0),"Ride",this());
	  if(!CheckFuel()){
		  Sound("Error");
		  FuelWarn(this(),OBRL,GetController(Par(0))+1);
	  }
    }
  return(1);
}

func AdjustVertex(){
	if(GetDir() == DIR_Left) SetVertex(0,0,2);
	else SetVertex(0,0,-2);
	
	if(GetAction() == "Walk"){
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
}

func Update(){
	AdjustVertex();
	CheckFuel();
	var BannedPhases = [4,5,6,7,8];
	if(GetComDir() == COMD_Stop && InArray(GetPhase(),BannedPhases) != -1 && GetAction() == "Walk") SetPhase(GetPhase()+1);
	
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
		SetPhysical("Jump",75000,2);
		SetPhysical("Walk",100000,2);
		return(1);
	}else{
		SetPhysical("Jump",30000,2);
		SetPhysical("Walk",30000,2);
	}
}

func Puff(){
	if(!CheckFuel()) return(0);
	if(GetPhase()%10 == 0){
		Smoke(GetVertex(0,0),GetVertex(0,1),RandomX(10,20));
		Sound("Chuff");
	}
	
	if(GetPhase()%3 == 0) iFuel--;
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