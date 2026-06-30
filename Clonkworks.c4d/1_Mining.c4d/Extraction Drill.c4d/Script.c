/*-- Neues Script --*/

#strict 2
#include EXDB
#include CXEC
#include IO__

local head;
local Left;

func Initialize() {
  SetAction("Lift");
  head = CreateObject(EXDC,1,20);
  SetActionTargets(head,0,this());
  LocalN("Owner",head) = this();
  return(_inherited());
}

func Destruction(){
	if(head) RemoveObject(head);
}

func WireFrom(){ return(0); }
public func InputList(){ return(["Stop","Drill"]); }

func Update(){
	if(GetCon() < 100) return(0);

	if(InputActive("Stop")) ControlUpSingle();
	else if(InputActive("Drill")) ControlDownSingle();

	if(Abs(FrameCounter()) % 15 == 0){

	if(FindObject(ENRG) && GetComDir(head) == COMD_Down) DoEnergy(-3);
	
	if(!head){
		head = CreateObject(EXDC,1,20);
		SetActionTargets(head,0,this());
		LocalN("Owner",head) = this();
	}
	
	if(Contents(0)){
		if(!Left)
		Exit(Contents(0),GetVertex(6,0),GetVertex(6,1),0,3);
		else Exit(Contents(0),-GetVertex(6,0),GetVertex(6,1),0,-3);
	}

	}
}

//control
func ControlDownSingle(pClonk){
	[$TxtDown$|Image=EXDC:2]
	SetComDir(COMD_Down,head);
	LocalN("SavedDir",head) = COMD_Down;
	return(1);
}

//control
func ControlUpSingle(pClonk){
	[$TxtUp$|Image=EXDC:1]
	if(GetY(head)-1 < GetY()) return(1);
	SetComDir(COMD_Up,head);
	LocalN("SavedDir",head) = COMD_Up;
	return(1);
}

//turn
func ControlLeft(pClonk){
	[$TxtTurnLeft$]
	if(Left) return(0);
	SetGraphics("Left",this,GetID());
	Sound("Click");
	Left = true;
}

//turn
func ControlRight(pClonk){
	[$TxtTurnRight$]
	if(!Left) return(0);
	SetGraphics(0,this,GetID());
	Sound("Click");
	Left = false;
}


//mouse
func ControlCommand(strCommand,pTarget,iTx,iTy,pTarget2,iData,pCmdObj){
	if(strCommand == "MoveTo"){
		SetCommand(head,"MoveTo",0,GetX(),iTy);
		return(1);
	}
}

func IsAdvancedProduct(){ return(1); }
func GetResearchBase(){ return(EFLN); }

func CmdPushOutHelper(pObj){
	SetCommand(pObj,"Exit");
}

//malfint
func Malfunction(){
	ControlUpSingle();
}