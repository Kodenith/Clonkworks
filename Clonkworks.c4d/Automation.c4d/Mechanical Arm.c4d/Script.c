/*-- Neues Script --*/

#strict 2
#include BAS2
#include CXEC
local Claw;
local PointerList;
local SelectedPointer;
local Rope;

func Initialize() {
  SetAction("Rotate");
  SetPhase(4);
  CreateClaw();
  PointerList = [];
  return(_inherited());
}

func CreateClaw(){
	Claw = CreateObject(MAM2,0,40);
	ClawResort();
	var rope = CreateObject(CK5P);
	rope->ConnectObjects(this(),Claw);
	LocalN("fNoPickUp_0",rope) = true;
	LocalN("fNoPickUp_1",rope) = true;
	rope->SetRopeLength(100);
	
	Claw->SetAction("Moving");
	SetCommand(Claw,"MoveTo",0,GetX(),GetY()+40);
	Rope = rope;
}

func NewFilter(cID){
	var new = CreateObject(MAM3,,25);
	LocalN("Avoid",new) = this();
	new->Set(cID);
	ArrayAdd(PointerList,new);
	SetVisibility(VIS_None,new);
	return(new);
}

func Logic(){
	if(GetCon() < 100) return(0);
	if(!Claw) CreateClaw();
	if(GetAction() != "Rotate") SetAction("Rotate");
	Claw->SetRDir(0);
	Claw->SetR(Angle(GetX(),GetY(),GetX(Claw),GetY(Claw))+180);
	
	if(Abs(GetActTime())%350==0){
		ClawResort();
	}
	
	if(!EnergyCheck(1)){
		Claw->SetAction("Idle");
		Claw->FinishCommand();
	}
	else if(GetAction(Claw) == "Idle"){
		Claw->SetAction("Moving");
		SetCommand(Claw,"MoveTo",0,GetX(),GetY()+45);
	}
	
	if(GetX(Claw) < GetX()-30) SetPhase(0);
	else if(GetX(Claw) < GetX()-20) SetPhase(1);
	else if(GetX(Claw) < GetX()-10) SetPhase(2);
	else if(GetX(Claw) < GetX()-5) SetPhase(3);
	else if(GetX(Claw) > GetX()+30) SetPhase(7);
	else if(GetX(Claw) > GetX()+20) SetPhase(6);
    else if(GetX(Claw) > GetX()+10) SetPhase(5);
	else if(GetX(Claw) > GetX()+5) SetPhase(4);
	
	//showing poimters
	if(FindObject2(Find_Action("Push"),Find_ActionTarget(this()))){
		for(var i in PointerList){
			SetVisibility(VIS_All,i);
			if(GetIndexOf(i,PointerList) == SelectedPointer){
				SetClrModulation(RGBa(255,255,255,0),i);
				SetClrModulation(RGBa(255,255,255,0),i,1);
			}else{
				SetClrModulation(RGBa(255,255,255,100),i,0);
				SetClrModulation(RGBa(255,255,255,100),i,1);
			}
		}
	}else{
		for(var i in PointerList){
			SetVisibility(VIS_None,i);
		}
	}
	
	//handling filters and moving
	if(!GetLength(PointerList)){
		NewFilter(GOLD);
		SelectedPointer = 0;
	}
	
	if(GetAction(Claw) == "Idle") return(0);
	
	//dont let the claw go beyond its limits
	if(ObjectDistance(this(),Claw) > 150){
		Claw->EndAllCommands();
		SetCommand(Claw,"MoveTo",0,GetX(),GetY()+40);
		return(0);
	}
	
	if(!GetCommand(Claw)){
		if(LocalN("Grabtarg",Claw)) Claw->Release();
		SetXDir(0,Claw); SetYDir(0,Claw);
		if(ObjectDistance(this(),Claw) < 50){
			RefreshRope();
		}
		if(FindNeedMove()){
			var need = FindNeedMove();
			var needId = GetID(need);
			for(var j in PointerList){
				if(LocalN("Filt",j) == needId){
					Claw->GrabAndDropOff(need,GetX(j),GetY(j));
					return(1);
				}
			}
		}else{
			SetCommand(Claw,"MoveTo",0,GetX(),GetY()+40);
		}
	}
}

func FindNeedMove(){
	for(var i in PointerList){
		if(i->GetUseable()) return(i->GetUseable());
	}
}

func RefreshRope(){
	if(Rope) RemoveObject(Rope);
	var rope = CreateObject(CK5P);
	rope->ConnectObjects(this(),Claw);
	LocalN("fNoPickUp_0",rope) = true;
	LocalN("fNoPickUp_1",rope) = true;
	rope->SetRopeLength(100);
	Rope = rope;
}

func ClawResort(){
	for(var i in FindObjects(Find_Category(C4D_Structure),Find_Not(Find_ID(MAM2)))){
		SetObjectOrder(i,Claw);
	}
	DebugLog("%v Resorted",Claw);
}

//Movement
func Incrementation(){ return(7); }

func ControlLeft(pClonk){
	[$TxtLeft$]
	var Move = PointerList[SelectedPointer];
	if(!Move) return(0);
	SetX(GetX(Move)-Incrementation(),Move);
	if(ObjectDistance(this(),Move) > 150) SetX(GetX(Move)+Incrementation(),Move);
	Sound("CatapultSet");
}

func ControlRight(pClonk){
	[$TxtRight$]
	var Move = PointerList[SelectedPointer];
	if(!Move) return(0);
	SetX(GetX(Move)+Incrementation(),Move);
	if(ObjectDistance(this(),Move) > 150) SetX(GetX(Move)-Incrementation(),Move);
	Sound("CatapultSet");
}

