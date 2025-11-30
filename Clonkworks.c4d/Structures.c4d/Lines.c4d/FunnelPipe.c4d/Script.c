/*--- Zuflussrohr ---*/

#strict
local Filter;

protected func Initialize()
{
  Local(0) = 1;
  Local(1) = 4;
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  if(Contained()) Exit();
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);
  
  if(from && to){
	  //getting max space
	  var space;
	  space = GetDefCoreVal("CollectionLimit", "DefCore", GetID(to));
	  if(space == 0) space = to->~MaxContents();
	  if(space == 0) space = 9999;
	  
	  if(GetAction(to) eq "DoorOpen") return(0); //im doing this so it doesnt get stuck on the open animation.
	  if(GetAction(from) eq "DoorOpen") return(0); // this too, apparently i forgot to do this previosuly
	  var ItemList = FindObjects(Find_Container(from), Find_OCF(OCF_Collectible), Sort_Random());
	  if(Filter) ItemList = FindObjects(Find_Container(from), Find_OCF(OCF_Collectible), Find_ID(Filter), Sort_Random());
	  var MoveItem = ItemList[0];
	  if(from->~CanNotBeDispensedFrom(MoveItem)) return(0);
	  if(GetOCF(to) & OCF_Container){
		  if(ContentsCount(,to) < space){
			  if(!ObjectCall(MoveItem,"RejectEntrance",to)) 
			  if(MoveItem != this() && to->~CanNotBeDispensedInto(MoveItem) != 1)
			  Enter(to, MoveItem);
		  }
	  }
	  else if(GetOCF(to) & OCF_Collectible){
		  var cont;
		  cont = Contained(to);
		  if(GetOCF(cont) & OCF_Alive) return(0);
		  if(cont){
			 if(ContentsCount(,cont) < space){
			  if(!ObjectCall(MoveItem,"RejectEntrance",to)) 
			  if(MoveItem != this() && cont->~CanNotBeDispensedInto(MoveItem) != 1)
			  Collect(cont, MoveItem);
			 }
		  }
		  else{
		  if(GetID(to) == FNKT) return(0);
		  Exit(MoveItem);
		  SetPosition(GetX(to), GetY(to), MoveItem);
		  }
	  }
  }
  
  if(Contained()) Exit();
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

public func KitType(){ return(FNKT); }

//tweakability
public func LineTweakable(){ return(1); }

public func TweakMenu(pClonk){
	if(!isOnLineTarget(pClonk)) return(0);
	
	CreateMenu(GetID(),pClonk,this(),0,"$TxtMenuTitle$",,1);
	if(!Filter){
		AddMenuItem("$TxtFilterSet$","SetFilterMenu",,pClonk,0,pClonk);
	}else{
		AddMenuItem("$TxtFilterCurrent$","RemoveFilter",Filter,pClonk,0,pClonk);
	}
	
	AddMenuItem("$TxtSwap$","SwapTargets",FNPP,pClonk,0,pClonk);
}

public func SwapTargets(foo,pClonk){
		if(pClonk){
		if(!isOnLineTarget(pClonk)) return(0);
		}
	  var from = GetActionTarget(0);
	  var to = GetActionTarget(1);
	  var from = GetActionTarget(0);
	  var to = GetActionTarget(1);
	  SetActionTargets(to,from);
	  	if(pClonk){
			Message("$TxtSwapConf$",pClonk,GetName(from),GetName(to));
			pClonk->Sound("Click");
			TweakMenu(pClonk);
		}
}

func SetFilterMenu(foo,pClonk){
	if(!isOnLineTarget(pClonk)) return(0);
	
	CreateMenu(GetID(),pClonk,this(),0,"$TxtFilterSet$");
	var i = 0;
	var def = 0;
	while(def = GetDefinition(i,C4D_Object)){
		i++;
		var ok = false;
		if(GetCategory(,def) & 512) ok = true;
		if(GetCategory(,def) & 1024) ok = true;
		if(GetCategory(,def) & 2048) ok = true;
		if(GetCategory(,def) & 16384) ok = true;
		if(GetCategory(,def) & C4D_Structure) ok = false;
		if(GetComponent(DUMM,,,def) > 0) ok = false;
		if(!ok) continue;
		
		AddMenuItem("$TxtFilterSet$: %s","SetFilter",def,pClonk,0,pClonk);
	}
}

public func SetFilter(Id,pClonk){
	if(pClonk){
		if(!isOnLineTarget(pClonk)) return(0);
	}
	Filter = Id;
	if(pClonk){
		pClonk->Sound("Click");
		TweakMenu(pClonk);
	}
}

public func RemoveFilter(foo,pClonk){
	if(pClonk){
		if(!isOnLineTarget(pClonk)) return(0);
	}
	Filter = 0;
	if(pClonk){
		pClonk->Sound("Click");
		TweakMenu(pClonk);
	}
}