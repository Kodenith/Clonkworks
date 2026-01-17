/*-- Neues Script --*/

#strict 2
#include BAS2
#include CXEC

func LeftVert(){ return(4); }
func RightVert(){ return(5); }
func CenterVert(){ return(7); }

local LeftSects, RightSects;
local Holder;
local Blasted;

func Initialize(){
	Blasted = false;
	SetEntrance(0);
	LeftSects = [];
	RightSects = [];
	NewSectLeft(); NewSectRight();
	CleanSectArrays();
}

public func GetLeftestSect(){
	return(LeftSects[GetLength(LeftSects)-1]);
}

public func GetRightestSect(){
	return(RightSects[GetLength(RightSects)-1]);
}

global func SetActionAttachment(AttachedVert,OwnVert,Obj){
	SetActionData(256*OwnVert+AttachedVert,Obj);
}

func SpawnNewBeam(int x) {
    var UseableColumns = [QUS0, QUS1, QUS2, QUS3, QUS4];
    
    var dist = 0;
    var search = 120;
    while(dist < search && !GBackSolid(x, dist)){
        dist++;
    }
    if(dist >= search) return(0);
    var bestBeam = 0;
    var bestSize = 10000;
	for(var definition in UseableColumns){
        var beamSize = DefinitionCall(definition, "SupportSize");
        if(beamSize >= (dist + 30) && beamSize < bestSize){
            bestSize = beamSize;
            bestBeam = definition;
        }
    }
    if(bestBeam){
		return(CreateConstruction(bestBeam,x,DefinitionCall(bestBeam, "SupportSize")-38,NO_OWNER,100,1,0));
        //return CreateObject(bestBeam, x, DefinitionCall(bestBeam, "SupportSize")-38, NO_OWNER);
    }
    
    return(0);
}

public func NewSectLeft(){
	var Obj = GetLeftestSect();
	var Sect;
	if(!Obj){
		Sect = CreateObject(QUA2,0,0,GetOwner());
		Sect->SetAction("Attached");
		SetActionTargets(this(),0,Sect);
		SetActionAttachment(LeftVert(),1,Sect);
	}else{
		Sect = CreateObject(QUA2,0,0,GetOwner());
		Sect->SetAction("Attached");
		SetActionTargets(Obj,0,Sect);
		SetActionAttachment(0,1,Sect);
	}
	
	if(GetLength(LeftSects)%6 == 0){
		var sup = SpawnNewBeam(AbsX(GetX(GetLeftestSect())-22));
		//DebugLog("%v",sup);
		if(sup){
			LocalN("Support",Sect) = sup;
			LocalN("Sect",sup) = Sect;
		}
	}
	ArrayAdd(LeftSects,Sect,1);
	//CleanSectArrays();
	return(Sect);
}

public func NewSectRight(){
	var Obj = GetRightestSect();
	var Sect;
	if(!Obj){
	    Sect = CreateObject(QUA2,0,0,GetOwner());
		Sect->SetAction("Attached");
		SetActionTargets(this(),0,Sect);
		SetActionAttachment(RightVert(),0,Sect);
	}else{
		Sect = CreateObject(QUA2,0,0,GetOwner());
		Sect->SetAction("Attached");
		SetActionTargets(Obj,0,Sect);
		SetActionAttachment(1,0,Sect);
	}
	
	if(GetLength(RightSects)%6 == 0){
		var sup = SpawnNewBeam(AbsX(GetX(GetRightestSect())+22));
		//DebugLog("%v",sup);
		if(sup){
			LocalN("Support",Sect) = sup;
			LocalN("Sect",sup) = Sect;
		}
	}
	ArrayAdd(RightSects,Sect,1);
	//CleanSectArrays();
	return(Sect);
}

func CleanSectArrays(){
	var newLeft = [];
	for(var i in LeftSects){
		if(i) ArrayAdd(newLeft,i,1);
	}
	LeftSects=newLeft;
	
	var newRight = [];
	for(var i in RightSects){
		if(i) ArrayAdd(newRight,i,1);
	}
	RightSects=newRight;
}

