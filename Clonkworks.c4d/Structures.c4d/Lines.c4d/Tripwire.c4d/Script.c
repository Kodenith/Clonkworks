/*--- Zuflussrohr ---*/

#strict
local Dontdetect;

//Detection variables
local detectObjects;
local detectLiving;
local detectVehicle;
local detectPlayers;

protected func Initialize()
{
  Local(0) = 70;
  Local(1) = 70;
	
  Dontdetect = [];
  
  detectLiving = true;
  detectObjects = true;
  detectVehicle = true;
  detectPlayers = [];
  for(var i = 0; i < GetPlayerCount(); i++)
  ArrayAdd(detectPlayers,i);
  
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

public func DebugGet(){
	DebugLog("Not allowed: %v",Dontdetect);
	
	  var from = GetActionTarget(0);
		var to = GetActionTarget(1);
	var InLineObjects = FindObjects(Find_OnLine(GetX(from),GetY(from),GetX(to),GetY(to)), Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Object),Find_Category(C4D_Living)), Find_NoContainer(), Find_Exclude(from),Find_Exclude(to));
	DebugLog("Line: %v",InLineObjects);
}

protected func Transfer()
{
  if(Contained()) Exit();
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);


  //Breaking if not straight. as this breaks the finding. 
  if(!PathFree(GetX(from),GetY(from),GetX(to),GetY(to))){
	  LineBreak(false);
	  RemoveObject(this());
	  return(0);
  }
  
  //custom break length for short lines
  if(ObjectCount(RSWR))
  if(ObjectDistance(from,to) > 300){
		LineBreak(false);
		RemoveObject(this());
		return(0);
  };
  
  var InLineObjects = [];
  if(detectObjects){
	  ArrayAddArray(InLineObjects,FindObjects(Find_OnLine(GetX(from),GetY(from),GetX(to),GetY(to)), Find_Category(C4D_Object), Find_NoContainer(), Find_Exclude(from),Find_Exclude(to)));
  }
  if(detectVehicle){
	  ArrayAddArray(InLineObjects,FindObjects(Find_OnLine(GetX(from),GetY(from),GetX(to),GetY(to)), Find_Category(C4D_Vehicle), Find_NoContainer(), Find_Exclude(from),Find_Exclude(to)));
  }
  if(detectLiving){
	  ArrayAddArray(InLineObjects,FindObjects(Find_OnLine(GetX(from),GetY(from),GetX(to),GetY(to)), Find_Category(C4D_Living), Find_NoContainer(), Find_Exclude(from),Find_Exclude(to)));
  }
  
  var doActivate = false;
  //check for new objects
  for(var new in InLineObjects){
	  if(InArray(new,Dontdetect) == -1){
		  if(GetOwner(new) != NO_OWNER){
		  if(InArray(GetOwner(new),detectPlayers) == -1) continue;
		  }
		  doActivate = true;
		  ArrayAdd(Dontdetect,new);
	  }
  }

  //check for objects not on line.
  for(var old in Dontdetect){
	  if(InArray(old,InLineObjects) == -1){
		  ArrayDeleteEntry(Dontdetect,old);
	  }
  }
  
  //activation
  if(!Contained(from) && !Contained(to) && doActivate)
  Activation();
  
}

public func Activation(){
	//DebugLog("Activated!");
	if(GetActTime() < 5) return(0);
	  var from = GetActionTarget(0);
	  var to = GetActionTarget(1);
	  
	  from->Sound("Click");
	  to->Sound("Click");
	  
	  from->~TripActivate();
	  to->~TripActivate();
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}

private func BreakMessage()
{
  var pPumpTarget = GetActionTarget(0);
  if (GetID(pPumpTarget) != FNKT)
    pPumpTarget = GetActionTarget(1);
  Message("$TxtLinebroke$", pPumpTarget);
}

public func CustomLengthLimit(){ return(1); }

public func KitType(){ return(FNKT); }

//tweakability
public func LineTweakable(){ return(1); }
public func TweakMenu(pClonk){
	if(!isOnLineTarget(pClonk)) return(0);
	CreateMenu(GetID(),pClonk,this(),0,"$TxtMenuTitle$",,1);
	
	var icon;
	if(detectObjects) icon = GOLD;
	else icon = ROCK;
	AddMenuItem("$TxtObj$","ToggleObj",icon,pClonk,0,pClonk);
	
	if(detectVehicle) icon = GOLD;
	else icon = ROCK;
	AddMenuItem("$TxtVehicle$","ToggleVehicle",icon,pClonk,0,pClonk);
	
	if(detectLiving) icon = GOLD;
	else icon = ROCK;
	AddMenuItem("$TxtLiving$","ToggleLiving",icon,pClonk,0,pClonk);
	
	//players
	AddMenuItem("$TxtDevider$",,,pClonk);
	for(var i = 0; i < GetPlayerCount(); i++){
		if(InArray(i,detectPlayers) != -1) icon = GOLD;
		else icon = ROCK;
		
		var packedVal = 256*ObjectNumber(pClonk) + i;
		AddMenuItem(Format("$TxtPlayer$",GetTaggedPlayerName(i)),"TogglePlayer",icon,pClonk,0,packedVal);
	}
}

func ToggleVal(ico,str){
	var bl = false;
	if(ico == ROCK) bl = true;
	LocalN(str) = bl;
}

public func ToggleObj(ico,pClonk){
	if(!isOnLineTarget(pClonk)) return(0);
	ToggleVal(ico,"detectObjects");
	pClonk->Sound("Click");
	TweakMenu(pClonk);
}

public func ToggleVehicle(ico,pClonk){
	if(!isOnLineTarget(pClonk)) return(0);
	ToggleVal(ico,"detectVehicle");
	pClonk->Sound("Click");
	TweakMenu(pClonk);
}

public func ToggleLiving(ico,pClonk){
	if(!isOnLineTarget(pClonk)) return(0);
	ToggleVal(ico,"detectLiving");
	pClonk->Sound("Click");
	TweakMenu(pClonk);
}

public func TogglePlayer(ico,Packed){
	var pClonk = Object(Packed/256);
	var iPlr = Packed%256;
	
	if(!isOnLineTarget(pClonk)) return(0);
	
	if(ico == ROCK) ArrayAdd(detectPlayers,iPlr);
	else ArrayDeleteEntry(detectPlayers,iPlr);
	
	pClonk->Sound("Click");
	TweakMenu(pClonk);
}

public func KitType(){ return(FNKT); }