/*-- Neues Script --*/

#strict 2
local iFuel;

func Initialize() {
  SetAction("NoFuel");
  return(1);
}

func ControlLeft(){
	[$TxtLeft$]
	if(!iFuel){
		if(GetAction() == "Fuel") SetAction("NoFuel");
		SetComDir(COMD_Stop);
		warn(Par(0));
		Sound("StartFail");
		return(1);
	}else{
		if(GetAction() == "NoFuel") SetAction("Fuel");
	}
	if(GetAction() == "Fuel"){
	if(!CanTurn()) return(SetComDir(COMD_Stop));
	if(GetDir() == DIR_Right){
		//relative rotation
		var Grabbers = FindObjects(Find_Action("Push"),Find_ActionTarget(this()));
		for(var i in Grabbers){
			var iX = AbsX(GetX(i));
			var ReplaceMe = CreateObject(ROCK,-iX,AbsY(GetY(i)));
			SetX(GetX(ReplaceMe),i);
			RemoveObject(ReplaceMe);
			SetDir(!GetDir(),i);
		}
	}
		SetComDir(COMD_Left);
		return(1);
	}
}

func ControlRight(){
	[$TxtRight$]
	if(!iFuel){
		if(GetAction() == "Fuel") SetAction("NoFuel");
		SetComDir(COMD_Stop);
		warn(Par(0));
		Sound("StartFail");
		return(1);
	}else{
		if(GetAction() == "NoFuel") SetAction("Fuel");
	}
	if(GetAction() == "Fuel"){
			if(!CanTurn()) return(SetComDir(COMD_Stop));
			if(GetDir() == DIR_Left){
		//relative rotation
		var Grabbers = FindObjects(Find_Action("Push"),Find_ActionTarget(this()));
		for(var i in Grabbers){
			var iX = AbsX(GetX(i));
			var ReplaceMe = CreateObject(ROCK,-iX,AbsY(GetY(i)));
			SetX(GetX(ReplaceMe),i);
			RemoveObject(ReplaceMe);
			SetDir(!GetDir(),i);
		}
	}
		SetComDir(COMD_Right);
		return(1);
	}
}

func ControlUp(){
	[$TxtUp$]
	if(CanLand()){
	SetComDir(COMD_Stop);
	var XDir;
	SetY(GetY()-3);
	if(GetDir() == DIR_Left) XDir = -25;
	if(GetDir() == DIR_Right) XDir = 25;
	var YDir = -50;
	ChangeDef(ChangeBack());
	this()->SetXDir(XDir);
	this()->SetYDir(YDir);
	Sound("Catapult");
	for(var Obj in FindObjects(Find_Distance(30), Find_Or( Find_Category(C4D_Vehicle), Find_Category(C4D_Living), Find_Category(C4D_Object)), Find_Exclude(this()))){
			Fling(Obj,XDir,YDir,10);
	}
	}
}

func ControlDig(){
	[$TxtStop$]
	if(GetAction() == "Fuel"){
		SetComDir(COMD_Stop);
		return(1);
	}
}

func ControlThrow(pClonk){
	[$TxtRefuel$]
	if(iFuel < 600){
		if(GetID(pClonk->Contents(0)) == OBRL){
			var Barl = pClonk->Contents(0);
			iFuel += Barl->GetAmount();
			ChangeDef(BARL,Barl);
			Sound("Splash1");
		}
	}
}

func Movement(){
	if(iFuel > 600) iFuel = 600;
	if(iFuel <= 0){
		iFuel = 0;
		SetComDir(COMD_Stop);
		Sound("Discharge");
		SetAction("NoFuel");
		return(1);
	}
	if(!CanTurn()) return(SetComDir(COMD_Stop));
	if(GetComDir() == COMD_Left){
		SetDir(DIR_Left);
		SetXDir(-25);
	}
	if(GetComDir() == COMD_Right){
		SetDir(DIR_Right);
		SetXDir(25);
	}
	if(GetComDir() != 0){
			if(!Random(4)) iFuel--;
	}
}

func ControlUpdate(self, int comdir, bool dig, bool throw){
	if(!CanTurn()) return(SetComDir(COMD_Stop));
	if(GetAction() == "Fuel"){
		SetComDir(COMD_Stop);
		if(comdir == COMD_Left || comdir == COMD_UpLeft || comdir == COMD_DownLeft) SetComDir(COMD_Left);
		if(comdir == COMD_Right || comdir == COMD_UpRight || comdir == COMD_DownRight) SetComDir(COMD_Right);
		return(1);
	}
}

//checks
func CanTurn(){
	if(!iFuel) return(0);
	if(GBackLiquid(0,5)) return(1);
}

func ConvertBack(){
	if(!CanTurn() && (GetContact(this(), -1) & CNAT_Bottom)) return(1);
}

func ChangeBack(){ return(PONT); }

func CanLand(){
	if(!GBackLiquid(0,5)) return(0);
	var X = GetX();
	var Y = GetY();
	var XDir;
	if(GetDir() == DIR_Left) XDir = -25;
	if(GetDir() == DIR_Right) XDir = 25;
	var YDir = -50;
	if(SimFlight(X,Y,XDir,YDir,25,49,38*3)) return(0);
	if(SimFlight(X,Y,XDir,YDir,50,100,38*3)) return(1);
}

func CheckGround(){
	if(FindObject(FUDS)) iFuel = 600;
		//stop the flying glitch
	if(!GBackLiquid(0,5) && !Stuck() && !GBackSolid() && GetXDir() == 0) ChangeDef(ChangeBack());
	if(ConvertBack()) ChangeDef(ChangeBack());
}
//end of checks

func warn(Clonk){
	var Owner = GetController(Clonk)+1;
	FuelWarn(this(),OBRL,Owner);
}

func Incineration(){
	Explode(45);
}

//INFOBAR
public func InfobarTrigger(){
	return(IB_Grab);
}

public func InfobarMax(){
	return(600);
}

public func InfobarValue(){
	return(iFuel);
}

public func InfobarColor(){
	return(RGBa(25,25,25));
}

public func InfobarInfo(){
	return("$InfoFuel$");
}

public func IsAdvancedProduct(){ return(1); }