func Logic(){
	if(GetCon() < 100) return(0);
	if(Abs(FrameCounter())%15 == 0){
		if(GetLength(LeftSects) < 1) NewSectLeft();
		if(GetLength(RightSects) < 1) NewSectRight();
		CleanSectArrays();
	}
	
	if(Abs(FrameCounter())%2 == 0 && FindObject(ENRG)){
		DoEnergy(-3);
	}
	
	if(GetLength(LeftSects) > 1 && GetLeftestSect() != 0 && Stuck(GetLeftestSect())){
		CreateObject(CPIG,AbsX(GetX(GetLeftestSect())),AbsY(GetY(GetLeftestSect())));
		RemoveObject(GetLeftestSect());
	}
	if(GetLength(RightSects) > 1 && GetRightestSect() != 0 && Stuck(GetRightestSect())){
		CreateObject(CPIG,AbsX(GetX(GetRightestSect())),AbsY(GetY(GetRightestSect())));
		RemoveObject(GetRightestSect());
	}
	
	if(!Holder){
		Holder = CreateObject(QUA3,0,GetVertex(CenterVert(),1));
		Holder->SetAction("Float");
		SetComDir(COMD_Stop,Holder);
	}else{
		if(GetX(Holder) < GetX(GetLeftestSect())-10){
			SetX(GetX(GetLeftestSect())-10,Holder);
			SetComDir(COMD_Stop,Holder);
			Holder->Sound("Click");
			Holder->FinishCommand();
		}
		if(GetX(Holder) > GetX(GetRightestSect())+10){
			SetX(GetX(GetRightestSect())+10,Holder);
			SetComDir(COMD_Stop,Holder);
			Holder->Sound("Click");
			Holder->FinishCommand();
		}
		SetY(GetY()+GetVertex(CenterVert(),1),Holder);
		SetYDir(0,Holder);
		
			if(!EnergyCheck(1)){
			SetComDir(COMD_Stop,Holder);
			return(1);
			}
	
		if(GetComDir(Holder) == COMD_Stop && !GetCommand(Holder)) SetXDir(0,Holder);
	}
}

//Holder Control
//Classic
func ControlLeft(){
	[$TxtLeft$|Image=CT01]
	if(!EnergyCheck(1)){
		Sound("Error");
		return(1);
	}
	Sound("Click");
	if(Holder) SetComDir(COMD_Left,Holder);
}

func ControlRight(){
	[$TxtLeft$|Image=CT01:2]
	if(!EnergyCheck(1)){
		Sound("Error");
		return(1);
	}
	Sound("Click");
	if(Holder) SetComDir(COMD_Right,Holder);
}

func ControlDig(){
	Sound("Click");
	if(Holder) SetComDir(COMD_Stop,Holder);
}

//JNR (only stopping as thats the only one neccesarry really)
public func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  if(comdir == COMD_Stop) ControlDig();
}

//Mouse
func ControlCommand(strCommand,pTarget,iTx,iTy,pTarget2,iData,pCmdObj){
	if(!EnergyCheck(1)){
		Sound("Error");
		return(1);
	}
	if(strCommand == "MoveTo" && Holder){
		SetCommand(Holder,"MoveTo",,iTx,GetY(Holder));
		return(1);
	}
}

//Entering Cannon Heads.
func ActivateEntrance(pObj){
	if(pObj->~IsCannon() && Holder && !Holder->HasCannon()){
		pObj->Connect(Holder);
		LocalN("cannon",Holder) = pObj;
		SetRDir(0,pObj);
		SetR(180, pObj);
	}
}

//Menu
func ControlThrow(pObj){
	[$TxtMenu$|Image=CXTX]
	OpenMenu(pObj);
	return(1);
}

public func OpenMenu(pClonk){
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	CreateMenu(CXTX,pClonk,this(),0,"$TxtMenu2$");
	
	var Size;
	
	//Putting
	if(Contents(0,pClonk)){
		AddMenuItem("$TxtPut$","PutMainItem",GetID(Contents(0,pClonk)),pClonk,0,pClonk);
		Size++;
	}
	
	//Firing
	if(Holder && Holder->HasCannon()){
		AddMenuItem("$TxtFire$","FireCannon",CT01,pClonk,0,pClonk,"",2,1);
		Size++;
	}
	
	//Firing
	if(Holder && Holder->HasCannon()){
		AddMenuItem("$TxtDismantle1$","DetachCannon",CT01,pClonk,0,pClonk,"",2,3);
		Size++;
	}
	
	//Adding New Parts
	AddMenuItem("$TxtNewPart$","ConstructLeft",QUAS,pClonk,0,pClonk);
	AddMenuItem("$TxtNewPart$","ConstructRight",QUAS,pClonk,0,pClonk,"",2,2);
	Size+=2;
	
	//Destructing parts
	if(GetLength(LeftSects)>1){
		AddMenuItem("$TxtDestPart$","RemoveLeft",QUAS,pClonk,0,pClonk,"",2,1);
		Size++;
	}
	if(GetLength(RightSects)>1){
		AddMenuItem("$TxtDestPart$","RemoveRight",QUAS,pClonk,0,pClonk,"",2,3);
		Size++;
	}
	
	//Inventory
	if(ContentsCount(,Holder) > 0 || (LocalN("cannon",Holder) && ContentsCount(,LocalN("cannon",Holder))) ){
		AddMenuItem("$TxtInventory$","InventoryMenu",CXIV,pClonk,0,pClonk);
		Size++;
	}
	
	SetMenuSize(Size,1,pClonk);
}

