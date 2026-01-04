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