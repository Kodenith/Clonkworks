
#strict 2

local Topic;
local Command;
local Timer;
local Par1,Par2,Par3;

func Initialize(){
	Timer = 60;
	SetPosition(0,0);
}

global func CastVote(fromPlr,iTopic,iCommand,iPar1,iPar2,iPar3){
	if(FindObject(___V)){
		PlrMessage("Please wait for the current vote to end before passing another one.",fromPlr-1);
		return(0);
	}
	var vote = CreateObject(___V);
	LocalN("Topic",vote) = iTopic;
	LocalN("Command",vote) = iCommand;
	LocalN("Par1",vote) = iPar1;
	LocalN("Par2",vote) = iPar2;
	LocalN("Par3",vote) = iPar3;
	if(fromPlr){
		Local(fromPlr-1,vote) = 1;
	}
	
	Message("$TxtNotification$",,iTopic);
}

func Tick(){
	Timer--;
	
	if(Timer <= 0){
			FinishVote();
			return(0);
	}
	
	if(Timer <= 10){
		Message("$TxtCooldown$",,Timer);
		Sound("CatapultSet",1);
	}
}

func FinishVote(){
	var Ok;
	for(var i = 0; i<GetPlayerCount(); i++){
		if(Local(GetPlayerByIndex(i)) == 1) Ok++;
	}
	
	if(Ok>(GetPlayerCount()/2)){
		Message("$TxtWin$");
		Sound("Blast3",1);
		if(!GameCallEx(Command,Par1,Par2,Par3)) eval(Format(Command,Par1,Par2,Par3));
	}else{
		Message("$TxtLose$");
		Sound("Discharge",1);
	}
	RemoveObject();
}

func Activate(iPlr){
	var pClonk = GetCursor(iPlr);
	if(Local(iPlr) != 0){
		MessageWindow("$Info3$",iPlr);
	}else{
		CreateMenu(GetID(),pClonk,this(),0,GetName(),0,1);
		AddMenuItem("$TxtInfo$",,,pClonk);
		AddMenuItem(Topic,,QUE_,pClonk);
		AddMenuItem("$TxtInfo2$",,,pClonk);
		
		AddMenuItem("$Y$","Confirm",YES_,pClonk,0,iPlr);
		AddMenuItem("$N$","Reject",NO__,pClonk,0,iPlr);
	}
}

func Confirm(){
	var iPlr = Par(1);
	Local(iPlr) = 1;
	Sound("Ding",0,GetCursor(iPlr),0,iPlr+1);
}

func Reject(){
	var iPlr = Par(1);
	Local(iPlr) = -1;
	Sound("Error",0,GetCursor(iPlr),0,iPlr+1);
}