func InventoryMenu(foo,pClonk){
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	CreateMenu(CXIV,pClonk,this(),0,"$TxtInventory$");
	var i,v;
	while(v = Contents(i++,Holder,false)){
		AddMenuItem("$TxtInventory$: %s",Format("RequestItem(0,%d,%d)",ObjectNumber(pClonk),ObjectNumber(v)),GetID(v),pClonk);
	}
	if(LocalN("cannon",Holder)){
		while(v = Contents(i++,LocalN("cannon",Holder),false)){
			AddMenuItem("$TxtInventory$: %s",Format("RequestItem(0,%d,%d)",ObjectNumber(pClonk),ObjectNumber(v)),GetID(v),pClonk);
		}
	}
}

func RequestItem(foo,clk,itm){
	var pClonk = Object(clk);
	var pItem = Object(itm);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	Exit(pItem);
	SetPosition(GetX()+GetVertex(0,0),GetY()+GetVertex(0,1)-5,pItem);
	Sound("Grapple");
	InventoryMenu(,pClonk);
}

func RemoveLeft(){
		CleanSectArrays();
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	
	if(GetLength(LeftSects)>1){
		RemoveObject(GetLeftestSect());
		Sound("Click");
	}
	
	if(!GetMenu(pClonk)) OpenMenu(pClonk);
}

func RemoveRight(){
		CleanSectArrays();
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	
	if(GetLength(RightSects)>1){
		RemoveObject(GetRightestSect());
		Sound("Click");
	}
	
	CleanSectArrays();
	if(!GetMenu(pClonk)) OpenMenu(pClonk);
}

func ConstructLeft(){
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	
	if(FindObject2(Find_Container(Holder),Find_ID(CPIG))){
		RemoveObject(FindObject2(Find_Container(Holder),Find_ID(CPIG)));
		NewSectLeft();
		Sound("Connect");
	}else{
		Sound("Error");
		Message("$TxtNewPartError$",this());
	}
	
	CleanSectArrays();
	if(!GetMenu(pClonk)) OpenMenu(pClonk);
}

func ConstructRight(){
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	
	if(FindObject2(Find_Container(Holder),Find_ID(CPIG))){
		RemoveObject(FindObject2(Find_Container(Holder),Find_ID(CPIG)));
		NewSectRight();
		Sound("Connect");
	}else{
		Sound("Error");
		Message("$TxtNewPartError$",this());
	}
	
	if(!GetMenu(pClonk)) OpenMenu(pClonk);
}

func FireCannon(){
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	if(Holder && Holder->HasCannon()){
		LocalN("cannon",Holder)->ComFire(pClonk);
	}
	
	if(!GetMenu(pClonk)) OpenMenu(pClonk);
}

func DetachCannon(){
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	if(Holder && Holder->HasCannon()){
		Holder->ReleaseCannon();
	}
	
	if(!GetMenu(pClonk)) OpenMenu(pClonk);
}

func PutMainItem(){
	var pClonk = Par(1);
	if(GetAction(pClonk) != "Push" || GetActionTarget(0,pClonk) != this()) return(0);
	if(Contents(0,pClonk)){
		Enter(Holder,Contents(0,pClonk));
		Sound("Grab");
	}
	
	OpenMenu(pClonk);
}

//reject grab
func RejectGrabbed(pClonk){
	if(Inside(AbsX(GetX(pClonk)),-10,10)) return(0);
	return(1);
}

//incineration & Destruction
func Damage(){
	if(GetDamage() > 80 && !Blasted){
		Blasted = true;
		if(GetLength(LeftSects)) Incinerate(LeftSects[0]);
		if(GetLength(RightSects)) Incinerate(RightSects[0]);
		if(Holder){
			var i, v;
			while(v = Contents(i++,Holder,false)){
				Exit(v);
			}
			Holder->ReleaseCannon();
			Holder->Explode(20);
		}
		
		for(var y = -20; y < 60; y+= 20){
			var sect;
			sect = CreateObject(QUA4,0,y);
			sect->Incinerate();
			SetR(90,sect);
		}
		
		for(var x = -40; x < 60; x+=20){
			var sect;
			sect = CreateObject(QUA4,x,-20);
			sect->Incinerate();
			//SetR(90,sect);
		}
		
		RemoveObject();
	}
}

func Incineration(){ DoDamage(101); }

func Destruction(){ if(Holder) RemoveObject(Holder); }

//other
func IsAdvancedProduct(){ return(1); }
func GetResearchBase(){ return(EXDR); }
func RejectContents(){ return(1); }