func ControlUp(pClonk){
	[$TxtUp$]
	var Move = PointerList[SelectedPointer];
	if(!Move) return(0);
	SetY(GetY(Move)-Incrementation(),Move);
	if(ObjectDistance(this(),Move) > 150) SetY(GetY(Move)+Incrementation(),Move);
	Sound("CatapultSet");
}

func ControlDown(pClonk){
	[$TxtDown$]
	var Move = PointerList[SelectedPointer];
	if(!Move) return(0);
	SetY(GetY(Move)+Incrementation(),Move);
	if(ObjectDistance(this(),Move) > 150) SetY(GetY(Move)-Incrementation(),Move);
	Sound("CatapultSet");
}

func ControlDownDouble(pClonk){
//foo? this prevents ungrabbing. new ungrab key below
return(1);
}

func ControlDigSingle(pClonk){
	[$TxtUngrab$|Image=CXIV]
	pClonk->SetAction("Walk");
	Sound("Grab");
}

//MENU
//note: you can store 2 values in one using 256*x+y.
//deviding by 256 gives you x and modulo by 256 gives you y

public func ControlThrow(pClonk){
	[$TxtMenu$|Image=CXTX]
	if(GetProcedure(pClonk) != "PUSH" || GetActionTarget(0,pClonk) != this()) return(0);
	CreateMenu(CXTX,pClonk,this(),0,"$TxtMenu2$",0,1);
	AddMenuItem("$TxtOption1$","NewCursorMenu",MCMC,pClonk,0,pClonk);
	//denyamic options
	for(var i in PointerList){
		AddMenuItem("$TxtOption2$","Select",LocalN("Filt",i),pClonk,0,pClonk);
	}
}

func Select(){
	var pClonk = Par(1);
	if(GetProcedure(pClonk) != "PUSH" || GetActionTarget(0,pClonk) != this()) return(0);
	
	SelectedPointer = GetIndexOf(FilterExists(Par(0)),PointerList);
	if(SelectedPointer == -1) SelectedPointer = 0;
	Sound("Connect");
}

func NewCursorMenu(){
	var pClonk = Par(1);
	if(GetProcedure(pClonk) != "PUSH" || GetActionTarget(0,pClonk) != this()) return(0);
	
	CreateMenu(MCMC,pClonk,this(),0,"$TxtMenu2$",0);
	var i,j;
	while(j = GetDefinition(i++,C4D_Object)){
		var ok = false;
		if(GetCategory(,j) & 512) ok = true;
		if(GetCategory(,j) & 1024) ok = true;
		if(GetCategory(,j) & 2048) ok = true;
		if(GetCategory(,j) & 8192) ok = true;
		if(GetCategory(,j) & 16384) ok = true;
		if(GetCategory(,j) & 65536) ok = true;
		if(FilterExists(j)) ok = false;
		if(j==FLAG) ok = false;
		if(ok) AddMenuItem("$TxtOption2$","NewFilter2",j,pClonk,,pClonk);
	}
	
	i = 0; j = 0;
	while(j = GetDefinition(i++,C4D_Vehicle)){
		var ok = false;
		if(GetCategory(,j) & 512) ok = true;
		if(GetCategory(,j) & 1024) ok = true;
		if(GetCategory(,j) & 2048) ok = true;
		if(GetCategory(,j) & 8192) ok = true;
		if(GetCategory(,j) & 16384) ok = true;
		if(GetCategory(,j) & 65536) ok = true;
		if(GetMass(,j) > 75) ok = false;
		if(FilterExists(j)) ok = false;
		if(j==CNVY||j==BRDG||j==BALN) ok = false;
		if(ok) AddMenuItem("$TxtOption2$","NewFilter2",j,pClonk,,pClonk);
	}
}

func FilterExists(cID){
	for(var i in PointerList){
		if(LocalN("Filt",i) == cID) return(i);
	}
}

func NewFilter2(){
	var pClonk = Par(1);
	if(GetProcedure(pClonk) != "PUSH" || GetActionTarget(0,pClonk) != this()) return(0);
	Sound("Ding");
	Message("$TxtAdded$",this(),GetName(,Par(0)));
	NewFilter(Par(0));
}

//deletion
func ControlDigDouble(pClonk){
	[$TxtDelete$|Image=MCMK]
	if(GetLength(PointerList) < 2) return(0);
	if(PointerList[SelectedPointer]){
		RemoveObject(PointerList[SelectedPointer]);
		var newList = [];
		for(var i in PointerList){
			if(i) ArrayAdd(newList,i,true);
		}
		PointerList = newList;
		SelectedPointer = 0;
		Sound("Connect");
	}
}

//destruction
func Destruction(){
	if(Claw) RemoveObject(Claw);
	for(var i in PointerList) RemoveObject(i);
	return(_inherited());
}

//context
func Accesible(){ return(GetCon() > 99); }

public func ContextPackage(pClonk){
	[$TxtMenu$|Image=CXTX|Condition=Accesible]
	AddCommand(pClonk,"Call",this(),pClonk,0,0,38*999,"ControlThrow");
	AddCommand(pClonk,"Grab",this());
}

//mouse
func ControlCommand(strCommand,pTarget,iTx,iTy,pTarget2,iData,pCmdObj){
	if(strCommand == "MoveTo"){
		if(Distance(GetX(),GetY(),iTx,iTy) < 150){
			var Select = PointerList[SelectedPointer];
			SetPosition(iTx,iTy,Select);
			Sound("CatapultSet");
			return(1);
		}
	}
}

public func GetResearchBase(){
	return(FLTG);
}