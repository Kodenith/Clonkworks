/*-- Neues Script --*/

#strict 2
#include EXDB
#include CXEC

local head;

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

func Update(){
	if(GetCon() < 100) return(0);
	
	if(!head){
		head = CreateObject(EXDC,1,20);
		SetActionTargets(head,0,this());
		LocalN("Owner",head) = this();
	}
	
	if(Contents(0)){
		Exit(Contents(0),GetVertex(6,0),GetVertex(6,1),0,RandomX(2,4));
	}
}

//control
func ControlDownSingle(pClonk){
	[$TxtDown$|Image=EXDC:2]
	SetComDir(COMD_Down,head);
	return(1);
}

//control
func ControlUpSingle(pClonk){
	[$TxtUp$|Image=EXDC:1]
	SetComDir(COMD_Up,head);
	return(1);
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

//INFOBAR
public func InfobarTrigger(){
	if(head && FindObject(REXD)) return(IB_Grab);
}

public func InfobarMax(){
	return(500);
}

public func InfobarValue(){
	return(LocalN("Durability",head));
}

public func InfobarColor(){
	return(RGBa(255,215,0));
}

public func InfobarInfo(){
	return("$TxtInfobar$");
}

//Context
func NeedHead(){ return(LocalN("Durability",head) <= 0); }
public func ContextApply(pClonk){
	[$TxtApply$|Image=EXDH|Condition=NeedHead]
	var head2 = FindObject2(Find_ID(EXDH),Find_Distance(400),Find_Not(Find_Action("Broken")));
	if(head2){
		AddCommand(pClonk,"PushTo",head2,0,0,head);
	}else{
		Message("$TxtNoHeads$",pClonk);
		pClonk->Sound("CommandFailure1");
	}
}

func CmdPushOutHelper(pObj){
	SetCommand(pObj,"Exit");
}

//malfint
func Malfunction(){
	ControlUpSingle();
}